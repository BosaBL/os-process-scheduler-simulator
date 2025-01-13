#ifndef QUEUE_H
#define QUEUE_H

typedef struct node {
  void *data;
  struct node *next;
} node;

typedef struct qqueue_t {
  node *front, *rear;
} qqueue;

qqueue *qqueue_init(int capacity);

int isEmpty(qqueue *q);

void qqueue_enqueue(qqueue *pque, void *element);

void *qqueue_peek(qqueue *pque);

void *qqueue_dequeue(qqueue *pque);

void deallocate_qqueue(qqueue *pque);

#endif
