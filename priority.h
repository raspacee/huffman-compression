#include <stdbool.h>
#include "huffman.h"

Node* create_heap(int size, int *heap_end);
bool insert_heap(Node *heap, int *heap_end, int letter, int freq);
void heapify(Node *heap, int *heap_end);
int get_parent(int i);
int* get_children(int i);
void swap(Node *heap, int a, int b);
Node peek(Node *heap);
Node get_min(Node *heap, int *heap_end);
bool is_leaf(int pos, int end);