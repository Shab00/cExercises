#include "queue.h"
#include <stdlib.h>

struct queue_node {
   struct queue_node *prev, *next;
   q_data_t data;
};

struct queue {
   struct queue_node *first, *last;
};

struct queue *queue_create(void) {
    struct list* newQueue = malloc(sizeof(struct queue));    
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
    
    struct queue_node* walker = list->first;
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

    newItem->prev = end;
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
    struct queue_node* beginning = list->first;
    struct queue_node* survivor;
    q_data_t localData = 0;

    if (beginning == NULL) {
        return false;
    }

    localData = beginning->data;
    survivor = beginning->next;

    if (survivor== NULL) {
        list->first = NULL;
        list->last = NULL;
    } else {
        survivor->prev = NULL;
        list->first = survivor;
    }
    
    free(beginning);

    return localData;
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

}

bool queue_is_empty(const struct queue *queue) {

}
