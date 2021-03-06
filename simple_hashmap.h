/* This hash map doesn't keep the address of key and value when user call insert.
   So after user call insert, they can free key and value if necessary.
   When user call get, the data returned point to the real content in hash table.
   So DO NOT FREE THE RETURNED DATA! */

#ifndef _SIMPLE_HASHMAP_H_
#define _SIMPLE_HASHMAP_H_

#include <stddef.h>
#include <stdint.h>

struct _hashmap;
typedef struct _hashmap hashmap;

hashmap* create_hashmap(size_t size);

void free_hashmap(hashmap* map);

int hashmap_insert(hashmap* map, char* key, size_t key_len, void* data, size_t data_len);

int hashmap_insert_uint64_key(hashmap* map, uint64_t key, void* data, size_t data_len);

int hashmap_remove(hashmap* map, char* key, size_t key_len);

int hashmap_remove_uint64_key(hashmap* map, uint64_t key);

void* hashmap_get(hashmap* map, char* key, size_t key_len);

void* hashmap_get_uint64_key(hashmap* map, uint64_t key);

size_t hashmap_count(hashmap* map);

#endif