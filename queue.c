#include "queue.h"

void queue_init(queue_t* queue)
{
    queue->head = NULL;
    queue->tail = NULL;
}

bool queue_is_empty(queue_t* queue)
{
    return queue->head == NULL;
}

pair_t queue_head(queue_t* queue)
{
    return queue->head->data;
}

void queue_enqueue(queue_t* queue, pair_t data)
{
    node_t* node;
    node = (node_t*)malloc(sizeof(node_t));
    node->data = data;
    node->next = NULL;

    if (queue_is_empty(queue)) {
        queue->head = node;
        queue->tail = node;
    } else {
        queue->tail->next = node;
        queue->tail = node;
    }
}

pair_t queue_dequeue(queue_t* queue)
{
    node_t* node;
    pair_t pair;

    node = queue->head;
    pair = node->data;

    if (queue->head == queue->tail) {
        queue->head = NULL;
        queue->tail = NULL;
    } else {
        queue->head = queue->head->next;
    }

    free(node);
    return pair;
}
