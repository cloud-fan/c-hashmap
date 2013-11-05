#ifndef _SIMPLE_HASHMAP_H_
#define _SIMPLE_HASHMAP_H_

#include <stddef.h>

struct s_hashmap;
typedef struct s_hashmap hashmap;

hashmap* create_hashmap(size_t size);

void free_hashmap(hashmap* map);

int hashmap_insert(hashmap* map, char* key, size_t key_len, void* data, size_t data_len);

int hashmap_remove(hashmap* map, char* key, size_t key_len);

void* hashmap_get(hashmap* map, char* key, size_t key_len);

size_t hashmap_count(hashmap* map);

#endif