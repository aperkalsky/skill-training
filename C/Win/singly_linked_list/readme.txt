Singly linked list with pointer-to-pointer

Implement:

typedef struct Node {
    int value;
    struct Node *next;
} Node;

Functions:

bool list_push_front(Node **head, int value);
bool list_push_back(Node **head, int value);
bool list_remove(Node **head, int value);
bool list_insert_after(Node *node, int value);
void list_reverse(Node **head);
void list_free(Node **head);

Extra challenge: implement list_remove() using a pointer-to-pointer rather than maintaining a previous pointer.

For example, understand the difference between:

Node *current;
Node *previous;

and:

Node **link;

This is an excellent exercise for understanding why T ** is useful in C.