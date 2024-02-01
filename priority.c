#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "priority.h"
#include "huffman.h"

#define PRINT_NODE(d) printf("Node --> freq -> %d, ch -> %c\n\n", d->freq, d->ch);

int main(void) {
    test_pqueue();
    return 0;
}

PQueue *initialize_pqueue(int size) {
    Node **q = (Node **) malloc(size * sizeof(Node *));
    PQueue *queue = malloc(sizeof(PQueue));
    queue->start = -1;
    queue->end = -1;
    queue->size = size;
    queue->queue = q;

    return queue;
}

/* Returns non-zero value if failed */
int enqueue_pqueue(PQueue *q, Node *node) {
    if (is_full(q)) {
        printf("enqueue_pqueue: queue size is already reached, cannot add\n");
        return -1;
    }

    if (is_empty(q)) {
        q->start = q->end = 0;
    } else {
        q->end = q->end + 1;
    }

    q->queue[q->end] = node;
    heapify_pqueue(q);
    return 0;
}

Node *dequeue_pqueue(PQueue *q) {
    if (is_empty(q)) {
        printf("dequeue_pqueue: queue is empty, cannot remove\n");
    }

    if (q->end == 0) {
        int tmp = q->start;
        q->start = q->end = -1;
        return q->queue[tmp];
    } else {
        Node *top = q->queue[q->start];

        q->queue[q->start] = q->queue[q->end];
        q->end = q->end - 1;

        // No need to heapify as there is only one element left
        if (q->end == 0) return top;

        int total_el;
        total_el = q->end;

        for (int i=0; i < total_el; i++) {
            int child_1 = 2 * i + 1;
            int child_2 = 2 * i + 2;

            if (child_1 <= total_el && q->queue[i]->freq < q->queue[child_1]->freq && q->queue[child_1]->freq > q->queue[child_2]->freq) {
                swap(q->queue, i, child_1);
                i = child_1;
            } else if (child_2 <= total_el && q->queue[i]->freq < q->queue[child_2]->freq) {
                swap(q->queue, i, child_2);
                i = child_2;
            } else {
                break;
            }
        }

        return top;
    }
}

/* Start from the last leaf node and check heapify it with its parent */
void heapify_pqueue(PQueue *q) {
    if (q->start == q->end) {
        return;
    } else {
        int total_el = q->end;

        for (int i=total_el; i > 0; i--){
            int parent = (i - 1) / 2;
            if (q->queue[i]->freq > q->queue[parent]->freq) {
                swap(q->queue, i, parent);
                i = (i - 1) / 2;
            } else {
                break;
            }
        }
    }

}

void swap(Node **queue, int a, int b) {
    Node *tmp = queue[a];
    queue[a] = queue[b];
    queue[b] = tmp;
}

bool is_full(PQueue *q) {
    if ((q->start == 0 && q->end == q->size - 1) || (q->end + 1 == q->start)) return true;
    return false;
}

bool is_empty(PQueue *q) {
    if (q->start == -1 && q->end == -1) return true;
    return false;
}

void print_pqueue(PQueue *q) {
    printf("PQueue size -> %d, start -> %d, end -> %d\n", q->size, q->start, q->end);
    for (int i=0; i<=q->end; i++) {
        printf("Node %d - Frequency -> %d, Char -> %c, left -> %p, right -> %p\n", i, q->queue[i]->freq, q->queue[i]->ch,
        q->queue[i]->left, q->queue[i]->right);
    }
}

void test_pqueue() {
    PQueue *myQueue = initialize_pqueue(10);
    print_pqueue(myQueue);
    Node n = {
        .freq = 2,
        .ch = 'z',
        .left = NULL,
        .right = NULL
    };

    Node n1 = {
        .freq = 7,
        .ch = 'a',
        .left = NULL,
        .right = NULL
    };

    Node n3 = {
        .freq = 26,
        .ch = 'z',
        .left = NULL,
        .right = NULL
    };

    Node n4 = {
        .freq = 25,
        .ch = 'z',
        .left = NULL,
        .right = NULL
    };
    Node n5 = {
        .freq = 19,
        .ch = 'z',
        .left = NULL,
        .right = NULL
    };
    Node n6 = {
        .freq = 17,
        .ch = 'z',
        .left = NULL,
        .right = NULL
    };
    enqueue_pqueue(myQueue, &n);
    enqueue_pqueue(myQueue, &n1);
    enqueue_pqueue(myQueue, &n3);
    enqueue_pqueue(myQueue, &n4);
    enqueue_pqueue(myQueue, &n5);
    enqueue_pqueue(myQueue, &n6);
    Node *d1 = dequeue_pqueue(myQueue);
    Node *d2 = dequeue_pqueue(myQueue);
    Node *d3 = dequeue_pqueue(myQueue);
    Node *d4 = dequeue_pqueue(myQueue);
    Node *d5 = dequeue_pqueue(myQueue);
    Node *d6 = dequeue_pqueue(myQueue);
    assert(d1->freq == 26);
    assert(d2->freq == 25);
    assert(d3->freq == 19);
    assert(d4->freq == 17);
    assert(d5->freq == 7);
    assert(d6->freq == 2);
}