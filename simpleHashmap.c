#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "simpleHashmap.h"

struct s_map_entry
{
    void* data;
    char* key;
    struct s_map_entry* next;
};

typedef struct s_map_entry map_entry;

struct s_hashmap
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

static map_entry* generate_map_entry(void* data, size_t data_len, char* key, size_t key_len)
{
    map_entry* entry = (map_entry*)malloc(sizeof(map_entry));
    entry->key = copy_content(key, key_len+1);
    entry->data = copy_content(data, data_len);
    entry->next = NULL;   
    return entry;
}

static void free_map_entry(map_entry* entry)
{
    free(entry->key);
    free(entry->data);
    free(entry);
}

//return 0: stop search because of key match
static int search_map_entry_in_linked_list(map_entry** entry, char* key, size_t key_len, map_entry** previous)
{
    int key_match = memcmp((*entry)->key, key, key_len);
    while(key_match != 0 && (*entry)->next != NULL)
    {
        *previous = *entry;
        *entry = (*entry)->next;
        key_match = memcmp((*entry)->key, key, key_len);
    }
    return key_match;
}

static size_t string_hash(char* string)
{
    size_t hash;  
    unsigned char *p;  
  
    for(hash = 0, p = (unsigned char *)string; *p ; p++)  
        hash = 31 * hash + *p;  
  
    return (hash & 0x7FFFFFFF);  
}



/*=============================================================================
                            Public Functions
=============================================================================*/

hashmap* create_hashmap(size_t size)
{
    hashmap* hm = (hashmap*)malloc(sizeof(hashmap));
    hm->table = (map_entry**)calloc(sizeof(map_entry*), size);
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
        {
            map_entry* next = entry->next;
            free_map_entry(entry);
            while(next != NULL)
            {
                map_entry* tmp = next->next;
                free_map_entry(next);
                next = tmp;
            }
        }

    }
    free(map->table);
    free(map);
}

void hashmap_insert(hashmap* map, void* data, size_t data_len, char* key, size_t key_len)
{
    if(map->size <= map->count)
    {
        printf("Hashmap is full, insert failed!\n");
        return;
    }

    ++map->count;
    int index = string_hash(key) % map->size;
    if(map->table[index] == NULL)
        map->table[index] = generate_map_entry(data, data_len, key, key_len);
    else
    {
        map_entry* entry = map->table[index];
        int key_match = search_map_entry_in_linked_list(&entry, key, key_len, NULL);
        if(key_match == 0)
        {
            free(entry->data);
            entry->data = copy_content(data, data_len);
            --map->count;
        }
        else
            entry->next = generate_map_entry(data, data_len, key, key_len);
    }
}

void hashmap_remove(hashmap* map, char* key, size_t key_len)
{
    if(map->count == 0)
        return;

    int index = string_hash(key) % map->size;
    map_entry* entry = map->table[index];
    if(entry != NULL)
    {
        map_entry* previous = NULL;
        int key_match = search_map_entry_in_linked_list(&entry, key, key_len, &previous);
        if(key_match == 0)
        {
            --map->count;
            if(previous == NULL)
                map->table[index] = entry->next;
            else
                previous->next = entry->next;
            free(entry);
        }
    }
}

void* hashmap_get(hashmap* map, char* key, size_t key_len)
{
    if(map->count == 0)
        return NULL;

    map_entry* entry = map->table[string_hash(key) % map->size];
    if(entry != NULL)
    {
        int key_match = search_map_entry_in_linked_list(&entry, key, key_len, NULL);
        if(key_match == 0)
            return entry->data;
    }
    return NULL;
}

size_t hashmap_count(hashmap* map)
{
    return map->count;
}