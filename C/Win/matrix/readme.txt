Themes: Pointers (Pointer-to-Pointer), Memory Management, const Qualifier

Background:
Managing multi-dimensional arrays dynamically in C requires allocating an array of pointers, where each pointer targets an array of values (row allocation). Cleaning this up properly and passing it read-only requires exact pointer mastery.

Your Task:
Write a function that accepts a dynamic 2D matrix and returns a completely new, dynamically allocated transposed matrix (rows become columns, columns become rows).

 

Signature: int** transpose_matrix(const int *const *matrix, size_t rows, size_t cols);

Pay strict attention to the parameter type: const int *const *matrix. This means both the row pointers and the integers themselves cannot be modified by your function.

Handle dynamic allocation failure gracefully. If any internal malloc or calloc fails midway through allocating the rows for the new matrix, your code must unwind and free all previously allocated rows to avoid a memory leak.

 