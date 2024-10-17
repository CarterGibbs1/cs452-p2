#include <stdlib.h>
#include "lab.h"

typedef struct queue queue;

struct queue {
    int MAX_SIZE;
    int head;   // represents index of head node
    int tail;   // represents index of tail node
    void **data;    // represents circular array of data
};

queue_t queue_init(int capacity) {
    queue_t q = (queue_t) malloc(sizeof(queue));
    q->data = (void**) malloc(capacity * sizeof(void*));
    q->MAX_SIZE = capacity;
    q->head=-1;
    q->tail=-1;
    return q;
}

void queue_destroy(queue_t q) {
    free(q->data);
    free(q);
}

void enqueue(queue_t q, void *data) {
    /*
    edge cases:
        - no head/tail
        - too many elements
    */
    
    // if too many elements
    if (abs(q->tail - q->head) == q->MAX_SIZE) {
        return;
    }

    // if no head/tail
    if (q->head == -1) {
        q->head = 0;
    }

    int tail_idx = q->head % q->MAX_SIZE;

    q->data[tail_idx + 1] = data;
    q->tail++;
}

void *dequeue(queue_t q) {
    /*
    edge cases:
        - if no elements
        - if head == tail (only one element)
        - if head > tail (since it is circular)
    */

    // if no elements
    if (q->head == -1) {
        return NULL;
    }

    // adjust because of circular array
    int head_idx = q->head % q->MAX_SIZE;
    int tail_idx = q->tail % q->MAX_SIZE;

    // if only one element
    if (head_idx == tail_idx) {
        void *retVal = q->data[head_idx];
        q->head = -1;
        q->tail = -1;
        return retVal;
    }

    void *retVal = q->data[tail_idx];
    q->tail--;
    return retVal;
}

void queue_shutdown(queue_t q) {

}

bool is_empty(queue_t q) {
    return q->head == -1;
}

bool is_shutdown(queue_t q) {
    return false;
}