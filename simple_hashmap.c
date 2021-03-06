#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "simple_hashmap.h"

struct _map_entry
{
    uint64_t key_hash;
    void* data;
    struct _map_entry* next;
};

typedef struct _map_entry map_entry;

struct _hashmap
{
    map_entry** table;
    size_t size;
    size_t count;
};

static void* copy_content(void* content, size_t len)
{
    void* copy = malloc(len);
    memcpy(copy,content,len);
    return copy;
}

static map_entry* generate_map_entry(void* data, size_t data_len, uint64_t key_hash)
{
    map_entry* entry = malloc(sizeof(map_entry));
    entry->key_hash = key_hash;
    entry->data = copy_content(data, data_len);
    entry->next = NULL;   
    return entry;
}

static void free_map_entries(map_entry* entry)
{
    map_entry* next = entry->next;
    free(entry->data);
    free(entry);
    if(next != NULL)
        free_map_entries(next);
}

static int search_map_entry_in_linked_list(map_entry** entry, uint64_t key_hash, map_entry** previous)
{
    int key_match = (*entry)->key_hash == key_hash;
    while(!key_match && (*entry)->next != NULL)
    {
        *previous = *entry;
        *entry = (*entry)->next;
        key_match = (*entry)->key_hash == key_hash;
    }
    return key_match;
}

static uint64_t string_hash(char* string, size_t len)
{
    uint64_t hash = 0;  
    unsigned char *p;  

    int i;
    for(i = 0, p = (unsigned char *)string; i<len ; i++)
        hash = 31 * hash + *(p+i);
  
    return (hash & 0x7FFFFFFFFFFFFFFF);  
}



/*=============================================================================
                            Public Functions
=============================================================================*/

hashmap* create_hashmap(size_t size)
{
    hashmap* hm = malloc(sizeof(hashmap));
    hm->table = calloc(sizeof(map_entry*), size);
    hm->size = size;
    hm->count = 0;
    return hm;
}

void free_hashmap(hashmap* map)
{
    int i;
    for(i=0;i<map->size;i++)
    {
        map_entry* entry = map->table[i];
        if(entry != NULL)
            free_map_entries(entry);
    }
    free(map->table);
    free(map);
}

int hashmap_insert(hashmap* map, char* key, size_t key_len, void* data, size_t data_len)
{
    return hashmap_insert_uint64_key(map, string_hash(key, key_len), data, data_len);
}

int hashmap_insert_uint64_key(hashmap* map, uint64_t key, void* data, size_t data_len)
{
    if(map->size <= map->count)
        return -1;

    ++map->count;
    size_t index = key % map->size;
    if(map->table[index] == NULL)
        map->table[index] = generate_map_entry(data, data_len, key);
    else
    {
        map_entry* entry = map->table[index];
        int key_match = search_map_entry_in_linked_list(&entry, key, NULL);
        if(key_match)
        {
            free(entry->data);
            entry->data = copy_content(data, data_len);
            --map->count;
        }
        else
            entry->next = generate_map_entry(data, data_len, key);
    }

    return 0;    
}

int hashmap_remove(hashmap* map, char* key, size_t key_len)
{
    return hashmap_remove_uint64_key(map, string_hash(key, key_len));
}

int hashmap_remove_uint64_key(hashmap* map, uint64_t key)
{
    if(map->count == 0)
        return -1;

    size_t index = key % map->size;
    map_entry* entry = map->table[index];
    if(entry != NULL)
    {
        map_entry* previous = NULL;
        int key_match = search_map_entry_in_linked_list(&entry, key, &previous);
        if(key_match)
        {
            --map->count;
            if(previous == NULL)
                map->table[index] = entry->next;
            else
                previous->next = entry->next;
            free(entry->data);
            free(entry);
        }
    }

    return 0;    
}

void* hashmap_get(hashmap* map, char* key, size_t key_len)
{
    return hashmap_get_uint64_key(map, string_hash(key, key_len));
}

void* hashmap_get_uint64_key(hashmap* map, uint64_t key)
{
    if(map->count == 0)
        return NULL;

    map_entry* entry = map->table[key % map->size];
    if(entry != NULL)
    {
        int key_match = search_map_entry_in_linked_list(&entry, key, NULL);
        if(key_match)
            return entry->data;
    }
    return NULL;    
}

size_t hashmap_count(hashmap* map)
{
    return map->count;
}