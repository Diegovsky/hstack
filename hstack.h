#pragma once
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct heapstack hstack_t;

typedef void (*fnptr)(void*);

extern __thread hstack_t* current_hs;

hstack_t* heapstack_new(size_t stack_size);
void heapstack_call(hstack_t* hs, fnptr fn, void* arg);
void heapstack_reserve(hstack_t* hs, size_t size);
ssize_t heapstack_remaining_space(hstack_t* hs);
void heapstack_free(hstack_t* hs);

void print_num(const char* prompt, int num);
