#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "parse.h"


struct prime
{
    uint64_t prime;
    uint64_t multiple;
};


struct array
{
    size_t capacity;
    size_t length;
    struct prime primes[];
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
    struct array *newarr = realloc(*v, sizeof(struct array) + sizeof(struct prime) * newcap);
    if (newarr == NULL) {
        return ENOMEM;
    }

    newarr->capacity = newcap;
    *v = newarr;

    return 0;
}


static int insert_prime(struct array **v, uint64_t n)
{
    if ((*v)->length == (*v)->capacity) {
        int rc = expand(v);
        if (rc != 0) {
            return rc;
        }
    }

    size_t idx = (*v)->length++;
    (*v)->primes[idx].prime = n;
    (*v)->primes[idx].multiple = n * n;

    return 0;
}


static inline void sort(struct array *v)
{
    for (size_t i = 1; i < v->length; ++i) {
        struct prime p = v->primes[i];
        size_t j = i;

        while (j > 0 && v->primes[j - 1].multiple > p.multiple) {
            v->primes[j] = v->primes[j - 1];
            --j;
        }

        v->primes[j] = p;
    }
}


int main(int argc, char **argv)
{
    uint64_t N = parse_N(argc, argv);
    struct array *arr = NULL;

    if (N < 2) {
        exit(0);
    }

    if (init(&arr, 1024) != 0) {
        fprintf(stderr, "Unexpectedly ran out of memory\n");
        exit(3);
    }

    fprintf(stdout, "%llu\n", 2ULL);
    if (insert_prime(&arr, 2) != 0) {
        fprintf(stderr, "Unexpectedly ran out of memory\n");
        free(arr);
        exit(3);
    }

    for (uint64_t n = 3; n <= N; ++n) {

        if (n < arr->primes[0].multiple) {
            fprintf(stdout, "%llu\n", (unsigned long long) n);

            if (n * n <= N) {
                int rc = insert_prime(&arr, n);
                if (rc != 0) {
                    fprintf(stderr, "Unexpectedly ran out of memory\n");
                    free(arr);
                    exit(3);
                }
            }

        } else {

            if (n == arr->primes[0].multiple) {
                for (size_t i = 0; i < arr->length; ++i) {
                    if (arr->primes[i].multiple > n) {
                        break;
                    }
                    arr->primes[i].multiple += arr->primes[i].prime;
                }
                sort(arr);
            }
        }
    }

#ifndef NDEBUG
    free(arr);
#endif
    exit(0);
}
