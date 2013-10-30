#ifndef _SIMPLE_HASHMAP_H_
#define _SIMPLE_HASHMAP_H_

#include <stddef.h>

struct s_hashmap;
typedef struct s_hashmap hashmap;

hashmap* create_hashmap(size_t size);

void free_hashmap(hashmap* map);

void hashmap_insert(hashmap* map, void* data, char* key);

void hashmap_remove(hashmap* map, char* key);

void* hashmap_get(hashmap* map, char* key);

size_t hashmap_count(hashmap* map);

#endif