#include "owlstd/str.h"

#include <gtest/gtest.h>

#include <cstring>

template <typename T>
int sign(const T x) {
    if (x < 0) {
        return -1;
    } else if (x > 0) {
        return 1;
    } else {
        return 0;
    }
}

TEST(Str, FromCStr) {
    char cstr_mut[] = "hello";
    owl_str_t str_mut = owl_str_from_cstr(cstr_mut);
    EXPECT_EQ(strlen(cstr_mut), str_mut.len);

    const char cstr_const[] = "hello";
    owl_str_const_t str_const = owl_str_from_const_cstr(cstr_const);
    EXPECT_EQ(strlen(cstr_const), str_const.len);
}

TEST(Str, ConstFromMut) {
    char cstr[] = "hello";
    owl_str_t str_mut = owl_str_from_cstr(cstr);
    owl_str_const_t str_const = owl_str_to_const(str_mut);

    EXPECT_TRUE(sign(strcmp(str_mut.data, str_const.data)) == 0);
    EXPECT_EQ(str_mut.len, str_const.len);
}

TEST(Str, CompareBasic) {
    const owl_str_const_t s1 = owl_str_from_const_cstr("aa");
    const owl_str_const_t s2 = owl_str_from_const_cstr("ab");

    EXPECT_EQ(sign(strcmp(s1.data, s2.data)), owl_str_compare(s1, s2));
    EXPECT_EQ(sign(strcmp(s2.data, s1.data)), owl_str_compare(s2, s1));

    EXPECT_EQ(sign(strcmp(s1.data, s1.data)), owl_str_compare(s1, s1));
    EXPECT_EQ(sign(strcmp(s2.data, s2.data)), owl_str_compare(s2, s2));
}

TEST(Str, CompareLength) {
    const owl_str_const_t s1 = owl_str_from_const_cstr("aa");
    const owl_str_const_t s2 = owl_str_from_const_cstr("aaa");

    EXPECT_EQ(sign(strcmp(s1.data, s2.data)), owl_str_compare(s1, s2));
    EXPECT_EQ(sign(strcmp(s2.data, s1.data)), owl_str_compare(s2, s1));

    EXPECT_EQ(sign(strcmp(s1.data, s1.data)), owl_str_compare(s1, s1));
    EXPECT_EQ(sign(strcmp(s2.data, s2.data)), owl_str_compare(s2, s2));
}

TEST(Str, CompareEmpty) {
    const owl_str_const_t s = owl_str_from_const_cstr("foo");
    const owl_str_const_t s_empty = owl_str_from_const_cstr("");

    EXPECT_EQ(sign(strcmp(s_empty.data, s_empty.data)),
              owl_str_compare(s_empty, s_empty));
    EXPECT_EQ(sign(strcmp(s_empty.data, s.data)), owl_str_compare(s_empty, s));
    EXPECT_EQ(sign(strcmp(s.data, s_empty.data)), owl_str_compare(s, s_empty));
}

TEST(Str, ToLower) {
    std::string lowercase{"hello"};
    std::string uppercase{"HELLO"};
    std::string mixed{"hElLo"};

    std::string buf = lowercase;
    owl_str_to_lower((owl_str_t){.data = buf.data(), .len = buf.size()});
    EXPECT_EQ(lowercase, buf);

    buf = uppercase;
    owl_str_to_lower((owl_str_t){.data = buf.data(), .len = buf.size()});
    EXPECT_EQ(lowercase, buf);

    buf = mixed;
    owl_str_to_lower((owl_str_t){.data = buf.data(), .len = buf.size()});
    EXPECT_EQ(lowercase, buf);
}

TEST(Str, ToUpper) {
    std::string lowercase{"hello"};
    std::string uppercase{"HELLO"};
    std::string mixed{"hElLo"};

    std::string buf = lowercase;
    owl_str_to_upper((owl_str_t){.data = buf.data(), .len = buf.size()});
    EXPECT_EQ(uppercase, buf);

    buf = uppercase;
    owl_str_to_upper((owl_str_t){.data = buf.data(), .len = buf.size()});
    EXPECT_EQ(uppercase, buf);

    buf = mixed;
    owl_str_to_upper((owl_str_t){.data = buf.data(), .len = buf.size()});
    EXPECT_EQ(uppercase, buf);
}

TEST(Str, Trim) {
    const owl_str_const_t correct = owl_str_from_const_cstr("foo \n\tbar");
    owl_str_const_t s = owl_str_from_const_cstr(" \n\tfoo \n\tbar\t\n ");
    s = owl_str_trim(s);
    EXPECT_TRUE(owl_str_compare(correct, s) == 0);
}
