#ifndef COMMON_H
#define COMMON_H

typedef struct Node_
{
    int freq;
    char ch;
    struct Node_ *left;
    struct Node_ *right;
} Node;

typedef struct PQueue_
{
    int start;
    int end;
    int size;
    Node **queue;
} PQueue;

#endif