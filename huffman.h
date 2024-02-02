#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "hashmap.h"

typedef struct Node_ {
    int freq;
    char ch;
    struct Node_ *left;
    struct Node_ *right;
} Node;

#define ALPHABETS_LEN 26

void print_codes(Node *root, char arr[], int top);
void store_codes(HashMap *map, Node *root, char arr[], int top);

#endif