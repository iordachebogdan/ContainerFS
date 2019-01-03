#ifndef _UTILITY_H_
#define _UTILITY_H_

#define FREE(...) variadic_free(count_arguments(#__VA_ARGS__), __VA_ARGS__)

struct List {
    void* data;
    struct List* next;
};

void pop_front(struct List**);
void push_front(struct List**, void*);

int count_arguments(const char*);
void variadic_free(int, ...);

#endif
