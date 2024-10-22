#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "lab.h"

typedef struct queue queue;
typedef struct node node;

struct node {
    void *data;
    node *next;
};

struct queue {
    int MAX_SIZE;
    int curr_size;
    node *head;   // represents head node
    node *tail;   // represents tail node
    //void **data;    // represents circular array of data
    bool shutdown;
    pthread_mutex_t mutex;   // mutex
};

queue_t queue_init(int capacity) {
    queue_t q = (queue_t) malloc(sizeof(queue));
    //q->data = (void**) malloc(capacity * sizeof(void*));
    q->MAX_SIZE = capacity;
    q->curr_size = 0;
    q->head=NULL;
    q->tail=NULL;
    q->shutdown = false;
    pthread_mutex_init(&q->mutex, NULL);
    return q;
}

void queue_destroy(queue_t q) {
    while (!is_empty(q)) {
        void *data = dequeue(q);
        free(data);
    }
    free(q);
}

void enqueue(queue_t q, void *data) {
    /*
    edge cases:
        - no head/tail
        - too many elements
    */

    // Needs to wait for some condition, cond is placeholder for now
    /**
    while (q->curr_size == q->MAX_SIZE && !q->shutdown) {
        pthread_cond_wait(&q->cond, &q->mutex);
    }
     */

    pthread_mutex_lock(&(q->mutex));
    // if too many elements
    /**if (q->curr_size == q->MAX_SIZE || q->shutdown) {
        pthread_mutex_unlock(&(q->mutex));
        return;
    } */

    

    node *newNode = (node *) malloc(sizeof(node));
    newNode->data = data;
    newNode->next = NULL;

    // if no elements
    if (q->curr_size == 0) {
        q->head = newNode;
        q->tail = newNode;
    } else {
        (q->tail)->next = newNode;
        q->tail = newNode;
    }
    q->curr_size++;
    pthread_mutex_unlock(&(q->mutex));
}

void *dequeue(queue_t q) {
    /*
    edge cases:
        - if no elements
        - if head == tail (only one element)
        - if head > tail (since it is circular)
    */

    // Needs to wait for condition
    while (is_empty(q) && !q->shutdown) {
        pthread_cond_wait(&q->cond, &q->mutex);
    }
    if (is_empty(q) && q->shutdown) {
        pthread_mutex_unlock(&(q->mutex));
        return NULL;
    }

    pthread_mutex_lock(&(q->mutex));

    // if only one element
    if (q->curr_size == 1) {
        node *retNode = q->head;
        void *retVal = retNode->data;
        q->head = NULL;
        q->tail = NULL;
        q->curr_size--;
        free(retNode);
        pthread_mutex_unlock(&(q->mutex));
        return retVal;
    }

    node *retNode = q->head;
    q->head = retNode->next;
    q->curr_size--;
    void *retVal = retNode->data;
    free(retNode);
    pthread_mutex_unlock(&(q->mutex));
    return retVal;
}

void queue_shutdown(queue_t q) {
    pthread_mutex_lock(&(q->mutex));
    q->shutdown = true;
    pthread_mutex_unlock(&(q->mutex));
}

bool is_empty(queue_t q) {
    return q->curr_size == 0;
}

bool is_shutdown(queue_t q) {
    return q->shutdown;
}