#include "owlstd/vector.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

enum {
    VECTOR_MIN_CAPACITY = 8,
    SIZE_BITS = sizeof(size_t) * 8,
};

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))

static size_t round_up_to_the_power_of_two(const size_t n) {
    return (n & (1 << (SIZE_BITS - __builtin_clzll(n) - 1))) << 1;
}

void owl_vector_reserve(owl_vector_t* vec, const size_t elem_size,
                    const size_t new_capacity) {
    if (vec->capacity >= new_capacity) {
        return;
    }

    if (vec->capacity == 0) {
        vec->capacity = VECTOR_MIN_CAPACITY;
    }

    const size_t diff = new_capacity / vec->capacity;
    size_t resize_factor = 1;
    if (diff >= 1) {
        resize_factor = round_up_to_the_power_of_two(diff);
    }

    vec->capacity *= resize_factor;
    vec->items = realloc(vec->items, vec->capacity * elem_size);
}

void owl_vector_push(owl_vector_t* vec, const size_t size, const void* elem) {
    owl_vector_reserve(vec, size, vec->size + 1);
    memcpy((char*)vec->items + vec->size * size, elem, size);
    vec->size += 1;
}

void owl_vector_pop(owl_vector_t* vec, const size_t n) {
    vec->size -= min(vec->size, n);
}

void owl_vector_free(owl_vector_t* vec) {
    if (!vec) {
        return;
    }

    free(vec->items);
    *vec = (owl_vector_t){0};
}
