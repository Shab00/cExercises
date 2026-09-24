#include "linked_list.h"
#include <stdlib.h>

struct list_node {
   struct list_node *prev, *next;
   ll_data_t data;
};

struct list {
   struct list_node *first, *last;
};

struct list *list_create(void) {
    struct list* newList = malloc(sizeof(struct list));    
    if (newList == NULL) {
        return NULL;
    }

    newList->first = NULL;
    newList->last = NULL;

    return newList;

}


size_t list_count(const struct list *list) {

    if (list == NULL) {
        return 0;
    }
    
    struct list_node* walker = list->first;
    size_t count = 0;

    while (walker != NULL) {
        count++;
        walker = walker->next;
    }

    return count;
    
}

void list_push(struct list *list, ll_data_t item_data) {

    struct list_node* end = list->last;
    struct list_node* newItem = malloc(sizeof(struct list_node));

    newItem->prev = end;
    newItem->next = NULL;
    newItem->data = item_data;

    if (end == NULL) {
       list->first = newItem; 
    } else {
       end->next = newItem; 
    }

    list->last = newItem;
}

ll_data_t list_pop(struct list *list) {

    struct list_node* end = list->last;
    struct list_node* survivor;
    ll_data_t localData = 0;

    if (end == NULL) {
        return -1;
    }

    localData = end->data;
    survivor = end->prev;

    if (survivor== NULL) {
        list->first = NULL;
        list->last = NULL;
    } else {
        survivor->next = NULL;
        list->last = survivor;
    }
    
    free(end);

    return localData;
}


void list_unshift(struct list *list, ll_data_t item_data) {

    struct list_node* beginning = list->first;
    struct list_node* newItem = malloc(sizeof(struct list_node));

    newItem->prev = NULL;
    newItem->next = beginning;
    newItem->data = item_data;

    if (beginning == NULL) {
       list->last = newItem; 
    } else {
       beginning->prev = newItem; 
    }

    list->first = newItem;
}


ll_data_t list_shift(struct list *list) {
    struct list_node* beginning = list->first;
    struct list_node* survivor;
    ll_data_t localData = 0;

    if (beginning == NULL) {
        return -1;
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


void list_delete(struct list *list, ll_data_t data) {

    struct list_node* walker = list->first;
    struct list_node* tracker = NULL;
    while (walker != NULL) {
        if (walker->data == data) {
            if (tracker != NULL) {
                tracker->next = walker->next;
            } else {
                list->first = walker->next;
            }

            if (walker->next != NULL) {
                walker->next->prev = tracker;
            } else {
                list->last = tracker;
            }

            free(walker);
            return;
        } else {
            tracker = walker;
            walker = walker->next;

        }
    }
}

void list_destroy(struct list *list) {

    if (list == NULL) {
        return;
    }
    
    struct list_node* walker = list->first;
    struct list_node* tracker = NULL;

    while (walker != NULL) {
        tracker = walker->next; 
        
        free(walker);            
        
        walker = tracker;   
    }
    free(list);
}
