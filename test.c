#include <stdlib.h>
#include <stdio.h>
#include "simpleHashmap.h"

int main(int argc, char const *argv[])
{
    hashmap* map = create_hashmap(1000);
    char key1[] = "aaa";
    char val1[] = "123";
    char key2[] = "bbb";
    char val2[] = "121";
    char key3[] = "ccc";
    char val3[] = "111";
    char key4[] = "ddd";
    char val4[] = "133";
    char key5[] = "aaa";
    char val5[] = "555";

    hashmap_insert(map, key1, 3, (void*)val1, 4);
    hashmap_insert(map, key2, 3, (void*)val2, 4);
    hashmap_insert(map, key3, 3, (void*)val3, 4);
    hashmap_insert(map, key4, 3, (void*)val4, 4);
    hashmap_insert(map, key5, 3, (void*)val5, 4);

    printf("%d\n", hashmap_count(map));

    printf("%s\n", (char*)hashmap_get(map, "ddd",3));
    printf("%s\n", (char*)hashmap_get(map, "ccc",3));
    printf("%s\n", (char*)hashmap_get(map, "aaa",3));
    printf("%s\n", (char*)hashmap_get(map, "bbb",3));

    free_hashmap(map);

    return 0;
}