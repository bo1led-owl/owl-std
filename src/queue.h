#pragma once

#include <stddef.h>

#include "vector.h"

typedef struct owl_queue {
    owl_vector_t left;
    owl_vector_t right;
} owl_queue_t;

/*
Enqueue an element of size `size` behind pointer `elem`
*/
void owl_queue_push(owl_queue_t* q, const size_t size, const void* elem);

/*
Dequeue an element of size `size` into `result`. Returns whether the element was
written
*/
int owl_queue_pop(owl_queue_t* q, const size_t size, void* result);

/*
Free memory allocated by a queue
*/
void owl_queue_free(owl_queue_t* q);
