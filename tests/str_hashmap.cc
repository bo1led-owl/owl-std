#include "owlstd/str_hashmap.h"

#include <gtest/gtest.h>

TEST(StrHashMap, Basic) {
    constexpr size_t SIZE = 4;

    owl_str_hashmap_t map = (owl_str_hashmap_t){};
    EXPECT_EQ(0, map.size);

    owl_str_const_t keys[SIZE] = {
        owl_str_from_const_cstr("0"),
        owl_str_from_const_cstr("1"),
        owl_str_from_const_cstr("2"),
        owl_str_from_const_cstr("3"),
    };

    owl_str_t values[SIZE] = {
        owl_str_from_cstr(const_cast<char*>("0")),
        owl_str_from_cstr(const_cast<char*>("1")),
        owl_str_from_cstr(const_cast<char*>("2")),
        owl_str_from_cstr(const_cast<char*>("3")),
    };

    for (size_t i = 0; i < SIZE; ++i) {
        owl_str_hashmap_insert(&map, keys[i], values[i]);
        EXPECT_EQ(i + 1, map.size);
    }

    size_t old_bucket_count = map.bucket_count;

    // check that no extra values are inserted
    for (size_t i = 0; i < SIZE; ++i) {
        owl_str_hashmap_insert(&map, keys[i], values[i]);
        EXPECT_EQ(SIZE, map.size);
    }
    EXPECT_EQ(old_bucket_count, map.bucket_count);

    for (size_t i = 0; i < SIZE; ++i) {
        owl_str_const_t expected = owl_str_const_from_mut(values[i]);
        owl_str_const_t actual =
            owl_str_const_from_mut(*owl_str_hashmap_get(&map, keys[i]));
        EXPECT_TRUE(owl_str_compare(expected, actual) == 0);
    }

    old_bucket_count = map.bucket_count;
    owl_str_hashmap_clear(&map);
    EXPECT_EQ(old_bucket_count, map.bucket_count);
    EXPECT_EQ(0, map.size);

    owl_str_hashmap_free(&map);
    EXPECT_EQ(0, map.bucket_count);
    EXPECT_EQ(NULL, map.keys);
    EXPECT_EQ(NULL, map.values);
    EXPECT_EQ(NULL, map.metadata);
    EXPECT_EQ(0, map.size);
}

TEST(StrHashMap, InsertOrReplace) {
    owl_str_hashmap_t map = (owl_str_hashmap_t){};
    EXPECT_EQ(0, map.size);

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

    for (size_t i = 0; i < 2; ++i) {
        owl_str_hashmap_insert_or_replace(&map, keys[i], values[i]);
        EXPECT_EQ(i + 1, map.size);
    }

    for (size_t i = 0; i < 2; ++i) {
        owl_str_const_t expected = owl_str_const_from_mut(values[i]);
        owl_str_const_t actual =
            owl_str_const_from_mut(*owl_str_hashmap_get(&map, keys[i]));
        EXPECT_TRUE(owl_str_compare(expected, actual) == 0);
    }

    for (size_t i = 2; i < 4; ++i) {
        owl_str_hashmap_insert_or_replace(&map, keys[i], values[i]);
        EXPECT_EQ(2, map.size);
    }

    for (size_t i = 2; i < 4; ++i) {
        owl_str_const_t expected = owl_str_const_from_mut(values[i]);
        owl_str_const_t actual =
            owl_str_const_from_mut(*owl_str_hashmap_get(&map, keys[i]));
        EXPECT_TRUE(owl_str_compare(expected, actual) == 0);
    }

    owl_str_hashmap_free(&map);
}
