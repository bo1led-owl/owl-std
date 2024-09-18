#include "owlstd/str_hashmap.h"

#include <assert.h>
#include <stdio.h>

#include "owlstd/str.h"

void print_phone_number(const owl_str_hashmap_t* phone_book, const char* name) {
    const owl_str_t* phone =
        owl_str_hashmap_get(phone_book, owl_str_from_const_cstr(name));

    if (phone) {
        printf("%s's phone number is `", name);
        owl_str_print(owl_str_to_const(*phone));
        printf("`\n");
    } else {
        printf("%s's phone number is not present in the phone book\n", name);
    }
}

int main() {
    owl_str_hashmap_t phone_book = {};

    // buffer is needed to not attempt writing into `data` section
    char alice_buf[3] = "123";

    // `insert` returns a pointer to the value behind the key
    owl_str_t* alices_number =
        owl_str_hashmap_insert(&phone_book, owl_str_from_const_cstr("Alice"),
                               owl_str_from_cstr(alice_buf));
    owl_str_hashmap_insert(&phone_book, owl_str_from_const_cstr("Bob"),
                           owl_str_from_cstr("321"));

    print_phone_number(&phone_book, "Alice");
    print_phone_number(&phone_book, "Bob");
    print_phone_number(&phone_book, "Mike");
    putchar('\n');

    // we can change the value directly:
    owl_str_copy(alices_number, owl_str_from_const_cstr("456"));

    // or replace it:
    owl_str_hashmap_insert_or_replace(
        &phone_book, owl_str_from_const_cstr("Bob"), owl_str_from_cstr("654"));

    print_phone_number(&phone_book, "Alice");
    print_phone_number(&phone_book, "Bob");

    owl_str_hashmap_free(&phone_book);
}
