#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include <stdint.h>
#include "huffman.h"
#include "priority.h"
#include "hashmap.h"
#include "common.h"

int main(int argc, char *argv[]) {
    if (argc != 3 || 
        (strcmp(argv[1], "-decomp") != 0 && strcmp(argv[1], "-comp") != 0)
    ) {
        printf("program usage not correct\nuse like this: ./huffman -[decomp or comp] [filename]\n");
        exit(-1);
    }

    if (strcmp(argv[1], "-decomp") == 0) {
        if (decompress_file(argv[2]) != 0) {
            printf("Error occured while decompressing file\n");
            exit(-1);
        }
        printf("Sucessfully decompressed the file, decompressed.txt \n");
    } else {
        FILE *fp = fopen(argv[2], "r");
        if (fp == NULL) {
            printf("ERROR: Cannot read file\n");
            exit(-1);
        }
        uint64_t total_char = 0; /* Total characters in original file */
        char c;

        /* Hashmap to store frequency of characters*/
        HashMap *freq_map = initialize_hashmap(200, FREQ_TYPE);         
        BucketData *data = NULL;

        /* Read each character one by one from file and update frequency of each character */
        while ((c = fgetc(fp)) != EOF) {
            char *ch = malloc(sizeof(char) * 2);
            ch[0] = c;
            ch[1] = '\0';
            if ((data = get_hashmap(freq_map, ch)) != NULL) {
                data->freq = data->freq + 1;
            } else {
                BucketData *f = malloc(sizeof(BucketData));
                f->freq = 1;
                insert_hashmap(freq_map, ch, f);
            }
            total_char++;
        }

        /* Create min priority queue */
        /* This will be used when creating the huffman tree */
        PQueue *q = initialize_pqueue(200);

        for (int i=0; i<freq_map->size; i++) {
            if (freq_map->buckets[i] != NULL) {
                Node *tmp = malloc(sizeof(Node));
                tmp->ch = freq_map->buckets[i]->key[0];
                tmp->freq = freq_map->buckets[i]->data->freq;
                tmp->left = NULL;
                tmp->right= NULL;
                enqueue_pqueue(q, tmp);
            }
        }

        Node *tree_root = create_htree(q);

        /* Traverse the tree and store the codes in a hashmap */

        HashMap *code_map = initialize_hashmap(100, STR_TYPE); /* Stores the letter -> codes */

        /*  Stores the codes -> letter, this map will be used to decompress a file. */
        HashMap *alph_map = initialize_hashmap(100, STR_TYPE);     

        char arr2[20];
        int top = 0;
        store_codes(code_map, alph_map, tree_root, arr2, top);

        if (compress_file(fp, code_map, alph_map, total_char) != 0) {
            printf("Some error occured while compressing the file\n");
            exit(-1);
        }
        printf("Sucessfully compressed the file, check decompressed.bj\n");

        free(code_map);
        free(freq_map);
        free(q);
        fclose(fp);
    }

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
    FILE *fp = fopen(filename, "rb");
    FILE *new_fp = fopen("decompressed.txt", "w");
    if (fp == NULL || new_fp == NULL) {
        printf("decompress_file: error opening file\n");
        exit(-1);
    }
    uint16_t two_byte;
    int flag;

    /* Read the meta data */
    uint64_t total_char;
    fread(&total_char, sizeof(uint64_t), 1, fp);
    int meta_size = fgetc(fp);
    MetaData meta[meta_size];
    fread(meta, sizeof(MetaData), meta_size, fp);

    /* Create a hashmap using the meta data, this hashmap will be used for decompressing */
    HashMap *meta_map = initialize_hashmap(meta_size, STR_TYPE);

    uint64_t total_char_read = 0;
    for (int i=0; i<meta_size; i++) {
        char *letter = malloc(sizeof(char) + 1); // hacky
        letter[0] = meta[i].alphabet;
        letter[1] = '\0';
        BucketData *data = malloc(sizeof(BucketData));
        data->string = letter;

        insert_hashmap(meta_map, meta[i].code, data);
    }

    char buf[17]; /* Buffer to store the incoming code from the file, extra space for \0 */
    int buf_index = 0;

    while ((flag = fread(&two_byte, sizeof(uint16_t), 1, fp)) > 0)
    {
        for (int i = 0; i < 16; i++)
        {
            uint8_t bit = (two_byte >> (15 - i)) & 1;
            if (bit == 0) buf[buf_index++] = '0';
            else buf[buf_index++] = '1';

            char *query = malloc(sizeof(char) * buf_index);
            if (query == NULL) {
                printf("decompress_file: failed to malloc\n");
                exit(-1);
            }
            buf[buf_index] = '\0';
            strcpy(query, buf);
            buf[buf_index] = ' ';
            BucketData *letter = get_hashmap(meta_map, query);
            if (letter != NULL) {
                fputc(letter->string[0], new_fp);
                buf_index = 0;
                total_char_read++;
            }

            free(query);
            if (buf_index == 16) buf_index = 0;
            /* Don't read waste trailing bits if all characters are decompressed*/
            if (total_char_read == total_char) break;         }
    }
    printf("\n");
    fclose(fp);
    fclose(new_fp);
    return 0;
}

