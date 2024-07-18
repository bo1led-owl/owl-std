#include "owlstd/vector.h"

#include <gtest/gtest.h>

TEST(Vector, Basic) {
    constexpr int SIZE = 32;

    owl_vector_t v_int = (owl_vector_t){};
    ASSERT_EQ(0, v_int.capacity);
    ASSERT_EQ(0, v_int.size);
    ASSERT_EQ(NULL, v_int.items);

    for (int x = 0; x < SIZE; ++x) {
        owl_vector_push(&v_int, sizeof(int), &x);
    }
    EXPECT_EQ(SIZE, v_int.size);

    for (int i = 0; i < SIZE; ++i) {
        EXPECT_EQ(i, ((int*)v_int.items)[i]);
    }

    owl_vector_free(&v_int);
    EXPECT_EQ(0, v_int.capacity);
    EXPECT_EQ(0, v_int.size);
    EXPECT_EQ(NULL, v_int.items);
}

TEST(Vector, Reserve) {
    constexpr int SIZE = 32;

    owl_vector_t v_int = (owl_vector_t){};
    owl_vector_reserve(&v_int, sizeof(int), SIZE);
    EXPECT_GE(v_int.capacity, SIZE);
    const void* old_ptr = v_int.items;

    for (int x = 0; x < SIZE; ++x) {
        owl_vector_push(&v_int, sizeof(int), &x);
    }
    EXPECT_EQ(SIZE, v_int.size);
    EXPECT_EQ(old_ptr, v_int.items);

    owl_vector_free(&v_int);
}

TEST(Vector, Pop) {
    constexpr int SIZE = 32;

    owl_vector_t v_int = (owl_vector_t){};
    ASSERT_EQ(0, v_int.capacity);
    ASSERT_EQ(0, v_int.size);
    ASSERT_EQ(NULL, v_int.items);

    for (int x = 0; x < SIZE; ++x) {
        owl_vector_push(&v_int, sizeof(int), &x);
    }
    EXPECT_EQ(SIZE, v_int.size);

    for (int i = 0; i < SIZE; ++i) {
        EXPECT_EQ(i, ((int*)v_int.items)[i]);
    }

    constexpr int ITEMS_TO_POP = 8;
    owl_vector_pop(&v_int, ITEMS_TO_POP);
    EXPECT_EQ(SIZE - ITEMS_TO_POP, v_int.size);

    for (int i = 0; i < SIZE - ITEMS_TO_POP; ++i) {
        EXPECT_EQ(i, ((int*)v_int.items)[i]);
    }

    owl_vector_free(&v_int);
}
