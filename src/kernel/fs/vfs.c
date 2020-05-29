#include <fs/vfs.h>
#include <fs/vfs_tree.h>
#include <kernel/debug.h>
#include <mm/kmalloc.h>
#include <stdbool.h>
#include <string.h>

static tree_t vfs_tree = NULL;

static vfs_node_t find_closest_mount(const char *path);
static void mount_root_node(vfs_node_t local_root);
static void mount_implementation(const char *path, vfs_node_t local_root);

void vfs_init()
{
	struct vfs_entry *root;

	root = kmalloc(sizeof(struct vfs_entry));
	root->name    = strdup("[root]");
	root->file    = NULL;
	root->fs_type = NULL;
	root->device  = NULL;

	vfs_tree = tree_create();
	vfs_tree->root = tree_node_create(root);
}

void vfs_mount(const char *path, vfs_node_t local_root)
{
	if(!vfs_tree) {
		debug_warning("VFS has not been initialized!\n");
		return;
	}

	if(!path || path[0] != '/') {
		debug_warning("Mount path must be absolute!\n");
		return;
	}

	local_root->refcount = -1;

	/* 2 cases:
	 * 1) Mounting to '/' (root node)
	 * 2) Mounting to anything else
	*/
	if(path[0] == '/' && path[1] == '\0')
		mount_root_node(local_root);
	else
		mount_implementation(path, local_root);

	debug_info("New Mount At: %s\n", path);
}

inline vfs_node_t vfs_create_node()
{
	void *ptr;
	
	ptr = kmalloc(sizeof(struct __vfs_node));
	memset(ptr, 0, sizeof(struct __vfs_node));

	return ptr;
}
inline void vfs_destroy_node(vfs_node_t node)
{
	kfree(node);
}

/* FileSystem Operations */
void vfs_open(vfs_node_t node, uint32_t flags)
{
	if(!node)
		return;

	if(node->open) {
		node->open(node, flags);
	}
}

void vfs_close(vfs_node_t node)
{
	if(!node)
		return;

	if(node->close) {
		node->close(node);
	}
}

ssize_t vfs_read(vfs_node_t node, uint64_t offset, void *buf, size_t nbyte)
{
	if(!node)
		return -1;

	if(node->read) {
		return node->read(node, offset, buf, nbyte);
	}
	return -1;
}

ssize_t vfs_write(vfs_node_t node, uint64_t offset, const void *buf, size_t nbyte)
{
	if(!node)
		return -1;

	if(node->write) {
		return node->write(node, offset, buf, nbyte);
	}
	return -1;
}

vfs_node_t vfs_finddir(vfs_node_t node, const char *name)
{
	if(!node)
		return NULL;

	if((node->flags & FILE_TYPE_DIRECTORY) && node->finddir) {
		return node->finddir(node, name);
	}
	return NULL;
}

vfs_node_t kopen(const char *filename, uint32_t flags)
{
	vfs_node_t node;

	node = find_closest_mount(filename);
	if(!node) {
		debug_warning("Cannot find a mount point near the file location\n");
		return NULL;
	}
	
	if(!(node->finddir)) {
		debug_warning("Node missing finddir function pointer");
		return NULL;
	}

	node = node->finddir(node, filename + strlen(node->name));

	if(node) {
		vfs_open(node, flags);
	}
	return node;
}

static vfs_node_t find_closest_mount(const char *path)
{
	struct tree_node *node;
	struct vfs_entry *entry;
	char *path_segment, *ptr;
	size_t path_length, segment_length;
	bool found;

	// Split path
	path_segment = ptr = strdup(path);
	path_length  = strlen(path);
	for(size_t i = 0; i < path_length; i++) {
		if(path_segment[i] == '/') path_segment[i] = '\0';
	}
	// Get past leading '/'
	path_segment++;

	node  = vfs_tree->root;
	if(!node)
		return NULL;

	node = node->child;
	entry = NULL;
	while(path_length-- && node) {
		found = false;

		// Itterate through tree checking each name with the current segment
		itterate_tree(sibling, node) {
			entry = (struct vfs_entry*)sibling->data;
			if(entry && !strcmp(entry->name, path_segment)) {
				node  = sibling->child;
				found = true;
				break;
			}
		}

		if(!found)
			break;

		segment_length = strlen(path_segment);
		path_segment   += segment_length + 1;
		path_length    -= segment_length;
	}

	kfree(ptr);

	if(entry)
		return entry->file;
	return NULL;
}

// TODO: Mount to root node
static void mount_root_node(vfs_node_t local_root)
{
	struct vfs_entry *entry;
	
	entry = vfs_tree->root->data;
	entry->file = local_root;
}

static void mount_implementation(const char *path, vfs_node_t local_root)
{
	struct tree_node *node;
	struct vfs_entry *entry;
	char *path_segment, *ptr;
	size_t path_length, segment_length;
	bool found;

	// Split path
	path_segment = ptr = strdup(path);
	path_length  = strlen(path);
	for(size_t i = 0; i < path_length; i++) {
		if(path_segment[i] == '/') path_segment[i] = '\0';
	}
	// Get past leading '/'
	path_segment++;

	node = vfs_tree->root;
	while(path_length-- && node) {
		found = false;

		// Itterate through tree checking each name with the current segment
		itterate_tree(sibling, node->child) {
			entry = (struct vfs_entry*)sibling->data;
			
			if(entry && !strcmp(entry->name, path_segment)) {
				node  = sibling;
				found = true;
				break;
			}
		}

		// Create node if not found
		if(!found) {
			entry = kmalloc(sizeof(struct vfs_entry));
			entry->name    = strdup(path_segment);
			entry->file    = NULL;
			entry->device  = NULL;
			entry->fs_type = NULL;
			
			if(node->child) {
				node = tree_append_sibling(node->child, entry);
			} else {
				node->child = tree_node_create(entry);
				node = node->child;
			}
		}

		segment_length = strlen(path_segment);
		path_segment   += segment_length + 1;
		path_length    -= segment_length;
	}

	if(entry) {
		entry->file = local_root;
	}

	kfree(ptr);
}

#ifdef VFS_DEBUG_MOUNTS
static void display_mounts(struct tree_node *node)
{
	struct vfs_entry *entry;

	if(!node) return;

	itterate_tree(sibling, node) {
		entry = (struct vfs_entry*)sibling->data;
		if(entry) {
			debug("MOUNT: %s\n", entry->name);

			if(sibling->child) {
				debug("DOWN\n");
				display_mounts(sibling->child);
				debug("UP\n");
			}
		}
	}
}
void vfs_display_mount_tree()
{
	display_mounts(vfs_tree->root);
}
#endif