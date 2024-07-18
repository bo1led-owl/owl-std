#include "owlstd/queue.h"

#include <string.h>

void owl_queue_push(owl_queue_t* q, const size_t size, const void* elem) {
    owl_vector_push(&q->left, size, elem);
}

static void pop_from_vec(owl_vector_t* v, const size_t size, void* result) {
    memcpy(result, (char*)v->items + (v->size - 1) * size, size);
    owl_vector_pop(v, 1);
}

int owl_queue_pop(owl_queue_t* q, const size_t size, void* result) {
    if (q->right.size > 0) {
        pop_from_vec(&q->right, size, result);
        return 1;
    }

    if (q->left.size == 0) {
        return 0;
    }

    owl_vector_reserve(&q->right, size, q->right.size + q->left.size);
    while (q->left.size > 0) {
        pop_from_vec(&q->left, size,
                     (char*)q->right.items + q->right.size * size);
        q->right.size += 1;
    }

    pop_from_vec(&q->right, size, result);
    return 1;
}

void owl_queue_free(owl_queue_t* q) {
    owl_vector_free(&q->left);
    owl_vector_free(&q->right);
    *q = (owl_queue_t){};
}
