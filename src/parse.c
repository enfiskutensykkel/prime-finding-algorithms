#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>


unsigned long long int parse_N(const char *arg)
{
    char *endptr = NULL;
    unsigned long long N = 0;

    N = strtoull(arg, &endptr, 0);
    if (N == 0ULL || N == ULLONG_MAX || *endptr != '\0') {
        return 0ULL;
    }

    return N;
}
