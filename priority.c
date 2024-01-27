#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "huffman.h"
#include "priority.h"

Node* create_heap(int size, int *heap_end) {
    Node* heap = (Node *) malloc(sizeof(Node) * size);
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

    int parent_idx = get_parent(curr);
    Node parent = heap[parent_idx];
    while (curr > 0 && (parent.freq > heap[curr].freq) && curr != parent_idx) {
        swap(heap, curr, parent_idx);
        curr = parent_idx;
        parent_idx = get_parent(parent_idx);
        parent = heap[parent_idx];
    }
}

int get_parent(int i) {
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

int* get_children(int i) {
    static int c[2];
    c[0] = i * 2 + 1;
    c[1] = i * 2 + 2;
    return c;
}

void swap(Node *heap, int a, int b) {
    Node tmp = heap[a];
    heap[a] = heap[b];
    heap[b] = tmp;
}

Node peek(Node *heap) {
    return heap[0];
}

Node get_min(Node *heap, int *heap_end) {
    if (*heap_end == 0) {
        printf("ERROR: heap already empty\n");
        return;
    }

    Node min = {
        .ch = heap[0].ch,
        .freq = heap[0].freq
    };

    int tmp_end = *heap_end - 1;
    swap(heap, 0, tmp_end);
    *heap_end = tmp_end;
    tmp_end--;

    int i = 0;
    int child1, child2;
    child1 = get_children(i)[0];
    child2 = get_children(i)[1];
    while (!is_leaf(i, tmp_end) && (heap[i].freq > heap[child1].freq || heap[i].freq > heap[child2].freq)) {
        if (heap[i].freq > heap[child1].freq && child1 <= tmp_end) {
            swap(heap, i, child1);
            i = child1;
        } else if (heap[i].freq > heap[child2].freq && child2 <= tmp_end) {
            swap(heap, i, child2);
            i = child2;
        } else {
            printf("ERROR(get_min): child out of bounds.\n");
            exit(-1);
        }
        child1 = get_children(i)[0];
        child2 = get_children(i)[1];
    }
    return min;
}

bool is_leaf(int pos, int end) {
    int child1 = get_children(pos)[0];
    int child2 = get_children(pos)[1];
    return (child1 > end && child2 > end);
}