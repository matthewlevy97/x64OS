#include <utils/queue.h>

queue_t queue_create(uint64_t max_size)
{
	queue_t queue;

	queue = malloc(sizeof(struct __queue));
	queue->head = NULL;
	queue->tail = NULL;
	queue->size = 0;
	queue->max_size = max_size;

	return queue;
}

queue_t queue_insert(queue_t queue, void *data)
{
	struct queue_node *node;

	if(queue->size >= queue->max_size) {
		// TODO: Should this return an error???
		return queue;
	}

	node = malloc(sizeof(struct queue_node));
	node->next = queue->tail;
	node->data = data;

	queue->tail = node;
	queue->size++;

	if(!(queue->head))
		queue->head = queue->tail;

	return queue;
}

void *queue_get(queue_t queue)
{
	struct queue_node *node;
	void *ret;

	ret = NULL;
	if(queue->head) {
		node = queue->head;
		queue->head = queue->head->next;
		queue->size--;

		ret = node->data;
		free(node);
	}

	return ret;
}

uint64_t queue_size(queue_t queue)
{
	return queue->size;
}