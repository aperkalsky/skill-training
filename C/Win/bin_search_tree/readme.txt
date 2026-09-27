Implement a binary search tree storing arbitrary values:



typedef int (*CompareFn)(const void *, const void *);
typedef struct TreeNode {
    void *data;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;
API:



TreeNode *tree_insert(TreeNode *root,
                      const void *data,
                      size_t data_size,
                      CompareFn compare);
void *tree_find(TreeNode *root,
                const void *key,
                CompareFn compare);
void tree_destroy(TreeNode *root);
Example:



int value = 42;
root = tree_insert(root,
                   &value,
                   sizeof(value),
                   compare_int);
Focus:

void *

ownership

deep copying

dynamic allocation

recursion

function pointers

pointer-to-pointer alternatives

Bonus: implement deletion of a node with 0, 1, or 2 children.