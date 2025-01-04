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


static int insert_prime(uint64_t n)
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
    uint64_t N = parse_N(argc, argv);
    uint64_t n = 3;

    if (N < 2) {
        exit(0);
    }

    capacity = 1024;
    size = 0;
    heap = realloc(NULL, sizeof(struct prime) * capacity);
    if (heap == NULL) {
        fprintf(stderr, "Unexpectedly ran out of memory\n");
        exit(3);
    }

    if (insert_prime(2) != 0) {
        fprintf(stderr, "Unexpectedly ran out of memory\n");
        free(heap);
        exit(3);
    }
    fprintf(stdout, "%llu\n", 2ULL);

    while (n <= N) {
        if (n < heap[1].multiple) {
            int rc = insert_prime(n);
            if (rc != 0) {
                fprintf(stderr, "Unexpectedly ran out of memory\n");
                free(heap);
                exit(3);
            }
            fprintf(stdout, "%llu\n", (unsigned long long) n);

        } else {
            while (heap[1].multiple == n) {
                heap[1].multiple += heap[1].prime;
                percolate_down(1);
            }
        }

        ++n;
    }

#ifndef NDEBUG
    free(heap);
#endif
    exit(0);
}
