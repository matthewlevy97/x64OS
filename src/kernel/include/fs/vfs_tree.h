#pragma once

#define itterate_tree(i, node) for(struct tree_node *i = (node); i != NULL; i = i->next)

struct tree_node {
	struct tree_node *next;
	struct tree_node *child;
	
	void *data;
};

typedef struct __tree_trunk {
	struct tree_node *root;
} *tree_t;

tree_t tree_create();
void tree_destroy(tree_t *tree);

struct tree_node *tree_node_create(void *data);
struct tree_node *tree_append_sibling(struct tree_node *node, void *data);
