#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "parse.h"


struct array
{
    size_t capacity;
    size_t length;
    uint64_t values[];
};


static int init(struct array **v, size_t capacity)
{
    if (capacity == 0) {
        return EINVAL;
    }

    struct array *arr = malloc(sizeof(struct array) + sizeof(uint64_t) * capacity);
    if (arr == NULL) {
        return ENOMEM;
    }

    arr->capacity = capacity;
    arr->length = 0;
    *v = arr;

    return 0;
}


static int expand(struct array **v)
{
    size_t newcap = (*v)->capacity << 1;
    struct array *newarr = realloc(*v, sizeof(struct array) + sizeof(uint64_t) * newcap);
    if (newarr == NULL) {
        return ENOMEM;
    }

    newarr->capacity = newcap;
    *v = newarr;

    return 0;
}


static int push(struct array **v, uint64_t x)
{
    if ((*v)->length == (*v)->capacity) {
        int rc = expand(v);
        if (rc != 0) {
            return rc;
        }
    }

    (*v)->values[(*v)->length++] = x;

    return 0;
}


int main(int argc, char **argv)
{
    uint64_t N = parse_N(argc, argv);  // "Up until N" and not "N primes" as the original algorithm
    struct array *primes = NULL;  // P
    struct array *multiples = NULL;  // V

    if (N < 2) {
        exit(0);
    }

    if (init(&primes, 1000) != 0) {
        fprintf(stderr, "Unexpectedly ran out of memory\n");
        exit(3);
    }

    if (init(&multiples, 100) != 0) {
        free(primes);
        fprintf(stderr, "Unexpectedly ran out of memory\n");
        exit(3);
    }

    if (push(&primes, 2) != 0) {
        free(primes);
        free(multiples);
        fprintf(stderr, "Unexpectedly ran out of memory\n");
        exit(3);
    }
    fprintf(stdout, "%llu\n", 2ULL);

    uint64_t square = 4;

    for (uint64_t x = 3; x <= N; x += 2) {

        if (square <= x) {
            uint64_t next_prime = primes->values[multiples->length + 1];

            if (push(&multiples, square) != 0) {
                fprintf(stderr, "Unexpectedly ran out of memory\n");
                free(primes);
                free(multiples);
                exit(3);
            }
            square = next_prime * next_prime;
        }

        for (size_t k = 0; k < multiples->length; ++k) {
            if (multiples->values[k] < x) {
                multiples->values[k] += primes->values[k];
            }

            if (multiples->values[k] == x) {
                goto not_prime;  // The irony of using goto in a Dijkstra algorithm is not lost on me :)
            }
        }

        fprintf(stdout, "%llu\n", (unsigned long long) x);
        if (push(&primes, x) != 0) {
            fprintf(stderr, "Unexpectedly ran out of memory\n");
            free(primes);
            free(multiples);
            exit(3);
        }

not_prime:
        (void) 0;
    }
    
    free(primes);
    free(multiples);
    exit(0);
}
