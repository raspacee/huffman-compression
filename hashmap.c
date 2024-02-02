#include <stdio.h>
#include <stdlib.h>
#include "hashmap.h"

// int main() {
//     HashMap *map = initialize_hashmap(100);

//     insert_hashmap(map, 'a', "1011");
//     insert_hashmap(map, 'c', "001");
//     insert_hashmap(map, 'd', "1110");
//     insert_hashmap(map, 'e', "10100");

//     char key = 'e';
//     char *result;
//     if ((result = get_hashmap(map, key)) != NULL) {
//         printf("%c - %s\n", key, result);
//     } else {
//         printf("Not found\n");
//     }
//     return 0;
// }

HashMap *initialize_hashmap(int size) {
    HashMap *map = malloc(sizeof(HashMap));
    map->buckets = malloc(sizeof(BucketItem) * size);
    map->size = size;

    for (int i=0; i<size; i++) {
        map->buckets[i] = NULL;
    }

    return map;
}

/* Returns non-zero value if sucessful */
int insert_hashmap(HashMap *map, char key, char *code) {
    if (map->filled == map->size) {
        printf("insert_hashmap: hashmap size is full, cannot add more.\n");
        return -1;
    }

    int index = hash_code(map, key);

    while (map->buckets[index] != NULL) {
        if (map->buckets[index]->key == key) {
            printf("insert_hashmap: this key %c is already added.\n", key);
            return -1;
        }
        /* Loop the index around the array incase end is reached */
        index = (index + 1) % map->size;     
    }

    BucketItem *item = malloc(sizeof(BucketItem));
    item->code = code;
    item->key = key;
    map->buckets[index] = item;
    map->filled++;

    return 0;
}

/* Returns NULL if not found */
char *get_hashmap(HashMap *map, char key) {
    int index = hash_code(map, key);

    while (map->buckets[index] != NULL) {
        if (map->buckets[index]->key == key)
            return map->buckets[index]->code;
        
        index = (index + 1) % map->size;
    }

    return NULL;
}

int hash_code(HashMap *map, char key) {
    return key % map->size;
}