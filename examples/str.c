#include "owlstd/str.h"

#include <stdio.h>
#include <stdlib.h>

/// to check whether the string is a palindrome, we don't need to mutate it, so
/// `str_const_t` is a better use
void is_palindrome(owl_str_const_t s) {
    int result = 1;
    for (size_t i = 0; i < s.len / 2; ++i) {
        if (s.data[i] != s.data[s.len - i - 1]) {
            result = 0;
            break;
        }
    }

    putchar('`');
    owl_str_print(s);
    putchar('`');
    printf(" %s\n", (result ? "is a palindrome" : "is not a palindrome"));
}

void print_in_all_cases(owl_str_const_t s) {
    // copy given immutable string to a mutable buffer, so we don't change
    // user's data
    owl_str_t buf = (owl_str_t){
        .data = malloc(s.len * sizeof(char)),
        .len = s.len,
    };
    owl_str_copy(&buf, s);

    printf("original: `");
    owl_str_print(owl_str_const_from_mut(buf));
    printf("`\n");

    owl_str_to_lower(buf);
    printf("lowercase: `");
    owl_str_print(owl_str_const_from_mut(buf));
    printf("`\n");

    owl_str_to_upper(buf);
    printf("uppercase: `");
    owl_str_print(owl_str_const_from_mut(buf));
    printf("`\n");

    free(buf.data);
}

int main() {
    print_in_all_cases(owl_str_from_const_cstr("Hello world!"));
    putchar('\n');

    owl_str_const_t not_a_palindrome = owl_str_from_const_cstr("Foo");
    owl_str_const_t palindrome = owl_str_from_const_cstr("FooooF");
    is_palindrome(palindrome);
    is_palindrome(not_a_palindrome);
}