/* Generate a compressed file. Returns non zero if failed. */
int compress_file(FILE *orig_file, HashMap *code_map, HashMap *alph_map, uint64_t total_char) {
    FILE *new_fp = fopen("compressed.bj", "wb");

    fwrite(&total_char, sizeof(uint64_t), 1, new_fp);
    /* Write the codes->alphabet hashmap metadata first */
    MetaData meta[alph_map->filled];
    int counter = 0;
    for (int i=0; i<alph_map->size; i++) {
        if (alph_map->buckets[i] != NULL) {
            strcpy(meta[counter].code, alph_map->buckets[i]->key);
            meta[counter].alphabet = alph_map->buckets[i]->data->string[0];
            counter++;

            if (counter == alph_map->filled) break; /* Don't remove dangerous things happen */
        }
    }
    int meta_size = alph_map->filled; /* Starting from index 1 */
    fputc(meta_size, new_fp); /* Write the metadata array size */
    fwrite(meta, sizeof(MetaData), meta_size, new_fp);

    char buf[16]; /* Buffer to store 2 bytes */
    int buf_index = -1;
    char c;
    /* Read the file from beginning */
    fseek(orig_file, 0, SEEK_SET);
    while ((c = fgetc(orig_file)) != EOF) {
        // Hacky way to convert a char to char*
        char *query = malloc(sizeof(char) * 2);
        query[0] = c;
        query[1] = '\0';

        BucketData *code = get_hashmap(code_map, query);
        if (code == NULL) {
            printf("main: key %s not found.\n", query);
            return -1;
        }

        for (size_t i=0; i<strlen(code->string); i++) {
            /* Buffer is full so write the byte to a file */
            if (buf_index == 15) {
                uint16_t two_byte = 0;
                for (int i=0; i <= 15; i++) {
                    two_byte |= buf[i] == '0' ? 0 << (15 - i) : 1 << (15 - i);
                }
                fwrite(&two_byte, sizeof(uint16_t), 1, new_fp);

                buf_index = -1; /* Reset the buffer */
            } 

            buf[++buf_index] = code->string[i];
        }
    }
    /* If any bits in the buffer is left, write it to file */
    if (buf_index >= 0) {
        uint16_t two_byte = 0;
        for (int i=0; i <= buf_index; i++) {
            two_byte |= buf[i] == '0' ? 0 << (15 - i) : 1 << (15 - i);
        }
        fwrite(&two_byte, sizeof(uint16_t), 1, new_fp);
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

        // hacky 
        char *key = malloc(sizeof(char) * 2);
        key[0] = root->ch;
        key[1] = '\0';

        insert_hashmap(code_map, key, code_data);

        BucketData *alph_data = malloc(sizeof(BucketData));
        alph_data->string = key;
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
  