#ifndef _UTILITY_H_
#define _UTILITY_H_

#define FREE(...) variadic_free(count_arguments(#__VA_ARGS__), __VA_ARGS__)

int count_arguments(const char*);
void variadic_free(int, ...);

#endif