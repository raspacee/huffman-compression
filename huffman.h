#ifndef HUFFMAN_H
#define HUFFMAN_H
typedef struct Node_ {
    int freq;
    char ch;
    struct Node_ *left;
    struct Node_ *right;
} Node;

#define ALPHABETS_LEN 26

void print_codes(Node *node, int arr[], int top);
#endif