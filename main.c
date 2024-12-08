#include <stdio.h>
#include "hstack.h"

int recfn(long n) {
    print_num("Recursing with n = ", n);
    if (n == 0 || n == 1) {
        return n;
    }
    long f = recfn(n - 1) + recfn(n - 2);
    return f;
}

void heap_entry(void* p) {
    print_num("Remaining space: ", heapstack_remaining_space(current_hs));
    long* c = (long*) p;
    *c = recfn(*c);
    return;
}


int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <number>\n", argv[0]);
        return 1;
    }
    long c = atol(argv[1]);
    hstack_t* hs = heapstack_new(4096);
    heapstack_call(hs, (fnptr)recfn, (void*)c);
    for(long i = 0; i <= c; i++) {
        long r = i;
        heapstack_call(hs, heap_entry, (void*)&r);
        printf("Fib(%ld): %ld\n", i, r);
    }
    heapstack_free(hs);
    return 0;
}
