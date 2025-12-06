#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct node {
    char *question;
    struct node *no;
    struct node *yes;
} node;

int yes_no(char *question)
{
    char answer[10];
    printf("%s? (y/n): ", question);
    fgets(answer, sizeof(answer), stdin);
    // flush extra input
    if (strchr(answer, '\n') == NULL) {
        int c; while ((c = getchar()) != '\n' && c != EOF);
    }
    return answer[0] == 'y';
}

node* create(char *question)
{
    node *n = malloc(sizeof(node));
    n->question = strdup(question);
    n->no = NULL;
    n->yes = NULL;
    return n;
}

void release(node *n)
{
    if (n) {
        if (n->no)
            release(n->no);
        if (n->yes)
            release(n->yes);
        if (n->question)
            free(n->question);
        free(n);
    }
}

int main()
{
    char question[80];
    char suspect[20];
    node *start_node = create("Does suspect have a mustache");
    start_node->no = create("Loretta Barnsworth");
    start_node->yes = create("Vinny the spoon");

    node *current;
    do {
        current = start_node;
        while(1) {
            if (yes_no(current->question))
            {
                if (current->yes) {
                    current = current->yes;
                } else {
                    printf("SUSPECT IDENTIFIED\n");
                    break;
                }
            } else if (current->no) {
                current = current->no;
            } else {
                // Make the yes-node the new suspect name
                printf("Who's the suspect? ");
                fgets(suspect, sizeof(suspect), stdin);
                suspect[strcspn(suspect, "\n")] = 0;

                node *yes_node = create(suspect);
                current->yes = yes_node;

                // Make the no-node a copy of this question
                node *no_node = create(current->question);
                current->no = no_node;

                // Then replace this question with the new question
                printf("Give me a question that is TRUE for %s but not for %s? ", suspect, current->question);
                char new_question[80];
                fgets(new_question, sizeof(new_question), stdin);
                new_question[strcspn(new_question, "\n")] = 0;
                free(current->question);
                current->question = strdup(new_question);

                break;
            }
        }
    } while(yes_no("Run again"));
    release(start_node);
    return 0;
}
