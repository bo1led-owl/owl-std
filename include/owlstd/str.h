#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
String slice of mutable data
Is not guaranteed to be null-terminated
*/
typedef struct owl_str {
    char* data;
    size_t len;
} owl_str_t;

/*
String slice of immutable data
*/
typedef struct owl_str_const {
    char const* data;
    size_t len;
} owl_str_const_t;

/*
Create an `owl_str` object from a c-style string
*/
owl_str_t owl_str_from_cstr(char* cstr);

/*
Create an `owl_str_const` object from a c-style string
*/
owl_str_const_t owl_str_from_const_cstr(const char* cstr);

/*
Create an `owl_str_const` object from an `owl-str` object
*/
owl_str_const_t owl_str_const_from_mut(owl_str_t s);

/*
Lexicographically compare two strings. Returns -1 if `a` < `b`, 0
if `a` == `b` and 1 if `a` > `b`
*/
int owl_str_compare(const owl_str_const_t a, const owl_str_const_t b);

/*
In-place put all of the characters of the string to lower case
*/
void owl_str_to_lower(owl_str_t s);

/*
In-place put all of the characters of the string to upper case
*/
void owl_str_to_upper(owl_str_t s);

/*
Get the slice of given string from its first to its last non-whitespace
character
*/
owl_str_const_t owl_str_trim(const owl_str_const_t s);

/*
Calculate the hash of given string
*/
size_t owl_str_hash(const owl_str_const_t s);

/*
Print given string to the default stdio
*/
void owl_str_print(const owl_str_const_t s);

#ifdef __cplusplus
}
#endif
