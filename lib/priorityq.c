#ifndef PRIORITYQ_C
#define PRIORITYQ_C

#include "priorityq.h"
#include <stdio.h>
#include <stdlib.h>

pqueue *pqueue_init(int capacity, int min_capacity,
                    int (*comparator)(void *, void *)) {
  pqueue *pque = NULL;

  pque = (pqueue *)malloc(sizeof(pqueue));

  if (pque == NULL) {
    fprintf(stderr, "FATAL: Could not allocate memory for priority queue.\n");
    exit(1);
  }

  pque->capacity = capacity;
  pque->min_capacity = min_capacity;
  pque->comparator = comparator;
  pque->heap_array = malloc(capacity * sizeof(void *));

  if (pque->heap_array == NULL) {
    fprintf(stderr, "FATAL: Could not allocate memory for priority queue.\n");
    exit(1);
  }

  pque->heap_size = 0;

  for (int i = 0; i < capacity; i++)
    pque->heap_array[i] = NULL;

  return pque;
}

/*Swaps two elements of the priority queue.*/
static void swap(pqueue *pque, const int key_1, const int key_2) {
  void *tmp = NULL;
  tmp = pque->heap_array[key_1];
  pque->heap_array[key_1] = pque->heap_array[key_2];
  pque->heap_array[key_2] = tmp;
}

static void max_capacity(pqueue *pque) {
  if (pque->heap_size >= pque->capacity) {
    fprintf(stderr, "FATAL: Enqueued item out of upper bounds.\n");
    exit(1);
  }
}

static void min_capacity(pqueue *pque) {
  if (pque->heap_size < 0) {
    fprintf(stderr, "FATAL: Enqueued item out of lower bounds.\n");
    exit(1);
  }
}

void heapify_down(pqueue *pque, int key) {
  int largest = key;
  int left = 2 * key + 1;
  int right = 2 * key + 2;

  if (left < pque->heap_size &&
      pque->comparator(pque->heap_array[left], pque->heap_array[largest]) ==
          PQUEUE_GREATER)
    largest = left;
  if (right < pque->heap_size &&
      pque->comparator(pque->heap_array[right], pque->heap_array[largest]) ==
          PQUEUE_GREATER)
    largest = right;

  if (largest != key) {
    swap(pque, largest, key);
    heapify_down(pque, largest);
  }
};

void pqueue_enqueue(pqueue *pque, void *element) {

  max_capacity(pque);
  pque->heap_array[pque->heap_size] = element;
  pque->heap_size += 1;

  int parent = pque->heap_size / 2;
  int i = pque->heap_size - 1;

  while (i > 0 &&
         pque->comparator(pque->heap_array[i], pque->heap_array[parent]) ==
             PQUEUE_GREATER) {
    swap(pque, parent, i);
    i = parent;
    parent = (i - 1) / 2;
  }
}

void *pqueue_peek(pqueue *pque) { return pque->heap_array[0]; }

void *pqueue_dequeue(pqueue *pque) {
  void *element = pque->heap_array[0];
  swap(pque, 0, pque->heap_size - 1);

  pque->heap_array[pque->heap_size - 1] = NULL;
  pque->heap_size -= 1;

  min_capacity(pque);

  heapify_down(pque, 0);

  return element;
}

#endif
