#include <stdlib.h>
#include <utils/linked_list.h>

static void remove_entry(List list, struct linked_list_entry *entry);

List linked_list_create()
{
	List list;

	list = malloc(sizeof(struct __linked_list_metadata));
	list->length = 0;
	list->head   = NULL;

	return list;
}

void linked_list_destroy(List *list)
{
	while((*list)->head)
		remove_entry(*list, (*list)->head);

	free(*list);
	*list = NULL;
}

List linked_list_rotate(List list)
{
	if(!list) return NULL;
	if(!(list->head)) return list;

	if(list->length > 1)
		list->head = list->head->next;

	return list;
}

size_t linked_list_length(List list)
{
	return (list ? list->length : 0);
}

List linked_list_insert(List list, size_t key, void *value)
{
	struct linked_list_entry *entry;

	if(!list)
		list = linked_list_create(); 

	entry = malloc(sizeof(struct linked_list_entry));
	entry->next  = entry;
	entry->prev  = entry;
	entry->key   = key;
	entry->value = value;

	if(list->head) {
		entry->next = list->head;
		entry->prev = list->head->prev;

		list->head->prev->next = entry;
		list->head->prev = entry;
	} else {
		list->head = entry;
	}

	list->length++;

	return list;
}

List linked_list_remove_by_key(List list, size_t key)
{
	struct linked_list_entry *entry;

	if(!list)
		return NULL;

	entry = list->head;
	while(entry && entry->key != key) {
		entry = entry->next;
	}

	if(entry)
		remove_entry(list, entry);

	return list;
}

List linked_list_remove_by_index(List list, size_t index)
{
	struct linked_list_entry *entry;

	if(!list)
		return NULL;

	entry = list->head;
	while(entry && index--) {
		entry = entry->next;
	}

	if(entry)
		remove_entry(list, entry);

	return list;
}

void *linked_list_get_by_key(List list, size_t key)
{
	struct linked_list_entry *entry;

	if(!list)
		return NULL;

	entry = list->head;
	while(entry && entry->key != key) {
		entry = entry->next;
	}

	return (entry ? entry->value : NULL);
}

void *linked_list_get_by_index(List list, size_t index)
{
	struct linked_list_entry *entry;

	if(!list)
		return NULL;

	entry = list->head;
	while(index-- && entry) {
		entry = entry->next;
	}

	if(entry)
		return entry->value;

	return NULL;
}

static inline void remove_entry(List list, struct linked_list_entry *entry)
{
	entry->next->prev = entry->prev;
	entry->prev->next = entry->next;

	if(list->head == entry)
		list->head = entry->next;

	free(entry);
	list->length--;

	if(!(list->length))
		list->head = NULL;
}