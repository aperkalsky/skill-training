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

// in addition it reports the popped value
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

// at least one node shall exist
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

// remove the node that contains the specified value
bool list_remove(Node** head, int value)
{
    Node** current = head;

    while (*current != NULL)
    {
        if ((*current)->value == value)
        {
            Node* tmp = *current;

            *current = tmp->next;
            free(tmp);

            return true;
        }

        current = &(*current)->next;
    }

    return false;
}

// insert a new node with specified value
bool list_insert_after(Node* node, int value)
{
    Node* new_node = malloc(sizeof(*new_node));

    if (new_node == NULL)
        return false;

    new_node->value = value;
    new_node->next = node->next;
    node->next = new_node;

    return true;
}

void list_reverse(Node** head)
{
    Node* previous = NULL;
    Node* current = *head;
    Node* next;

    while (current != NULL)
    {
        next = current->next;
        current->next = previous;
        previous = current;
        current = next;
    }

    *head = previous;
}

void list_free(Node** head)
{
    Node** current = head;

    while (*current != NULL)
    {
        Node* tmp = *current;

        *current = tmp->next;
        free(tmp);
    }
}

Node* list_find(Node* head, int value)
{
    Node** current = &head;

    while (*current != NULL)
    {
        if ((*current)->value == value)
        {
            return *current;
        }
        current = &(*current)->next;
    }

    return NULL;
}

void list_print(const Node* head)
{
    if (head == NULL)
    {
        puts("The list is empty");
    }

    Node** current = &head;

    while (*current != NULL)
    {
        printf("Node = 0x%" PRIxPTR ", value = %d, next = 0x%" PRIxPTR "\n", (uintptr_t)(*current), (*current)->value, (uintptr_t)((*current)->next));
        current = &(*current)->next;
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
    printf("list_push_front result = %d\n", result);
    result = list_push_back(&head, 5);
    printf("list_push_back result = %d\n", result);
    list_print(head);

    // try removing non-existent value
    result = list_remove(&head, 111);
    printf("list_remove result = %d\n", result);

    // now remove real one
    result = list_remove(&head, 5);
    printf("list_remove result = %d\n", result);
    list_print(head);

    // add it back
    result = list_push_back(&head, 5);
    printf("list_push_back result = %d\n", result);
    list_print(head);

    // search for it
    Node* pNode = list_find(head, 5);
    if (pNode)
    {
        printf("Found node = 0x%" PRIxPTR ", value = %d\n", (uintptr_t)pNode, pNode->value);
    }
    else
    {
        puts("Node not found");
    }

    // test free list
    puts("Call list_free");
    list_free(&head);
    list_print(head);

    // fill the list
    result = list_push_front(&head, 1);
    printf("list_push_front result = %d\n", result);
    result = list_push_back(&head, 2);
    printf("list_push_back result = %d\n", result);
    result = list_push_back(&head, 3);
    printf("list_push_back result = %d\n", result);
    list_print(head);

    // get pointer to the node
    pNode = list_find(head, 2);
    if (pNode)
    {
        printf("Found node = 0x%" PRIxPTR ", value = %d\n", (uintptr_t)pNode, pNode->value);
    }
    else
    {
        puts("Node not found");
    }

    // call insert after
    result = list_insert_after(pNode, 4);
    printf("list_insert_after result = %d\n", result);
    list_print(head);

    // call list reverse
    puts("Reversing a list");
    result = list_reverse(&head);
    list_print(head);

    return 0;
}