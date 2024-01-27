#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "huffman.h"
#include "priority.h"

Node* create_heap(int size, int *heap_end) {
    Node* heap = malloc(sizeof(Node) * size);
    memset(heap, 0, sizeof(Node) * size);
    *heap_end = 0;
    return heap;
}

bool insert_heap(Node *heap, int *heap_end, int letter, int freq) {
    if (*heap_end == ALPHABETS_LEN - 1) {
        printf("ERROR(insert_heap): heap is full");
        return false;
    } else {
        heap[*heap_end].freq = freq;
        heap[*heap_end].ch = letter;
        heapify(heap, heap_end);
        *heap_end = *heap_end + 1;
        return true;
    }
}

void heapify(Node *heap, int *heap_end) {
    int curr = *heap_end;

    int parent_idx = get_parent(heap, curr);
    Node parent = heap[parent_idx];
    while (curr > 0 && (parent.freq > heap[curr].freq)) {
        swap(heap, curr, parent_idx);
        curr = parent_idx;
        parent_idx = get_parent(heap, parent_idx);
    }
}

int get_parent(Node *heap, int i) {
    if (i == 0) return 0;
    else if (i > ALPHABETS_LEN) return -1;
    else {
        int pos;
        if (i % 2 == 0)
            pos = (i - 2) / 2;
        else 
            pos = (i - 1) / 2;
        return pos;     
    }
}

void swap(Node *heap, int a, int b) {
    Node tmp = {
        .ch = heap[a].ch,
        .freq = heap[a].freq
    };

    heap[a].ch = heap[b].ch;
    heap[a].freq = heap[b].freq;

    heap[b].ch = tmp.ch;
    heap[b].freq = tmp.freq;
}