#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    fprintf(stderr, "FATAL: Enqueued item out of bounds.\n");
    exit(1);
  }
}

static void heapify_down(pqueue *pque, int key) {
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

void enqueue(pqueue *pque, void *element) {

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

int comparator(void *a, void *b) {
  int value_1, value_2;

  value_1 = *((int *)a);
  value_2 = *((int *)b);

  if (value_1 > value_2) {
    return PQUEUE_GREATER;
  };

  if (value_1 < value_2) {
    return PQUEUE_LESS;
  };

  return PQUEUE_EQUAL;
}

int main() {
  pqueue *pq = pqueue_init(100, 2, &comparator);

  int a = 1;
  int b = 2;
  int c = 3;
  int d = 0;

  for (int i = 0; i < pq->heap_size; i++)
    printf("%d\n", *(int *)pq->heap_array[i]);
}
