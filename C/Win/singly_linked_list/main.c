#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>

typedef struct Node {
    int value;
    struct Node* next;
} Node;

bool list_push_front(Node** head, int value)
{
    Node* new_node = malloc(sizeof(*new_node));

    if (new_node == NULL)
        return false;

    new_node->value = value;
    new_node->next = *head;

    *head = new_node;
    return true;
}

// reports the popped value
bool list_pop_front(Node** head, int* value)
{
    if (head == NULL || *head == NULL || value == NULL)
        return false;

    Node* tmp = *head;
    *head = tmp->next;
    *value = tmp->value;
    free(tmp);

    return true;
}

bool list_push_back(Node** head, int value)
{
    if (head == NULL || *head == NULL)
        return false;

    // find last node
    Node* pLast = *head;

    while (pLast->next)
    {
        pLast = pLast->next;
    }

    // create a new node
    Node* new_node = malloc(sizeof(*new_node));

    if (new_node == NULL)
        return false;

    new_node->value = value;
    new_node->next = NULL;
    pLast->next = new_node;

    return true;
}

bool list_remove(Node** head, int value);
bool list_insert_after(Node* node, int value);
void list_reverse(Node** head);
void list_free(Node** head);
Node* list_find(Node* head, int value);

void node_print(const Node* node)
{
    if (node != NULL)
    {
        printf("Node = 0x%" PRIxPTR ", value = %d\n", (uintptr_t)node, node->value);
        node_print(node->next);
    }
}

void list_print(const Node* head)
{
    if (head == NULL)
    {
        puts("The list is empty");
    }
    else
    {
        node_print(head);
    }
}

int main()
{
    bool result;

    Node* head = NULL;  // create empty list

    // add first node
    list_print(head);
    list_push_front(&head, 10);
    list_print(head);

    // remove a node
    int tmp;

    result = list_pop_front(&head, &tmp);

    printf("pop_front result = %d\n", result);

    if (result)
    {
        printf("    value removed = %d\n", tmp);
    }
    list_print(head);

    // try push back, shall fail
    result = list_push_back(&head, 5);
    printf("list_push_back result = %d\n", result);

    // now add a node and try again
    result = list_push_front(&head, 11);
    printf("pop_front result = %d\n", result);
    result = list_push_back(&head, 5);
    printf("list_push_back result = %d\n", result);
    list_print(head);

    return 0;
}