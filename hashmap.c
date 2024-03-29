#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

HashMap *initialize_hashmap(int size, DataType type) {
    HashMap *map = malloc(sizeof(HashMap));
    map->type = type;
    map->buckets = malloc(sizeof(BucketItem) * size);
    map->size = size;
    map->filled = 0;

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
    int orig_index = index;

    while (map->buckets[index] != NULL) {
        if (strcmp(map->buckets[index]->key, key) == 0)
            return map->buckets[index]->data;
        
        index = (index + 1) % map->size;
        if (orig_index == index) break;
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

//     BucketData *tmp = malloc(sizeof(BucketData));
//     tmp->freq = 101;
//     tmp->freq = 90;
//     printf("%d\n", get_hashmap(map, "z")->freq);

//     return 0;
// }