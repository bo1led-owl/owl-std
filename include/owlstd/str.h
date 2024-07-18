#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
String slice
Is not guaranteed to be null-terminated
*/
typedef struct owl_str {
    char* data;
    size_t len;
} owl_str_t;

/*
String slice pointing to non mutable data
*/
typedef struct owl_str_const {
    char const* data;
    size_t len;
} owl_str_const_t;

/*
Create a `owl_str` object from a c-style string
*/
owl_str_t owl_str_from_cstr(char* cstr);

/*
Create a `owl_str_const` object from a c-style string
*/
owl_str_const_t owl_str_from_const_cstr(const char* cstr);

/*
Create a `owl_str_const` object from a c-style string
*/
owl_str_const_t owl_str_const_from_mut(owl_str_t s);

/*
Lexicographically compare two strings. Returns -1 if `a` < `b`, 0
if `a` == `b` and 1 if `a` > `b`
*/
int owl_str_compare(const owl_str_const_t a, const owl_str_const_t b);

/*
Put all of the characters of the string to lower case in place
*/
void owl_str_to_lower(owl_str_t s);

/*
Put all of the characters of the string to upper case in place
*/
void owl_str_to_upper(owl_str_t s);

/*
Remove whitespace from both ends of the given string and return the slice to
trimmed string inside the original one
*/
owl_str_const_t owl_str_trim(const owl_str_const_t s);

/*
Hash given string
*/
size_t owl_str_hash(const owl_str_const_t s);

/*
Print given string to the default stdio
*/
void owl_str_print(const owl_str_const_t s);

#ifdef __cplusplus
}
#endif
