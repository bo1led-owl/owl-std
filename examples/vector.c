#include "owlstd/vector.h"

#include <stdio.h>

enum {
    SIZE = 16,
};

int main() {
    owl_vector_t vec = (owl_vector_t){};

    owl_vector_reserve(&vec, sizeof(int), SIZE);

    for (int i = 0; i < SIZE; ++i) {
        owl_vector_push(&vec, sizeof(int), &i);
    }

    printf("{ ");
    for (int i = 0; i < SIZE; ++i) {
        printf("%d", ((int*)vec.items)[i]);

        if (i < SIZE - 1) {
            printf(", ");
        }
    }
    printf(" }\n");
}
