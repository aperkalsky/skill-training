#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef int (*CompareFn)(const void*, const void*);

typedef struct TreeNode {
    int value;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

int compare_int(const void* a, const void* b)
{
    int va = *(const int*)a;
    int vb = *(const int*)b;

    if (va < vb)
        return -1;

    if (va > vb)
        return 1;

    return 0;
}

int* bst_find(TreeNode* root, int key, CompareFn compare)
{
    if (root == NULL)
    {
        return NULL;
    }

    int compare_result = compare_int((void*)&key, (void*)&(root)->value);

    if (compare_result == 0)
    {
        return &root->value;
    }

    if (compare_result == -1)
    {
        return bst_find(root->left, key, compare_int);
    }
    else
    {
        return bst_find(root->right, key, compare_int);
    }
}

void bst_destroy(TreeNode* root)
{
    if (root == NULL)
        return;

    bst_destroy(root->left);
    bst_destroy(root->right);

    printf("Destroying node %d\n", root->value);
    free(root);
}

void bst_print(TreeNode* root)
{
    if (root == NULL)
        return;

    printf("Node = %d\n", root->value);

    bst_print(root->left);
    bst_print(root->right);
}

bool bst_insert(TreeNode** root, int value, CompareFn compare)
{
    if (*root == NULL)
    {
        TreeNode* new_node = malloc(sizeof(*new_node));
        if (new_node == NULL)
            return false;

        new_node->value = value;
        new_node->left = NULL;
        new_node->right = NULL;

        *root = new_node;
        return true;
    }

    int compare_result = compare_int((void*)&value, (void*)&(*root)->value);

    if (compare_result == 0)
    {
        return false;   // duplicate
    }

    if (compare_result == -1)
    {
        return bst_insert(&(*root)->left, value, compare_int);
    }
    else
    {
        return bst_insert(&(*root)->right, value, compare_int);
    }
}

int main()
{ 
    TreeNode* root = NULL;

    bst_insert(&root, 50, compare_int);
    bst_insert(&root, 30, compare_int);
    bst_insert(&root, 70, compare_int);

    int val_to_find = 20;
    int* result = bst_find(root, val_to_find, compare_int);

    if (result != NULL)
    {
        printf("Search for %d, result = %d\n", val_to_find, *result);
    }
    else
    {
        printf("Search for %d: not found\n", val_to_find);
    }

    val_to_find = 70;
    result = bst_find(root, val_to_find, compare_int);

    if (result != NULL)
    {
        printf("Search for %d, result = %d\n", val_to_find, *result);
    }
    else
    {
        printf("Search for %d: not found\n", val_to_find);
    }

    bst_print(root);

    bst_destroy(root);

    return 0;
}