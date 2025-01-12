#ifndef PRIORITYQ_H
#define PRIORITYQ_H

#define PQUEUE_GREATER 1
#define PQUEUE_EQUAL 0
#define PQUEUE_LESS -1

typedef struct pqueue_t {
  int (*comparator)(void *, void *);
  int min_capacity;
  int capacity;
  void **heap_array;
  int heap_size;
} pqueue;

pqueue *pqueue_init(int capacity, int min_capacity,
                    int (*comparator)(void *, void *));

void pqueue_enqueue(pqueue *pque, void *element);

void *pqueue_peek(pqueue *pque);

void *pqueue_dequeue(pqueue *pque);

void heapify_down(pqueue *pque, int key);

void deallocate_pqueue(pqueue *pque);

#endif
