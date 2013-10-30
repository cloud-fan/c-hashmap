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
    char key5[] = "eee";
    char val5[] = "5555";

    hashmap_insert(map, (void*)val1, key1);
    hashmap_insert(map, (void*)val2, key2);
    hashmap_insert(map, (void*)val3, key3);
    hashmap_insert(map, (void*)val4, key4);
    hashmap_insert(map, (void*)val5, key5);

    printf("%d\n", hashmap_count(map));

    printf("%s\n", (char*)hashmap_get(map, "ddd"));
    printf("%s\n", (char*)hashmap_get(map, "eee"));
    printf("%s\n", (char*)hashmap_get(map, "ccc"));
    printf("%s\n", (char*)hashmap_get(map, "aaa"));
    printf("%s\n", (char*)hashmap_get(map, "bbb"));

    return 0;
}