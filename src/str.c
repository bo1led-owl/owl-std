#include "owlstd/str.h"

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#define min(a, b) (((a) < (b)) ? (a) : (b))

owl_str_t owl_str_from_cstr(char* cstr) {
    return (owl_str_t){.data = cstr, .len = strlen(cstr)};
}

owl_str_const_t owl_str_from_const_cstr(const char* cstr) {
    return (owl_str_const_t){.data = cstr, .len = strlen(cstr)};
}

owl_str_const_t owl_str_const_from_mut(const owl_str_t s) {
    return (owl_str_const_t){.data = s.data, .len = s.len};
}

int owl_str_compare(const owl_str_const_t a, const owl_str_const_t b) {
    const size_t len = min(a.len, b.len);
    for (size_t i = 0; i < len; ++i) {
        if (a.data[i] > b.data[i]) {
            return 1;
        } else if (a.data[i] < b.data[i]) {
            return -1;
        }
    }

    if (a.len < b.len) {
        return -1;
    } else if (a.len > b.len) {
        return 1;
    } else {
        return 0;
    }
}

void owl_str_to_lower(owl_str_t s) {
    for (size_t i = 0; i < s.len; ++i) {
        s.data[i] = tolower(s.data[i]);
    }
}

void owl_str_to_upper(owl_str_t s) {
    for (size_t i = 0; i < s.len; ++i) {
        s.data[i] = toupper(s.data[i]);
    }
}

owl_str_const_t owl_str_trim(const owl_str_const_t s) {
    if (!s.data) {
        return (owl_str_const_t){};
    }

    owl_str_const_t res = s;
    for (size_t i = 0; i < s.len; ++i) {
        if (!isspace(s.data[i])) {
            break;
        }

        res.data += 1;
        res.len -= 1;
    }

    for (ssize_t i = res.len - 1; i >= 0; --i) {
        if (!isspace(res.data[i])) {
            break;
        }
        res.len -= 1;
    }

    return res;
}

// djb2 algorithm
size_t owl_str_hash(const owl_str_const_t s) {
    size_t hash = 5381;
    for (size_t i = 0; i < s.len; ++i) {
        hash = ((hash << 5) + hash) + s.data[i];
    }

    return hash;
}

void owl_str_print(const owl_str_const_t s) {
    if (!s.data) {
        return;
    }
    fwrite(s.data, sizeof(char), s.len, stdout);
}
