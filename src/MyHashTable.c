#include "MyHashMap.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct
{
    int n;
    char** keys;
    void** values;
} HASH_TABLE;

hhandle create_table()
{
    HASH_TABLE* table = (HASH_TABLE*) malloc(sizeof(HASH_TABLE));
    table->n = 0;
    table->keys = (char**) malloc(sizeof(char*)*100);
    table->values = (void**) malloc(sizeof(void*)*100);
    return table;
}

void* h_find(hhandle table, const char* key)
{
    HASH_TABLE* t = (HASH_TABLE*)table;
    for (int i = 0 ; i < t->n ; i++) {
        if (strcmp(key, t->keys[i])==0) return t->values[i];
    }
    return NULL;
}

void h_add(hhandle table, const char* key, void * value)
{
    void * r = h_find(table, key);
    if (r == NULL) {
        HASH_TABLE* t = (HASH_TABLE*)table;
        t->keys[t->n] = key;
        t->values[t->n] = value;
        t->n++;
    }
}


void
h_forEach(hhandle table, void (*action)(const char* key, void* value))
{
    HASH_TABLE* t = (HASH_TABLE*)table;
    for (int i = 0 ; i < t->n ; i++) {
        action(t->keys[i], t->values[i]);
    }
}