#include "queue.h"
#include <stdlib.h>

struct queue_node {
   struct queue_node *next;
   q_data_t data;
};

struct queue {
   struct queue_node *first, *last;
};

struct queue *queue_create(void) {
    struct queue* newQueue = malloc(sizeof(struct queue));    
    if (newQueue == NULL) {
        return NULL;
    }

    newQueue->first = NULL;
    newQueue->last = NULL;

    return newQueue;

}


size_t queue_count(const struct queue *queue) {

    if (queue == NULL) {
        return 0;
    }
    
    struct queue_node* walker = queue->first;
    size_t count = 0;

    while (walker != NULL) {
        count++;
        walker = walker->next;
    }

    return count;
    
}

void queue_enqueue(struct queue *queue, q_data_t item_data) {

    struct queue_node* end = queue->last;
    struct queue_node* newItem = malloc(sizeof(struct queue_node));

    if (newItem == NULL) {
        return;
    }

    newItem->next = NULL;
    newItem->data = item_data;

    if (end == NULL) {
       queue->first = newItem; 
    } else {
       end->next = newItem; 
    }

    queue->last = newItem;
}



bool queue_dequeue(struct queue *queue, q_data_t *out) {
    struct queue_node* beginning = queue->first;
    struct queue_node* survivor;

    if (beginning == NULL) {
        return false;
    }

    *out = beginning->data;
    survivor = beginning->next;

    if (survivor== NULL) {
        queue->first = NULL;
        queue->last = NULL;
    } else {
        queue->first = survivor;
    }
    
    free(beginning);

    return true;
}

void queue_destroy(struct queue *queue) {

    if (queue == NULL) {
        return;
    }
    
    struct queue_node* walker = queue->first;
    struct queue_node* tracker = NULL;

    while (walker != NULL) {
        tracker = walker->next; 
        
        free(walker);            
        
        walker = tracker;   
    }
    free(queue);
}


bool queue_peek(const struct queue *queue, q_data_t *out) {

    struct queue_node* beginning = queue->first;

    if (beginning == NULL) {
        return false;
    }

    *out = beginning->data;

    return true;
}

bool queue_is_empty(const struct queue *queue) {
    if (queue->first == NULL) {
        return true;
    }

    return false;

}
