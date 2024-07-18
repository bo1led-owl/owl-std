#include "owlstd/str_hashmap.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LOAD_FACTOR 0.75
enum {
    MIN_SIZE = 16,
};

typedef struct str_pair str_pair_t;

void owl_str_hashmap_clear(str_hashmap_t* map) {
    if (!map)
        return;

    for (size_t i = 0; i < map->bucket_count; ++i) {
        map->metadata[i] = 0;
    }

    map->size = 0;
}

void owl_str_hashmap_free(str_hashmap_t* map) {
    if (!map)
        return;

    free(map->data);
    *map = (str_hashmap_t){};
}

static size_t probe(const size_t i, const size_t bucket_count) {
    // TODO: another kind of probing?
    return (i + 1) % bucket_count;
}

static owl_str_t* insert(str_pair_t* data, char* metadata,
                     const size_t bucket_count, const str_pair_t* item) {
    const size_t hash = owl_str_hash(item->key) % bucket_count;

    for (size_t i = hash;; i = probe(i, bucket_count)) {
        if (!metadata[i]) {
            metadata[i] = 1;
            memcpy(data + i, item, sizeof(str_pair_t));
            return NULL;
        }

        if (owl_str_compare(data[i].key, item->key) == 0) {
            return &data[i].value;
        }
    }
}

static double load_factor(const str_hashmap_t* map) {
    if (map->bucket_count == 0) {
        return INFINITY;
    }

    return (double)map->size / (double)map->bucket_count;
}

static void resize_if_needed(str_hashmap_t* map) {
    if (load_factor(map) < MAX_LOAD_FACTOR) {
        return;
    }

    if (map->bucket_count == 0) {
        *map = (str_hashmap_t){.data = calloc(MIN_SIZE, sizeof(str_pair_t)),
                               .metadata = calloc(MIN_SIZE, sizeof(char)),
                               .bucket_count = MIN_SIZE,
                               .size = map->size};
        return;
    }

    const size_t new_bucket_count = map->bucket_count * 2;
    str_pair_t* new_data = malloc(new_bucket_count * sizeof(str_pair_t));
    char* new_metadata = calloc(new_bucket_count, sizeof(char));

    // rehashing
    for (size_t i = 0, moved_elements = 0;
         i < map->bucket_count && moved_elements < map->size; ++i) {
        if (!map->metadata[i])
            continue;

        insert(new_data, new_metadata, new_bucket_count, &map->data[i]);

        moved_elements += 1;
    }

    free(map->data);
    free(map->metadata);
    *map = (str_hashmap_t){.data = new_data,
                           .metadata = new_metadata,
                           .bucket_count = new_bucket_count,
                           .size = map->size};
}

int owl_str_hashmap_insert(str_hashmap_t* map, const owl_str_t key,
                        const owl_str_t value) {
    resize_if_needed(map);

    const str_pair_t entry = {.key = key, .value = value};
    owl_str_t* result = insert((str_pair_t*)map->data, map->metadata, map->bucket_count, &entry);

    if (!result)
        map->size += 1;

    // evaluates to `true` if the value was present
    return result != NULL;
}

void owl_str_hashmap_insert_or_replace(str_hashmap_t* map, const owl_str_t key,
                                   const owl_str_t value) {
    resize_if_needed(map);

    const str_pair_t entry = {.key = key, .value = value};
    owl_str_t* result = insert((str_pair_t*)map->data, map->metadata, map->bucket_count, &entry);

    if (result) {
        *result = value;
    }
}

owl_str_t const* owl_str_hashmap_get(const str_hashmap_t* map, const owl_str_t key) {
    if (map->bucket_count == 0)
        return NULL;

    const size_t hash = owl_str_hash(key) % map->bucket_count;

    for (size_t i = hash;; i = probe(i, map->bucket_count)) {
        if (!map->metadata[i]) {
            return NULL;
        }

        if (owl_str_compare(map->data[i].key, key) == 0) {
            return &map->data[i].value;
        }
    }
}
