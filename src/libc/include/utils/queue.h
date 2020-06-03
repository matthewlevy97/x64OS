#pragma once

#include <stddef.h>

struct queue_node {
	struct queue_node *next;
	void *data;
};

typedef struct __queue {
	struct queue_node *head, *tail;
	uint64_t size, max_size;
} * queue_t;