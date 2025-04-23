#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int val;
    struct node * next;
} node_t;

void print_list(node_t * head) {
    node_t * current = head;

    while (current != NULL) {
        printf("%d\n", current->val);
        current = current->next;
    }
}

int pop(node_t ** head) {
    int retval = -1;
    node_t * next_node = NULL;

    if (*head == NULL) {
        return -1;
    }

    next_node = (*head)->next;
    retval = (*head)->val;
    free(*head);
    *head = next_node;

    return retval;
}

int remove_by_value(node_t ** head, int val) {
    /* TODO: fill in your code here */

    if (*head == NULL) {
        return -1;
    }

    node_t *current = *head;
    node_t *temp_node = NULL;

    if (current->val == val) {
        return pop(head); // Use the existing pop function
    }

    // Traverse the list to find the node with the value
    while (current->next != NULL && current->next->val != val) {
        current = current->next;
    }

    if (current->next == NULL) {
        // Value not found in the list
        return -1;
    }

    // Remove the node
    temp_node = current->next;        // Node to remove
    current->next = temp_node->next;  // Bypass the node
    free(temp_node);                  // Free memory

    return 0; // Removal successful
}

int main() {

    node_t * test_list = (node_t *) malloc(sizeof(node_t));
    test_list->val = 1;
    test_list->next = (node_t *) malloc(sizeof(node_t));
    test_list->next->val = 2;
    test_list->next->next = (node_t *) malloc(sizeof(node_t));
    test_list->next->next->val = 3;
    test_list->next->next->next = (node_t *) malloc(sizeof(node_t));
    test_list->next->next->next->val = 4;
    test_list->next->next->next->next = NULL;

    remove_by_value(&test_list, 3);

    print_list(test_list);
}
