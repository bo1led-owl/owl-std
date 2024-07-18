#pragma once

#include <stddef.h>

/*
Dynamic array of any type.
Does not contain information about element size, make sure that `elem_size`
argument is always the same for each vector.
*/
typedef struct owl_vector {
    void* items;
    size_t size;
    size_t capacity;
} owl_vector_t;

/*
Copy element of size `elem_size` from `elem` into vector
*/
void owl_vector_push(owl_vector_t* vec, const size_t elem_size,
                     const void* elem);

/*
Reserve enough space for `n` elements of size `elem_size`
*/
void owl_vector_reserve(owl_vector_t* vec, const size_t elem_size,
                        const size_t n);

/*
Remove n elements from the end of the vector
*/
void owl_vector_pop(owl_vector_t* vec, const size_t n);

/*
Free the buffer and set all the fields to zero
*/
void owl_vector_free(owl_vector_t* vec);
