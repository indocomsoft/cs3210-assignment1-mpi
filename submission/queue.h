#ifndef QUEUE_DEFINED
#define QUEUE_DEFINED

typedef struct raw_pair_t pair_t;
typedef struct raw_node_t node_t;
typedef struct raw_queue_t queue_t;

#include "train.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

struct raw_pair_t {
    int time;
    train_t train;
};

struct raw_node_t {
    pair_t data;
    node_t* next;
};

struct raw_queue_t {
    node_t* head;
    node_t* tail;
};

void queue_init(queue_t* queue);
bool queue_is_empty(queue_t* queue);
void queue_enqueue(queue_t* queue, pair_t data);

/**
 * These 2 functions will segfault if queue is empty.
 * Check first using bool queue_is_empty(queue_t*);
 */
pair_t queue_head(queue_t* queue);
pair_t queue_dequeue(queue_t* queue);

#endif
