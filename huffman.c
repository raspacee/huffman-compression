#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "huffman.h"
#include "priority.h"
#include "hashmap.h"
#include "common.h"

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

    Node *tree_root = create_htree(q);

    char arr[20];
    int top = 0;
    print_codes(tree_root, arr, top);

    /* Traverse the tree and store the codes in a hashmap */

    HashMap *code_map = initialize_hashmap(100, STR_TYPE); /* Stores the letter -> codes */

    /*  Stores the codes -> letter, this map will be used to decompress a file. */
    HashMap *alph_map = initialize_hashmap(100, STR_TYPE);     

    char arr2[20];
    top = 0;
    store_codes(code_map, alph_map, tree_root, arr2, top);

    if (compress_file(fp, code_map) != 0) {
        printf("Some error occured while compressing the file\n");
        exit(-1);
    }

    if (decompress_file("compressed.bj") != 0) {
        printf("Error occured while decompressing file\n");
        exit(-1);
    }

    free(code_map);
    free(q);
    fclose(fp);
    return 0;
}

/* Create a huffman tree from a given priority queue. Returns the root node of the tree */
Node *create_htree(PQueue *q) {
    while (get_size_pqueue(q) > 1)
    {
        Node *left_node = dequeue_pqueue(q);
        Node *right_node = dequeue_pqueue(q);
        Node *new_node = malloc(sizeof(Node));
        new_node->ch = 0;
        new_node->freq = left_node->freq + right_node->freq;
        new_node->left = left_node;
        new_node->right = right_node;
        enqueue_pqueue(q, new_node);
    }
    return q->queue[0];
}

int decompress_file(char *filename) {
    FILE *tmp = fopen(filename, "rb");
    unsigned char byte;
    int flag;
    while ((flag = fread(&byte, sizeof(unsigned char), 1, tmp)) > 0)
    {
        for (int i = 0; i < 8; i++)
        {
            printf("%d", (byte & (1 << (7 - i))) >> (7 - i));
        }
    }
    printf("\n");
    return 0;
}

/* Generate a compressed file. Returns non zero if failed. */
int compress_file(FILE *orig_file, HashMap *code_map) {
    FILE *new_fp = fopen("compressed.bj", "wb");
    char buf[8]; /* Buffer to store 1 byte */
    int buf_index = -1;
    char c;
    /* Read the file from beginning */
    fseek(orig_file, 0, SEEK_SET);
    while ((c = fgetc(orig_file)) != EOF) {
        if (c == ' ') {
            continue;
        }

        // Hacky way to convert a char to char*
        char *query = malloc(sizeof(char) * 2);
        query[0] = c;
        query[1] = '\0';

        BucketData *code = get_hashmap(code_map, query);
        if (code == NULL) {
            printf("main: key %s not found.\n", query);
            return -1;
        }

        for (int i=0; i<strlen(code->string); i++) {
            /* Buffer is full so write the byte to a file */
            if (buf_index == 7) {
                unsigned char byte = 0;
                byte |= buf[0] == '0' ? 0 << 7 : 1 << 7; 
                byte |= buf[1] == '0' ? 0 << 6 : 1 << 6; 
                byte |= buf[2] == '0' ? 0 << 5 : 1 << 5; 
                byte |= buf[3] == '0' ? 0 << 4 : 1 << 4; 
                byte |= buf[4] == '0' ? 0 << 3 : 1 << 3; 
                byte |= buf[5] == '0' ? 0 << 2 : 1 << 2; 
                byte |= buf[6] == '0' ? 0 << 1 : 1 << 1; 
                byte |= buf[7] == '0' ? 0 : 1 ; 
                fwrite(&byte, sizeof(unsigned char), 1, new_fp);

                buf_index = -1; /* Reset the buffer */
            } 

            buf[++buf_index] = code->string[i];
        }
    }
    /* If any bits in the buffer is left, write it to file */
    if (buf_index >= 0) {
        unsigned char byte = 0;
        for (int i=0; i <= buf_index; i++) {
            byte |= buf[i] == '0' ? 0 << (7 - i) : 1 << (7 - i);
        }
        fwrite(&byte, sizeof(unsigned char), 1, new_fp);
    }
    fclose(new_fp);

    return 0;
}

void store_codes(HashMap *code_map, HashMap *alph_map, Node *root, char arr[], int top) {
    if (root->left) { 
        arr[top] = '0'; 
        store_codes(code_map, alph_map, root->left, arr, top + 1); 
    } 
  
    if (root->right) { 
        arr[top] = '1'; 
        store_codes(code_map, alph_map, root->right, arr, top + 1); 
    } 
  
    /* Leaf node reached */
    if (root->ch > 0) { 
        BucketData *code_data = malloc(sizeof(BucketData));
        char *code = malloc(sizeof(char) * top + 1);
        code[strlen(code) - 1] = '\0';
        strncpy(code, arr, sizeof(char) * top);
        code_data->string = code;

        //hacky
        char *key = malloc(sizeof(char) * 2);
        key[0] = root->ch;
        key[1] = '\0';

        insert_hashmap(code_map, key, code_data);

        BucketData *alph_data = malloc(sizeof(BucketData));
        alph_data->string = key;
        printf("code %s, letter %s\n", code, key);
        insert_hashmap(alph_map, code, alph_data);
    } 
}

void print_codes(Node *root, char arr[], int top) { 
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
  