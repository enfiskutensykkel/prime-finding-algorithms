#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "parse.h"
#include "isqrt.h"


static inline void set_bit(uint64_t *bitmap, uint64_t idx)
{
    bitmap[idx >> 6] |= (1ULL << (idx & 63));
}


static inline bool is_set(const uint64_t *bitmap, uint64_t idx)
{
    return !!(bitmap[idx >> 6] & (1ULL << (idx & 63)));
}


int main(int argc, char **argv)
{
    static uint64_t *bitmap = NULL;
    uint64_t N = parse_N(argc, argv);
    uint64_t n, N_sqrt;

    bitmap = calloc((N / 64) + 1, sizeof(uint64_t));
    if (bitmap == NULL) {
        fprintf(stderr, "Unexpectedly ran out of memory\n");
        exit(3);
    }

    for (n = 2, N_sqrt = isqrt(N); n <= N_sqrt; ++n) {
        if (!is_set(bitmap, n)) {

            fprintf(stdout, "%llu\n", (unsigned long long) n);

            // set multiples of n
            for (uint64_t i = 2 * n; i <= N; i += n) {
                set_bit(bitmap, i);
            }
        }
    }

    // print out the rest of the primes
    for (; n <= N; ++n) {
        if (!is_set(bitmap, n)) {
            fprintf(stdout, "%llu\n", (unsigned long long) n);
        }
    }

#ifndef NDEBUG
    free(bitmap);
#endif
    exit(0);
}
