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

    int heap_end;
    Node* heap = create_heap(ALPHABETS_LEN, &heap_end);
    for (int i=0; i<ALPHABETS_LEN; i++) {
        if (frequency[i] > 0) {
            insert_heap(heap, &heap_end, i + 97, frequency[i], NULL, NULL);
        }
    }

    while (heap_end >= 2) {
        Node a = get_min(heap, &heap_end);
        Node b = get_min(heap, &heap_end);
        insert_heap(heap, &heap_end, 0, a.freq + b.freq, &a, &b);
    }

    Stack stack;
    stack.tos = -1;
    traverse(&heap[0], &stack);

    free(heap);
    fclose(fp);
    return 0;
}

void traverse(Node *node, Stack *stack) {
    // if (node == NULL) return;
    // if (node->left == NULL && node->right == NULL) {
    //     char code[30];
    //     get_string(stack, code);
    //     printf("Symbol %c has code %s\n", node->ch, code);
    // } else {
    //     push_stack(stack, '0');
    //     traverse(node->left, stack);
    //     pop_stack(stack);

    //     push_stack(stack, '1');
    //     traverse(node->right, stack);
    //     pop_stack(stack);
    // }
    if (node == NULL) return;
    printf("%c\n", node->ch);
    traverse(node->left, stack);
    traverse(node->right, stack);
}

void push_stack(Stack *stack, char ch) {
    if (stack->tos >= STACK_SIZE) {
        printf("ERROR(add_stack): stack size limit exceeded.\n");
        exit(-1);
    }
    stack->tos = stack->tos + 1;
    stack->stk[stack->tos] = ch;
}

char pop_stack(Stack *stack) {
    if (stack->tos >= 0) {
        int tmp = stack->tos;
        stack->tos = stack->tos - 1;
        return stack->stk[tmp];
    } else {
        printf("Stack is empty cannot pop.\n");
        return;
    }
}

char peek_stack(Stack *stack) {
    return stack->stk[stack->tos - 1];
}

void get_string(Stack *stack, char *code) {
    int tmp = stack->tos;

    for (tmp; tmp >= 0; tmp--) {
        code[tmp] = stack->stk[tmp];
    }
}