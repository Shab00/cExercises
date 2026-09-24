#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>
#include <stdbool.h>

typedef int q_data_t;
struct queue;

// constructs a new (empty) queue
struct queue *queue_create(void);

// counts the items on a queue
size_t queue_count(const struct queue *queue);

// inserts item at back of a queue
void queue_enqueue(struct queue *queue, q_data_t item_data);

// removes item from front of a queue

bool queue_dequeue(struct queue *queue, q_data_t *out);

// destroys an entire queue
// queue will be a dangling pointer after calling this method on it
void queue_destroy(struct queue *queue);

bool queue_peek(const struct queue *queue, q_data_t *out);

bool queue_is_empty(const struct queue *queue);

#endif
