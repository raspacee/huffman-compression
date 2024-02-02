#ifndef HASHMAP_H
#define HASHMAP_H

typedef struct BucketItem {
    char key;
    char *code;
} BucketItem;

typedef struct HashMap {
    BucketItem **buckets;
    int size;
    int filled; /* How much of the buckets is filled */
} HashMap;

HashMap *initialize_hashmap(int size);
int insert_hashmap(HashMap *map, char key, char *code);
char *get_hashmap(HashMap *map, char key);
int hash_code(HashMap *map, char key);

#endif