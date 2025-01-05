#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "parse.h"


int main(int argc, char **argv)
{
    const uint64_t N = parse_N(argc, argv);

    if (N < 2) {
        exit(0);
    }

    size_t P_cap = 1000;
    size_t V_cap = 100;

    uint64_t *P = realloc(NULL, sizeof(uint64_t) * P_cap);
    if (P == NULL) {
        fprintf(stderr, "Ran out of memory\n");
        exit(3);
    }

    uint64_t *V = realloc(NULL, sizeof(uint64_t) * V_cap);
    if (V == NULL) {
        fprintf(stderr, "Ran out of memory\n");
        free(P);
        exit(3);
    }

    size_t i = 1;
    size_t lim = 0;
    uint64_t x = 1;

    P[0] = 2;
    fprintf(stdout, "%llu\n", 2ULL);

    uint64_t multiple = 4;

    for (;;) {
        x += 2;

        if (x >= N) {
            break;
        }

        if (lim == V_cap) {
            uint64_t *v = realloc(V, sizeof(uint64_t) * V_cap * 2);
            if (v == NULL) {
                fprintf(stderr, "Ran out of memory\n");
                free(P);
                free(V);
                exit(3);
            }
            
            V = v;
            V_cap *= 2;
        }

        if (multiple <= x) {
            V[lim++] = multiple;
            multiple = P[lim] * P[lim];
        }

        for (size_t k = 1; k < lim; ++k) {
            if (V[k] < x) {
                V[k] += P[k];
            }

            if (x == V[k]) {
                goto not_prime;
            }
        }

        if (i == P_cap) {
            uint64_t *p = realloc(P, sizeof(uint64_t) * P_cap * 2);
            if (p == NULL) {
                fprintf(stderr, "Ran out of memory\n");
                free(P);
                free(V);
                exit(3);
            }

            P = p;
            P_cap *= 2;
        }

        P[i++] = x;
        fprintf(stdout, "%llu\n", (unsigned long long) x);

not_prime:
        (void) 0;
    }

    free(V);
    free(P);
    exit(0);
}
