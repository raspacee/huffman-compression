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

    PQueue *q = initialize_pqueue(30);

    for (int i=0; i<ALPHABETS_LEN; i++) {
        if (frequency[i] > 0) {
            Node *tmp = malloc(sizeof(Node));
            tmp->ch = i + 97;
            tmp->freq = frequency[i];
            tmp->left = NULL;
            tmp->right= NULL;
            enqueue_pqueue(q, tmp);
        }
    }

    while (get_size_pqueue(q) > 1) {
        Node *left_node = dequeue_pqueue(q);
        Node *right_node = dequeue_pqueue(q);
        Node *new_node = malloc(sizeof(Node));
        new_node->ch = 0;
        new_node->freq = left_node->freq + right_node->freq;
        new_node->left = left_node;
        new_node->right = right_node;
        enqueue_pqueue(q, new_node);
    }

    int arr[20];
    int top = 0;
    print_codes(q->queue[0], arr, top);

    free(q);
    fclose(fp);
    return 0;
}

void print_codes(Node *root, int arr[], int top) 
  
{ 
    if (root->left) { 
        arr[top] = 0; 
        print_codes(root->left, arr, top + 1); 
    } 
  
    if (root->right) { 
        arr[top] = 1; 
        print_codes(root->right, arr, top + 1); 
    } 
  
    if (root->ch > 0) { 
        printf("code for %c is : ", root->ch); 

        for (int i=0; i<top; i++) {
            printf("%d", arr[i]);
        }
        printf("\n");
    } 
} 
  