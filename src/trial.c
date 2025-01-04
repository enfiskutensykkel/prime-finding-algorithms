#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "parse.h"
#include "isqrt.h"


int main(int argc, char **argv)
{
    uint64_t N = parse_N(argc, argv);

    size_t capacity = 1024;
    size_t length = 0;
    uint64_t *primes = malloc(sizeof(uint64_t) * capacity);
    if (primes == NULL) {
        fprintf(stderr, "Unexpectedly run out of memory\n");
        exit(3);
    }

    for (uint64_t n = 2; n <= N; ++n) {
        uint64_t n_sqrt = isqrt(n);
        
        for (size_t i = 0; i < length && primes[i] <= n_sqrt; ++i) {
            if (n % primes[i] == 0) {
                goto not_prime;
            }
        }

        fprintf(stdout, "%llu\n", (unsigned long long) n);

        if (length == capacity) {
            size_t newcap = capacity << 1;
            uint64_t *newarr = realloc(primes, sizeof(uint64_t) * newcap);
            if (newarr == NULL) {
                fprintf(stderr, "Unexpectedly run out of memory\n");
                free(primes);
                exit(3);
            }

            primes = newarr;
            capacity = newcap;
        }

        primes[length++] = n;

not_prime:
        (void) 0;
    }

#ifndef NDEBUG
    free(primes);
#endif
    exit(0);
}
