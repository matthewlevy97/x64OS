#include <fs/vfs_tree.h>
#include <mm/kmalloc.h>

static void kill_children(struct tree_node **node);

tree_t tree_create()
{
	tree_t tree;

	tree = kmalloc(sizeof(struct __tree_trunk));
	tree->root = NULL;

	return tree;
}

void tree_destroy(tree_t *tree)
{
	tree_t tmp;

	tmp   = *tree;
	*tree = NULL;

	kill_children(&(tmp->root));
	kfree(tmp);
}

struct tree_node *tree_node_create(void *data)
{
	struct tree_node *node;

	node = kmalloc(sizeof(struct tree_node));
	node->next  = NULL;
	node->child = NULL;
	node->data  = data;

	return node;
}

struct tree_node *tree_append_sibling(struct tree_node *node, void *data)
{
	struct tree_node *tmp;

	if(node == NULL)
		return NULL;

	while(node) {
		tmp  = node;
		node = node->next;
	}

	tmp->next = tree_node_create(data);

	return tmp->next;
}

static void kill_children(struct tree_node **node)
{
	struct tree_node *tmp;

	if(!node || !(*node))
		return;

	tmp = *node;
	*node = NULL;

	kill_children(&(tmp->child));
	kill_children(&(tmp->next));

	kfree(tmp);
}