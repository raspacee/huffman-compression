#ifndef PRIORITY_H
#define PRIORITY_H
typedef struct PQueue_
{
    int start;
    int end;
    int size;
    Node **queue;
} PQueue;

PQueue *initialize_pqueue(int size);
bool is_full(PQueue *q);
bool is_empty(PQueue *q);
void heapify_pqueue(PQueue *q);
void swap(Node **queue, int a, int b);
int enqueue_pqueue(PQueue *q, Node *node);
Node *dequeue_pqueue(PQueue *q);
void print_pqueue(PQueue *q);
void test_pqueue();
#endif