#include "owlstd/str_hashmap.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LOAD_FACTOR 0.75
enum {
    MIN_SIZE = 16,
};

typedef enum : uint8_t {
    META_EMPTY = 0,
    META_PRESENT = 1,
} metadata_state_t;

static metadata_state_t bitset_get(const uint8_t* bitset, const size_t i) {
    assert(bitset);
    const size_t byte_index = i / 8;
    const size_t bit_index = i % 8;
    return bitset[byte_index] >> bit_index;
}

static void bitset_set(uint8_t* bitset, const size_t i,
                       const metadata_state_t value) {
    assert(bitset);
    const size_t byte_index = i / 8;
    const size_t bit_index = i % 8;
    switch (value) {
        case META_EMPTY:
            bitset[byte_index] &= ~(1 << bit_index);
            return;
        case META_PRESENT:
            bitset[byte_index] |= 1 << bit_index;
            return;
    }
}

static size_t bitset_bytes_needed(const size_t n) {
    const size_t extra_byte = (n % 8) != 0;
    return (n / 8) + extra_byte;
}

static size_t probe(const size_t i, const size_t bucket_count) {
    // TODO: another kind of probing?
    return (i + 1) % bucket_count;
}

static owl_str_t* insert(owl_str_const_t* keys, owl_str_t* values,
                         uint8_t* metadata, const size_t bucket_count,
                         const owl_str_const_t* key, const owl_str_t* value) {
    assert(keys);
    assert(values);
    assert(metadata);
    assert(key);
    assert(value);

    const size_t hash = owl_str_hash(*key) % bucket_count;

    for (size_t i = hash;; i = probe(i, bucket_count)) {
        if (bitset_get(metadata, i) == META_EMPTY) {
            bitset_set(metadata, i, META_PRESENT);
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
    assert(map);

    if (map->bucket_count == 0) {
        return INFINITY;
    }

    return (double)map->size / (double)map->bucket_count;
}

static void resize_if_needed(owl_str_hashmap_t* map) {
    assert(map);

    if (load_factor(map) < MAX_LOAD_FACTOR) {
        return;
    }

    if (map->bucket_count == 0) {
        *map = (owl_str_hashmap_t){
            .keys = calloc(MIN_SIZE, sizeof(owl_str_const_t)),
            .values = calloc(MIN_SIZE, sizeof(owl_str_t)),
            .metadata = calloc(bitset_bytes_needed(MIN_SIZE), sizeof(uint8_t)),
            .bucket_count = MIN_SIZE,
            .size = map->size,
        };
        return;
    }

    const size_t new_bucket_count = map->bucket_count * 2;
    owl_str_const_t* new_keys =
        malloc(new_bucket_count * sizeof(owl_str_const_t));
    owl_str_t* new_values = malloc(new_bucket_count * sizeof(owl_str_t));
    uint8_t* new_metadata =
        calloc(bitset_bytes_needed(new_bucket_count), sizeof(uint8_t));

    // rehashing
    for (size_t i = 0, moved_elements = 0;
         i < map->bucket_count && moved_elements < map->size; ++i) {
        if (bitset_get(map->metadata, i) == META_EMPTY) {
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

void owl_str_hashmap_clear(owl_str_hashmap_t* map) {
    assert(map);

    memset(map->metadata, 0,
           map->bucket_count / 8 + (map->bucket_count % 8 > 0));

    map->size = 0;
}

void owl_str_hashmap_free(owl_str_hashmap_t* map) {
    assert(map);

    free(map->keys);
    free(map->values);
    free(map->metadata);
    *map = (owl_str_hashmap_t){};
}

owl_str_t* owl_str_hashmap_insert(owl_str_hashmap_t* map,
                                  const owl_str_const_t key,
                                  const owl_str_t value) {
    assert(map);

    resize_if_needed(map);

    owl_str_t* result = insert(map->keys, map->values, map->metadata,
                               map->bucket_count, &key, &value);

    if (!result) {
        map->size += 1;
    }

    return result;
}

owl_str_t* owl_str_hashmap_insert_or_replace(owl_str_hashmap_t* map,
                                             const owl_str_const_t key,
                                             const owl_str_t value) {
    assert(map);

    resize_if_needed(map);

    owl_str_t* result = insert(map->keys, map->values, map->metadata,
                               map->bucket_count, &key, &value);

    if (result) {
        *result = value;
    } else {
        map->size += 1;
    }

    return result;
}

owl_str_t* owl_str_hashmap_get(const owl_str_hashmap_t* map,
                               const owl_str_const_t key) {
    assert(map);

    if (map->bucket_count == 0) {
        return NULL;
    }

    const size_t hash = owl_str_hash(key) % map->bucket_count;

    for (size_t i = hash;; i = probe(i, map->bucket_count)) {
        if (bitset_get(map->metadata, i) == META_EMPTY) {
            return NULL;
        }

        if (owl_str_compare(map->keys[i], key) == 0) {
            return &map->values[i];
        }
    }
}
