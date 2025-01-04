#include <stdint.h>
#include "isqrt.h"


uint64_t isqrt(uint64_t n)
{
    int shift = 2;
    uint64_t result = 0;

    if (n < 2) {
        // isqrt(0) -> 0, isqrt(1) -> 1
        return n;
    }

    // find the shift amount
    while ((n >> shift) != 0) {
        shift += 2;
    }

    while (shift >= 0) {
        uint64_t cand = (result <<= 1ULL) + 1;

        if (cand * cand <= (n >> shift)) {
            result = cand;
        }

        shift -= 2;
    }

    return result;
}
