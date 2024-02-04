#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "priority.h"
#include "hashmap.h"
#include "common.h"

#define ALPHABETS_LEN 26

#pragma pack(1)
typedef struct MetaData_ {
    char code[16];
    char alphabet;
} MetaData;
#pragma pack()

void print_codes(Node *root, char arr[], int top);
void store_codes(HashMap *code_map, HashMap *alph_map, Node *root, char arr[], int top);
int compress_file(FILE *orig_file, HashMap *code_map, HashMap *alph_map);
int decompress_file(char *filename);
Node *create_htree(PQueue *q);

#endif