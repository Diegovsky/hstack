
#include "hstack.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define UNW_LOCAL_ONLY
#include <libunwind.h>

#ifndef __GNUC__
#pragma clang diagnostic ignored "-Wpragmas"
#else
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#endif
#pragma GCC diagnostic ignored "-Wpointer-arith"
#pragma clang diagnostic ignored "-Wlanguage-extension-token"

struct heapstack {
  size_t stack_size;
  void *stack;
};

__thread hstack_t *current_hs = NULL;

const size_t STACK_EXCESS = 40 * sizeof(ptrdiff_t);

hstack_t *heapstack_new(size_t stack_size) {
    hstack_t *hs = malloc(sizeof(hstack_t));
    hs->stack = malloc(stack_size + STACK_EXCESS);
    hs->stack_size = stack_size + STACK_EXCESS;
    return hs;
}

void heapstack_reserve(hstack_t *hs, size_t size) {
    if (hs->stack_size < size) {
        hs->stack_size = size + STACK_EXCESS;
        hs = realloc(hs->stack, hs->stack_size);
    }
}

struct stack_info {
    void* stack;
    unw_word_t ins_ptr;
};

__attribute_noinline__ static struct stack_info stack_info(int level) {
    unw_cursor_t cursor; unw_context_t uc;
    unw_word_t sp, ip;

    unw_getcontext(&uc);
    unw_init_local(&cursor, &uc);
    // Let's skip the current stack frame;
    unw_step(&cursor);
    for(int i = 0; i < level; i++) {
        unw_step(&cursor);
    }
    unw_get_reg(&cursor, UNW_REG_SP, &sp);
    unw_get_reg(&cursor, UNW_REG_IP, &ip);
    return (struct stack_info) { .stack = (void*)sp, .ins_ptr = ip };
}

typedef struct {
    void* stack_start;
    void* current_sp;
} program_stack_t;

static program_stack_t current_program_stack(void) {
    unw_cursor_t cursor; unw_context_t uc;
    unw_word_t sp;

    size_t current_sp;

    unw_getcontext(&uc);
    unw_init_local(&cursor, &uc);
    unw_step(&cursor);
    unw_get_reg(&cursor, UNW_REG_SP, &sp);
    current_sp = (size_t)sp;

    while (unw_step(&cursor) > 0) {
        unw_get_reg(&cursor, UNW_REG_SP, &sp);
    }

    return (program_stack_t) { .stack_start = (void*)sp, .current_sp = (void*)current_sp };
}

static void pstack_info(int level) {
    struct stack_info si = stack_info(level+1);
    printf("Stack pointer: %p\n", si.stack);
    printf("Instruction pointer: %p\n", (void*)si.ins_ptr);
}

void heapstack_call(hstack_t *hs, fnptr fn, void *arg) {
    program_stack_t progstack = current_program_stack();
    size_t progstack_size = progstack.stack_start - progstack.current_sp;
    heapstack_reserve(hs, hs->stack_size + progstack_size);

    void *stack_base = (hs->stack + hs->stack_size - progstack_size);

    memcpy(stack_base, progstack.current_sp, progstack_size);
    stack_base -= 8;

    asm volatile(
        // save current stack pointer into r11
        "movq %%rsp, %%r11\n"

        // set stack pointer to stack_base
        "movq %2, %%rsp\n"

        // save old stack pointer on new stack
        "pushq %%r11\n"
        // save old base stack pointer on new stack
        "pushq %%rbp\n"

        // align stack pointer with base stack pointer
        "movq %%rsp, %%rbp\n"

        // save `current_hs` in the stack
        "pushq %0\n"
        // change `current_hs` to point to new stack
        "movq %2, %0\n"

        // call function with argument
        "movq %1, %%rdi\n"
        "callq *%3\n"

        // restore old base stack pointer
        "popq %%rbp\n"
        // restore old stack pointer
        "popq %%r11\n"
        "movq %%r11, %%rsp\n"

        // Restore `current_hs` from the stack
        "popq %0\n"

        : "=r"(current_hs), "+r"(arg) : "r"(stack_base), "r"(fn) : "r11", "r12", "rdi", "memory");

    return;
}

ssize_t heapstack_remaining_space(hstack_t *hs) {
    if (hs == NULL) {
        return 0;
    }
    void *rsp;
    asm("movq %%rsp, %0" : "=r"(rsp));

    return rsp - hs->stack;
}

void heapstack_free(hstack_t *hs) { free(hs->stack); free(hs); }

#include <unistd.h>

void print_num(const char* prompt, int number) {
    write(STDOUT_FILENO, prompt, strlen(prompt));
    if (number == 0) {
        write(STDOUT_FILENO, "0\n\r", 3);
    } else {
        char buf[6] = {0};
        const size_t bufsize = sizeof(buf);
        int i = 0;
        if (number < 0) {
            buf[i++] = '-';
            number = ~number + 1;
        }
        for (; number > 0 && (size_t)i < bufsize; i++) {
            buf[i] = (number % 10) + '0';
            number /= 10;
        }
        char num[i];
        for (int j = 0; j < i; j++) {
            num[j] = buf[i - j - 1];
        }
        write(STDOUT_FILENO, num, i);

        write(STDOUT_FILENO, "\n\r", 1);
    }
}
