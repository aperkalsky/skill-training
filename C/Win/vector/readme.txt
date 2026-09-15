Themes: Pointers, Memory Management, Data Structures

Background:
C doesn't have native templates like C++. To build a generic container, you must rely on type erasure using void* pointers, explicit element sizes, and pointer arithmetic.

Your Task:
Implement a generic, dynamic Vector similar to std::vector in C++.

 

Define a struct that keeps track of a void *data block, the element_size in bytes, the current size, and the total capacity.

Implement the following functions:

Vector* vector_create(size_t element_size);

void vector_push_back(Vector *v, const void *item); (Must double capacity using realloc when full).

void* vector_get(const Vector *v, size_t index); (Performs raw pointer arithmetic using byte casting: (char*)data + (index * element_size)).

void vector_destroy(Vector *v);