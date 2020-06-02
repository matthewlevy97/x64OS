#pragma once

#include <stddef.h>

#define foreach(list, type, curr) for(type * curr = (list); curr != NULL; curr = curr->next)
#define foreach_prev(list, type, curr, prev) \
	for(type * curr = (list), * prev = NULL; curr != NULL; prev = curr, curr = curr->next)

struct linked_list_entry {
	struct linked_list_entry *next, *prev;
	
	size_t key;
	void *value;
};

struct __linked_list_metadata {
	size_t length;
	
	struct linked_list_entry *head;
};
typedef struct __linked_list_metadata * List;

List linked_list_create();
void linked_list_destroy(List *list);

List linked_list_rotate(List list);

size_t linked_list_length(List list);

List linked_list_insert(List list, size_t key, void *value);

List linked_list_remove_by_key(List list, size_t key);
List linked_list_remove_by_index(List list, size_t index);

void *linked_list_get_by_key(List list, size_t key);
void *linked_list_get_by_index(List list, size_t index);