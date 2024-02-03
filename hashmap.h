#ifndef HASHMAP_H
#define HASHMAP_H

typedef enum DataType {
    FREQ_TYPE,
    CODE_TYPE
} DataType;

typedef union BucketData {
    char *code;
    int freq;
} BucketData;

typedef struct BucketItem {
    char *key;
    BucketData *data;
} BucketItem;

typedef struct HashMap {
    BucketItem **buckets;
    DataType type; /* Type of data the hashmap stores, either integer or pointer to string */
    int size;
    int filled; /* How much of the buckets is filled */
} HashMap;

HashMap *initialize_hashmap(int size, DataType type);
int insert_hashmap(HashMap *map, char *key, BucketData *data);
BucketData *get_hashmap(HashMap *map, char *key);
int hash_code(HashMap *map, char *key);

#endif