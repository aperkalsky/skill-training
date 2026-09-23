#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

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

bool list_push_back(Node** head, int value);
bool list_remove(Node** head, int value);
bool list_insert_after(Node* node, int value);
void list_reverse(Node** head);
void list_free(Node** head);
Node* list_find(Node* head, int value);

void node_print(const Node* node)
{
    if (node != NULL)
    {
        printf("Node = 0x%I64X, value = %d\n", (uint64_t)node, node->value);
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

    list_print(head);
    list_push_front(&head, 10);
    list_print(head);

    int tmp;

    result = list_pop_front(&head, &tmp);

    printf("pop_front result = %d, value removed = %d\n", result, tmp);
    list_print(head);

    return 0;
}