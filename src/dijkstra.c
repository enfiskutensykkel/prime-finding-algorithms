#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <limits.h>
#include "parse.h"


struct prime
{
    unsigned long long int prime;
    unsigned long long int multiple;
};


static size_t capacity = 0;
static size_t size = 0;

static struct prime *heap = NULL;


static void percolate_up(size_t idx)
{
    size_t child;

    while ((child = idx >> 1) > 0) {

        if (heap[idx].multiple < heap[child].multiple) {
            struct prime p = heap[child];
            heap[child] = heap[idx];
            heap[idx] = p;
        }

        idx = child;
    }
}


static void percolate_down(size_t idx)
{
    size_t child;

    while ((child = idx << 1) <= size) {

        if (child + 1 <= size && heap[child+1].multiple < heap[child].multiple) {
            ++child;
        }

        if (heap[idx].multiple > heap[child].multiple) {
            struct prime p = heap[idx];
            heap[idx] = heap[child];
            heap[child] = p;
        }

        idx = child;
    }
}


static int insert_prime(unsigned long long int n)
{
    if (size + 1 == capacity) {
        size_t newcap = capacity << 1;
        struct prime *newheap = realloc(heap, sizeof(struct prime) * newcap);

        if (newheap == NULL) {
            return ENOMEM;
        }

        heap = newheap;
        capacity = newcap;
    }

    size_t idx = ++size;
    heap[idx].prime = n;
    heap[idx].multiple = n * n;

    percolate_up(idx);

    return 0;
}


int main(int argc, char **argv)
{
    long long unsigned N;
    long long unsigned n = 3;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s N\n\nFind prime numbers less than or equal to N.", argv[0]);
        return 1;
    }
    
    N = parse_N(argv[1]);
    if (N == 0) {
        fprintf(stderr, "Invalid number: %s\n", argv[1]);
        fprintf(stderr, "N must be between 1 and %llu", ULLONG_MAX);
        return 2;
    }

    if (N < 2) {
        // we are done, no prime numbers below 2
        return 0;
    }

    capacity = 64;
    size = 0;
    heap = realloc(NULL, sizeof(struct prime) * capacity);
    if (heap == NULL) {
        fprintf(stderr, "Unexpectedly ran out of memory\n");
        return 3;
    }

    // the first element in the heap array is just a placeholder
    heap[0].prime = 0;
    heap[0].multiple = 0;

    insert_prime(2);
    fprintf(stdout, "%llu\n", 2ULL);

    while (n <= N) {
        if (n < heap[1].multiple) {
            int rc = insert_prime(n);
            if (rc != 0) {
                fprintf(stderr, "Unexpectedly ran out of memory\n");
                return 3;
            }
            fprintf(stdout, "%llu\n", n);

        } else {
            while (heap[1].multiple == n) {
                heap[1].multiple += heap[1].prime;
                percolate_down(1);
            }
        }

        ++n;
    }

    free(heap);

    return 0;
}
