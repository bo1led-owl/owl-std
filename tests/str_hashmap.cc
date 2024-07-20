#include "owlstd/str_hashmap.h"

#include <gtest/gtest.h>

constexpr size_t SIZE = 4;
static const owl_str_const_t keys[SIZE] = {
    owl_str_from_const_cstr("0"),
    owl_str_from_const_cstr("1"),
    owl_str_from_const_cstr("2"),
    owl_str_from_const_cstr("3"),
};

static const owl_str_t values[SIZE] = {
    owl_str_from_cstr(const_cast<char*>("0")),
    owl_str_from_cstr(const_cast<char*>("1")),
    owl_str_from_cstr(const_cast<char*>("2")),
    owl_str_from_cstr(const_cast<char*>("3")),
};

TEST(StrHashMap, Basic) {
    owl_str_hashmap_t map = (owl_str_hashmap_t){};
    EXPECT_EQ(0, map.size);

    for (size_t i = 0; i < SIZE; ++i) {
        owl_str_hashmap_insert(&map, keys[i], values[i]);
        EXPECT_EQ(i + 1, map.size);
    }

    for (size_t i = 0; i < SIZE; ++i) {
        owl_str_const_t expected = owl_str_const_from_mut(values[i]);
        owl_str_const_t actual =
            owl_str_const_from_mut(*owl_str_hashmap_get(&map, keys[i]));
        EXPECT_TRUE(owl_str_compare(expected, actual) == 0);
    }

    owl_str_hashmap_free(&map);
}

TEST(StrHashMap, InsertOrReplace) {
    owl_str_hashmap_t map = (owl_str_hashmap_t){};

    owl_str_const_t keys[] = {
        owl_str_from_const_cstr("0"),
        owl_str_from_const_cstr("1"),
        owl_str_from_const_cstr("0"),
        owl_str_from_const_cstr("1"),
    };

    owl_str_t values[] = {
        owl_str_from_cstr(const_cast<char*>("0")),
        owl_str_from_cstr(const_cast<char*>("1")),
        owl_str_from_cstr(const_cast<char*>("2")),
        owl_str_from_cstr(const_cast<char*>("3")),
    };

    // insert
    for (size_t i = 0; i < 2; ++i) {
        owl_str_hashmap_insert_or_replace(&map, keys[i], values[i]);
        EXPECT_EQ(i + 1, map.size);
    }

    // check
    for (size_t i = 0; i < 2; ++i) {
        owl_str_const_t expected = owl_str_const_from_mut(values[i]);
        owl_str_const_t actual =
            owl_str_const_from_mut(*owl_str_hashmap_get(&map, keys[i]));
        EXPECT_TRUE(owl_str_compare(expected, actual) == 0);
    }

    // replace
    for (size_t i = 2; i < 4; ++i) {
        owl_str_hashmap_insert_or_replace(&map, keys[i], values[i]);
        EXPECT_EQ(2, map.size);
    }

    // check again
    for (size_t i = 2; i < 4; ++i) {
        owl_str_const_t expected = owl_str_const_from_mut(values[i]);
        owl_str_const_t actual =
            owl_str_const_from_mut(*owl_str_hashmap_get(&map, keys[i]));
        EXPECT_TRUE(owl_str_compare(expected, actual) == 0);
    }

    owl_str_hashmap_free(&map);
}

TEST(StrHashMap, ExtraInsertion) {
    owl_str_hashmap_t map = (owl_str_hashmap_t){};
    EXPECT_EQ(0, map.size);

    for (size_t i = 0; i < SIZE; ++i) {
        owl_str_hashmap_insert(&map, keys[i], values[i]);
        EXPECT_EQ(i + 1, map.size);
    }

    const size_t old_bucket_count = map.bucket_count;
    const size_t old_size = map.size;

    // check that no extra values are inserted
    for (size_t i = 0; i < SIZE; ++i) {
        owl_str_hashmap_insert(&map, keys[i], values[i]);
        EXPECT_EQ(SIZE, map.size);
    }
    EXPECT_EQ(old_bucket_count, map.bucket_count);
    EXPECT_EQ(old_size, map.size);

    owl_str_hashmap_free(&map);
}

TEST(StrHashMap, GetFromEmptyMap) {
    owl_str_hashmap_t map = {};

    EXPECT_EQ(NULL, owl_str_hashmap_get(&map, owl_str_from_const_cstr("foo")));
}

TEST(StrHashMap, Erase) {
    owl_str_hashmap_t map = (owl_str_hashmap_t){};

    for (size_t i = 0; i < SIZE; ++i) {
        owl_str_hashmap_insert(&map, keys[i], values[i]);
    }

    for (size_t i = 0; i < SIZE; ++i) {
        owl_str_hashmap_erase(&map, keys[i]);
        EXPECT_EQ(NULL, owl_str_hashmap_get(&map, keys[i]));
        EXPECT_EQ(SIZE - i - 1, map.size);
    }

    owl_str_hashmap_free(&map);
}

TEST(StrHashMap, Clear) {
    owl_str_hashmap_t map = (owl_str_hashmap_t){};

    for (size_t i = 0; i < SIZE; ++i) {
        owl_str_hashmap_insert(&map, keys[i], values[i]);
    }

    const size_t old_bucket_count = map.bucket_count;
    owl_str_hashmap_clear(&map);
    EXPECT_EQ(old_bucket_count, map.bucket_count);
    EXPECT_EQ(0, map.size);

    owl_str_hashmap_free(&map);
}

TEST(StrHashMap, Free) {
    owl_str_hashmap_t map = (owl_str_hashmap_t){};

    for (size_t i = 0; i < SIZE; ++i) {
        owl_str_hashmap_insert(&map, keys[i], values[i]);
    }

    owl_str_hashmap_free(&map);
    EXPECT_EQ(0, map.bucket_count);
    EXPECT_EQ(NULL, map.keys);
    EXPECT_EQ(NULL, map.values);
    EXPECT_EQ(NULL, map.metadata);
    EXPECT_EQ(0, map.size);
}
