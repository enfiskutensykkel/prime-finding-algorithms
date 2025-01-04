#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "parse.h"
#include "isqrt.h"


struct prime
{
    uint64_t value;
    struct prime *next;
    struct prime *prev;
};


static void insert_prime(struct prime *list_head, uint64_t n)
{
    struct prime *p = malloc(sizeof(struct prime));
    if (p == NULL) {
        fprintf(stderr, "Unexpectedly ran out of memory\n");
        exit(3);
    }

    p->value = n;
    p->next = list_head;
    p->prev = list_head->prev;
    list_head->prev->next = p;
    list_head->prev = p;
}


#ifndef NDEBUG
static void remove_prime(struct prime *list_head)
{
    list_head->next->prev = list_head->prev;
    list_head->prev->next = list_head->next;
    free(list_head);
}
#endif


int main(int argc, char **argv)
{
    uint64_t N = parse_N(argc, argv);

    struct prime head = {
        .value = 0,
        .next = &head,
        .prev = &head
    };

    for (uint64_t n = 2; n <= N; ++n) {
        uint64_t n_sqrt = isqrt(n);
        
        for (const struct prime *it = head.next;
                it != &head && it->value <= n_sqrt;
                it = it->next) {

            if (n % it->value == 0) {
                goto not_prime;
            }
        }

        fprintf(stdout, "%llu\n", (unsigned long long) n);
        insert_prime(&head, n);

not_prime:
        (void) 0;
    }

#ifndef NDEBUG
    for (struct prime *it = head.next, *next = it->next; 
        it != &head;
        it = next, next = next->next) {
        remove_prime(it);
    }
#endif

    exit(0);
}
