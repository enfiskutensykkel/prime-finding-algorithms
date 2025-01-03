#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include "parse.h"


uint64_t parse_N(int argc, char * const * argv)
{
    char *endptr = NULL;
    unsigned long long N = 0;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s N\n\nFind prime numbers less than or equal to N.", argv[0]);
        exit(1);
    }

    if (argc > 2) {
        fprintf(stderr, "Too many arguments\n");
        fprintf(stderr, "Usage: %s\n", argv[0]);
        exit(1);
    }
    
    N = strtoull(argv[1], &endptr, 0);
    if (N == 0ULL || N >= UINT64_MAX || *endptr != '\0') {
        fprintf(stderr, "Invalid number: %s\n", argv[1]);
        if (errno == ERANGE || N == 0ULL || N >= UINT64_MAX) {
            fprintf(stderr, "N must be between 2 and %llu", (unsigned long long) UINT64_MAX);
            exit(1);
        }
        return 0ULL;
    }

    return N;
}
