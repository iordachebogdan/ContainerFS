#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <stdarg.h>
#include <stdlib.h>

#define FREE(...) variadic_free(count_arguments(#__VA_ARGS__), __VA_ARGS__)

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

#endif