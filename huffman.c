#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "huffman.h"
#include "priority.h"

int main() {
    int frequency[ALPHABETS_LEN] = {0};
    char c;
    FILE *fp = fopen("text", "r");
    if (fp == NULL) {
        printf("ERROR: Cannot read file\n");
        exit(-1);
    }

    while ((c = fgetc(fp)) != EOF) {
        c = tolower(c);
        short int index = c - 97;
        frequency[index]++;
    }

    // printf("-----Frequency table------");
    // for (int i = 0; i < (sizeof(frequency)/sizeof(frequency[0])); i++) {
    //     printf("%c - %d\n", i + 97, frequency[i]);
    // }

    int heap_end;
    Node* heap = create_heap(ALPHABETS_LEN, &heap_end);
    insert_heap(heap, &heap_end, 'b', 9);
    insert_heap(heap, &heap_end, 'z', 5);
    insert_heap(heap, &heap_end, 'e', 19);
    insert_heap(heap, &heap_end, 'p', 1);
    for (int i=0; i<heap_end; i++) {
        printf("%c - %d\n", heap[i].ch, heap[i].freq);
    }

    // for (int i=0; i<ALPHABETS_LEN; i++) {
    //     printf("%d - %c\n", heap[i].freq, heap[i].ch);
    // }
    free(heap);
    fclose(fp);
    return 0;
}