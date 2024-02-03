#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

HashMap *initialize_hashmap(int size, DataType type) {
    HashMap *map = malloc(sizeof(HashMap));
    map->type = type;
    map->buckets = malloc(sizeof(BucketItem) * size);
    map->size = size;

    for (int i=0; i<size; i++) {
        map->buckets[i] = NULL;
    }

    return map;
}

/* Returns non-zero value if sucessful */
int insert_hashmap(HashMap *map, char *key, BucketData *data) {
    if (map->filled == map->size) {
        printf("insert_hashmap: hashmap size is full, cannot add more.\n");
        return -1;
    }

    int index = hash_code(map, key);

    while (map->buckets[index] != NULL) {
        if (strcmp(map->buckets[index]->key, key) == 0) {
            printf("insert_hashmap: this key %s is already added.\n", key);
            return -1;
        }
        /* Loop the index around the array incase end is reached */
        index = (index + 1) % map->size;     
    }

    BucketItem *item = malloc(sizeof(BucketItem));
    item->data = data;
    item->key = key;
    map->buckets[index] = item;
    map->filled++;

    return 0;
}

/* Returns NULL if not found */
BucketData *get_hashmap(HashMap *map, char *key) {
    int index = hash_code(map, key);

    while (map->buckets[index] != NULL) {
        if (strcmp(map->buckets[index]->key, key) == 0)
            return map->buckets[index]->data;
        
        index = (index + 1) % map->size;
    }

    return NULL;
}

int hash_code(HashMap *map, char *key) {
    /* TODO: Find a better way to do this */
    return key[0] % map->size;
}

// int main() {
//     HashMap *map = initialize_hashmap(100, FREQ_TYPE);

//     BucketData a = { .freq = 3 };
//     insert_hashmap(map, "a", &a);
//     BucketData b = { .freq = 99 };
//     insert_hashmap(map, "z", &b);
//     BucketData c = { .freq = 34 };
//     insert_hashmap(map, "/", &c);

//     char *key = "z";
//     BucketData *result;
//     if ((result = get_hashmap(map, key)) != NULL) {
//         printf("%s -> %d\n", key, result->freq);
//     } else {
//         printf("Not found\n");
//     }

//     HashMap *map2 = initialize_hashmap(100, CODE_TYPE);

//     BucketData w = { .code = "00" };
//     insert_hashmap(map2, "a", &w);
//     BucketData x = { .code = "101" };
//     insert_hashmap(map2, "z", &x);
//     BucketData y = { .code = "10001" };
//     insert_hashmap(map2, "/", &y);

//     char *key2 = "e";
//     BucketData *result2;
//     if ((result2 = get_hashmap(map2, key2)) != NULL) {
//         printf("%s -> %s\n", key2, result2->code);
//     } else {
//         printf("Not found\n");
//     }
//     return 0;
// }