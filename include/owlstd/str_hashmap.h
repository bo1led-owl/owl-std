#pragma once

#include <stdint.h>

#include "str.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct owl_str_hashmap {
    owl_str_const_t* keys;
    owl_str_t* values;
    uint8_t* metadata;  // bitset

    size_t bucket_count;
    size_t size;
} owl_str_hashmap_t;

/*
Remove all elements from the hashmap
*/
void owl_str_hashmap_clear(owl_str_hashmap_t* map);

/*
Free memory allocated to the hashmap, setting all its fields to zero
*/
void owl_str_hashmap_free(owl_str_hashmap_t* map);

/*
Insert a new key-value pair into the hashmap. Returns a pointer to the inserted
value or an existing value if the key was present
*/
owl_str_t* owl_str_hashmap_insert(owl_str_hashmap_t* map,
                                  const owl_str_const_t key,
                                  const owl_str_t value);

/*
Insert a key-value pair into the hashmap, or replace the value if the key was
already present. Returns a pointer to the inserted or replaced value
*/
owl_str_t* owl_str_hashmap_insert_or_replace(owl_str_hashmap_t* map,
                                             const owl_str_const_t key,
                                             const owl_str_t value);

/*
Get a pointer to the value by key. Returns NULL if the key is not
present
*/
owl_str_t* owl_str_hashmap_get(const owl_str_hashmap_t* map,
                               const owl_str_const_t key);

/*
Remove the key-value pair from the map
*/
void owl_str_hashmap_erase(owl_str_hashmap_t* map, const owl_str_const_t key);

#ifdef __cplusplus
}
#endif
