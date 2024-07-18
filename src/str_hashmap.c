#include "owlstd/str_hashmap.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LOAD_FACTOR 0.75
enum {
    MIN_SIZE = 16,
};

enum {
    META_EMPTY = 0,
    META_PRESENT = 1,
};

void owl_str_hashmap_clear(owl_str_hashmap_t* map) {
    if (!map) {
        return;
    }

    for (size_t i = 0; i < map->bucket_count; ++i) {
        map->metadata[i] = META_EMPTY;
    }

    map->size = 0;
}

void owl_str_hashmap_free(owl_str_hashmap_t* map) {
    if (!map) {
        return;
    }

    free(map->keys);
    free(map->values);
    free(map->metadata);
    *map = (owl_str_hashmap_t){};
}

static size_t probe(const size_t i, const size_t bucket_count) {
    // TODO: another kind of probing?
    return (i + 1) % bucket_count;
}

static owl_str_t* insert(owl_str_const_t* keys, owl_str_t* values,
                         char* metadata, const size_t bucket_count,
                         const owl_str_const_t* key, const owl_str_t* value) {
    const size_t hash = owl_str_hash(*key) % bucket_count;

    for (size_t i = hash;; i = probe(i, bucket_count)) {
        if (metadata[i] == META_EMPTY) {
            metadata[i] = META_PRESENT;
            memcpy(keys + i, key, sizeof(owl_str_const_t));
            memcpy(values + i, value, sizeof(owl_str_t));
            return NULL;
        }

        if (owl_str_compare(keys[i], *key) == 0) {
            return &values[i];
        }
    }
}

static double load_factor(const owl_str_hashmap_t* map) {
    if (map->bucket_count == 0) {
        return INFINITY;
    }

    return (double)map->size / (double)map->bucket_count;
}

static void resize_if_needed(owl_str_hashmap_t* map) {
    if (load_factor(map) < MAX_LOAD_FACTOR) {
        return;
    }

    if (map->bucket_count == 0) {
        *map = (owl_str_hashmap_t){
            .keys = calloc(MIN_SIZE, sizeof(owl_str_const_t)),
            .values = calloc(MIN_SIZE, sizeof(owl_str_t)),
            .metadata = calloc(MIN_SIZE, sizeof(char)),
            .bucket_count = MIN_SIZE,
            .size = map->size,
        };
        return;
    }

    const size_t new_bucket_count = map->bucket_count * 2;
    owl_str_const_t* new_keys =
        malloc(new_bucket_count * sizeof(owl_str_const_t));
    owl_str_t* new_values = malloc(new_bucket_count * sizeof(owl_str_t));
    char* new_metadata = calloc(new_bucket_count, sizeof(char));

    // rehashing
    for (size_t i = 0, moved_elements = 0;
         i < map->bucket_count && moved_elements < map->size; ++i) {
        if (map->metadata[i] == META_EMPTY) {
            continue;
        }

        insert(new_keys, new_values, new_metadata, new_bucket_count,
               &map->keys[i], &map->values[i]);

        moved_elements += 1;
    }

    free(map->keys);
    free(map->values);
    free(map->metadata);
    *map = (owl_str_hashmap_t){
        .keys = new_keys,
        .values = new_values,
        .metadata = new_metadata,
        .bucket_count = new_bucket_count,
        .size = map->size,
    };
}

int owl_str_hashmap_insert(owl_str_hashmap_t* map, const owl_str_const_t key,
                           const owl_str_t value) {
    resize_if_needed(map);

    owl_str_t* result = insert(map->keys, map->values, map->metadata,
                               map->bucket_count, &key, &value);

    if (!result) {
        map->size += 1;
    }

    // evaluates to `true` if the value was present
    return result != NULL;
}

void owl_str_hashmap_insert_or_replace(owl_str_hashmap_t* map,
                                       const owl_str_const_t key,
                                       const owl_str_t value) {
    resize_if_needed(map);

    owl_str_t* result = insert(map->keys, map->values, map->metadata,
                               map->bucket_count, &key, &value);

    if (result) {
        *result = value;
    } else {
        map->size += 1;
    }
}

owl_str_t* owl_str_hashmap_get(const owl_str_hashmap_t* map,
                               const owl_str_const_t key) {
    if (map->bucket_count == 0) {
        return NULL;
    }

    const size_t hash = owl_str_hash(key) % map->bucket_count;

    for (size_t i = hash;; i = probe(i, map->bucket_count)) {
        if (map->metadata[i] == META_EMPTY) {
            return NULL;
        }

        if (owl_str_compare(map->keys[i], key) == 0) {
            return &map->values[i];
        }
    }
}
