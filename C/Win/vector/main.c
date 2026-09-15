#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Structural layout for Generic Vector via Type Erasure
typedef struct
{
    void* data;          // Raw pointer to the contiguous block of memory
    size_t element_size; // The size of an individual element in bytes
    size_t size;         // The number of elements currently stored
    size_t capacity;     // The maximum number of elements before reallocating
} Vector;

// Custom struct to test generic compatibility
typedef struct {
    uint32_t id;
    float score;
} Student;

// ==========================================
// TODO: IMPLEMENT THESE FOUR FUNCTIONS
// ==========================================

/**
 * Dynamically allocates a Vector structure on the heap.
 * Initializes capacity to a sensible default (e.g., 4) and data pointer using malloc.
 */
Vector* vector_create(size_t element_size)
{
    // Allocate the main Vector descriptor structure on the heap
    Vector* v = (Vector*)malloc(sizeof(Vector));
    if (v == NULL)
    {
        return NULL; // Guard against allocation failure
    }

    // Initialize properties
    v->element_size = element_size;
    v->size = 0;
    v->capacity = 4; // Sensible default capacity

    // Allocate the actual array buffer block on the heap
    v->data = malloc(v->capacity * v->element_size);
    if (v->data == NULL)
    {
        free(v); // If the buffer allocation fails, clean up the struct before exiting
        return NULL;
    }

    return v; // Safe! This heap address remains valid after the function exits
}

/**
 * Appends a copy of 'item' to the end of the vector.
 * If size == capacity, it must double the capacity using realloc.
 * Remember to copy the item's raw bytes into the buffer using memcpy.
 */
void vector_push_back(Vector* v, const void* item)
{
    // Check if there's room for the new item
    if (v->size == v->capacity)
    {
        size_t new_capacity = v->capacity * 2;

        // Fix: Pass the total new size IN BYTES to realloc
        void* new_buf = realloc(v->data, new_capacity * v->element_size);

        if (new_buf == NULL)
        {
            // Handle allocation failure gracefully by exiting early
            return;
        }

        // realloc already copied old data to new_buf, no manual memcpy needed
        v->capacity = new_capacity;
        v->data = new_buf;
    }

    // Calc target memory address
    size_t existing_data_length = v->element_size * v->size;
    void* target = (char*)v->data + existing_data_length;

    // Copy new data
    memcpy(target, item, v->element_size);

    // increment item count
    v->size++;
}


/**
 * Returns a pointer to the element at the specified index.
 * Must perform raw byte pointer arithmetic to calculate the offset.
 */
void* vector_get(const Vector* v, size_t index)
{
    // Out-of-bounds check (handles empty vector and too-large index in one go)
    if (index >= v->size)
    {
        return NULL;
    }

    // offset calculation logic
    size_t offset = v->element_size * index;
    return (char*)v->data + offset;
}


/**
 * Safely cleans up all dynamically allocated memory.
 * Frees both the internal buffer and the vector structure itself.
 */
void vector_destroy(Vector* v)
{
    if (v != NULL)
    {
        // Free the internal array buffer first
        free(v->data);

        // Free the Vector structure container second
        free(v);
    }
}

// ==========================================
// TEST HARNESS (Do not modify main)
// ==========================================
int main()
{
    printf("--- Test 1: Vector of Integers ---\n");
    Vector* int_vec = vector_create(sizeof(int));

    // Push integers 10 to 50
    for (int i = 10; i <= 50; i += 10) {
        vector_push_back(int_vec, &i);
    }

    // Read them back
    for (size_t i = 0; i < int_vec->size; i++) {
        int* val = (int*)vector_get(int_vec, i);
        printf("Index %zu: %d\n", i, *val);
    }
    printf("Final Capacity: %zu (Expected: 8)\n\n", int_vec->capacity);
    vector_destroy(int_vec);

    printf("--- Test 2: Vector of Custom Structures ---\n");
    Vector* student_vec = vector_create(sizeof(Student));

    Student s1 = { 101, 88.5f };
    Student s2 = { 102, 94.2f };

    vector_push_back(student_vec, &s1);
    vector_push_back(student_vec, &s2);

    for (size_t i = 0; i < student_vec->size; i++) {
        Student* s = (Student*)vector_get(student_vec, i);
        printf("Student Index %zu -> ID: %u, Score: %.1f\n", i, s->id, s->score);
    }

    vector_destroy(student_vec);
    return 0;
}
