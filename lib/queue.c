#ifndef QUEUE_C
#define QUEUE_C

#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

static node *createNode(void *new_data) {
  node *new_node = (node *)malloc(sizeof(node));
  new_node->data = new_data;
  new_node->next = NULL;
  return new_node;
}

qqueue *qqueue_init(int capacity) {
  qqueue *pque = NULL;

  pque = (qqueue *)malloc(sizeof(qqueue));

  if (pque == NULL) {
    fprintf(stderr, "FATAL: Could not allocate memory for priority queue.\n");
    exit(1);
  }

  pque->front = pque->rear = NULL;

  return pque;
}

int isEmpty(qqueue *q) {

  if (q->front == NULL && q->rear == NULL) {
    return 1;
  }

  return 0;
}

void qqueue_enqueue(qqueue *q, void *new_data) {
  // Create a new linked list node
  node *new_node = createNode(new_data);

  // If queue is empty, the new node is both the front
  // and rear
  if (q->rear == NULL) {
    q->front = q->rear = new_node;
    return;
  }

  // Add the new node at the end of the queue and
  // change rear
  q->rear->next = new_node;
  q->rear = new_node;
}

void *qqueue_dequeue(qqueue *q) {
  if (isEmpty(q)) {
    printf("Queue Underflow\n");
    return NULL;
  }

  node *temp = q->front;
  void *data = temp->data;

  q->front = q->front->next;

  // If front becomes null, then change rear also
  // to null
  if (q->front == NULL)
    q->rear = NULL;

  // Deallocate memory of the old front node
  free(temp);

  return data;
}

void *qqueue_peek(qqueue *q) {
  // Checking if the queue is empty
  if (isEmpty(q)) {
    printf("Queue is empty\n");
  }
  return q->front->data;
}

void deallocate_qqueue(qqueue *pque) {}

#endif
