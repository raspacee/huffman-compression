#ifndef HUFFMAN_H
#define HUFFMAN_H
typedef struct Node_ {
    int freq;
    char ch;
    struct Node_ *left;
    struct Node_ *right;
} Node;

#define ALPHABETS_LEN 26
#define STACK_SIZE 20

typedef struct Stack_ {
    char stk[STACK_SIZE];
    int tos;
} Stack;

void traverse(Node *node, Stack *stack);
void push_stack(Stack *stack, char ch);
char pop_stack(Stack *stack);
char peek_stack(Stack *stack);
void get_string(Stack *stack, char *code);
#endif