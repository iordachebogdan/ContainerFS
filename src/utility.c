#include "utility.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

void pop_front(struct List** head) {
    struct List* new_head = (*head)->next;
    free(*head);
    *head = new_head;
}

void push_front(struct List** head, void* data) {
    struct List* new_head = malloc(sizeof(struct List));
    new_head->data = data;
    new_head->next = *head;
    *head = new_head;
}

int count_arguments(const char* s) {
    int res = 0;
    for (int i = 0; s[i]; ++i) {
        res += (s[i] == ',');
    }
    return res;
}

void variadic_free(int n, ...) {
    va_list args;
    va_start(args, n);
    for (int i = 0; i < n; ++i) {
        free(va_arg(args, void*));
    }
    va_end(args);
}
