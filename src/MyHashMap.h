#ifndef __MY_HASH_MAP__
#define __MY_HASH_MAP__

#include <stdint.h>

typedef uintptr_t hhandle;

hhandle create_table();

void* h_find(hhandle table, const char* key);

void h_add(hhandle table, const char* key, void * value);

void h_forEach(hhandle table, void (*action)(const char* key, void* value));

#endif