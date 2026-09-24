#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t* buffer;
    size_t capacity;// buffer size
    size_t head;    // position where the next item will be written
    size_t tail;    // position where the next item will be read
    size_t count;   // num items currently stored in the buffer
} RingBuffer;

#define RB_SIZE 16

static uint8_t ring_buf[RB_SIZE];

static RingBuffer rb_desc;

void rb_print(const RingBuffer* rb)
{
    if (rb == NULL)
    {
        return;
    }

    printf("Capacity = %zu\n", rb->capacity);
    printf("Count = %zu\n", rb->count);
    printf("Head = %zu\n", rb->head);
    printf("Tail = %zu\n", rb->tail);

    puts("Data:");

    for (size_t i = 0; i < rb->capacity; i++)
    {
        printf("%zu:0x%02X ", i, rb->buffer[i]);
    }
    puts(" ");
}

bool rb_init(RingBuffer* rb, uint8_t* storage, size_t size)
{
    if (rb == NULL || storage == NULL || size == 0)
    {
        return false;
    }

    rb->buffer = storage;
    rb->capacity = size;
    rb->head = rb->tail = rb->count = 0;

    return true;
}

bool rb_put(RingBuffer* rb, uint8_t byte)
{
    if (rb == NULL)
    {
        return false;
    }

    if (rb->count == rb->capacity)
    {
        return false;   // buffer is full
    }

    rb->buffer[rb->head] = byte;
    
    if (rb->head == rb->capacity - 1)
    {
        rb->head = 0;
    }
    else
    {
        rb->head++;
    }

    rb->count++;

    return true;
}

// Statuses
// false: buffer was empty or pointer ot byte is invalid. *byte is not modified
// true: one byte was removed and copied to *byte
bool rb_get(RingBuffer* rb, uint8_t* byte)
{
    if (rb == NULL || byte == NULL || rb->count == 0)
    {
        return false;
    }

    *byte = rb->buffer[rb->tail];

    if (rb->tail == rb->capacity - 1)
    {
        rb->tail = 0;
    }
    else
    {
        rb->tail++;
    }

    rb->count--;

    return true;
}

size_t rb_count(const RingBuffer* rb)
{
    if (rb == NULL)
    {
        return 0;
    }

    return rb->count;
}

// report free space
size_t rb_free_space(const RingBuffer* rb)
{
    if (rb == NULL)
    {
        return 0;
    }

    return rb->capacity - rb->count;
}

int main()
{
    bool status;

    status = rb_init(&rb_desc, ring_buf, RB_SIZE);
    printf("Init status = %d\n", status);

    rb_print(&rb_desc);

    // test filling buffer
    for (int i = 0; i < RB_SIZE + 3; i++)
    {
        status = rb_put(&rb_desc, (uint8_t)i);
        printf("Element %d adding status = %d\n", i, status);
    }

    rb_print(&rb_desc);

    // test reading buffer
    uint8_t tmp;

    status = rb_get(&rb_desc, &tmp);
    printf("Get status = %d, data = %d\n", status, tmp);
    rb_print(&rb_desc);

    // now put another one
    status = rb_put(&rb_desc, (uint8_t)0xAA);
    printf("Element adding status = %d\n", status);
    rb_print(&rb_desc);

    return 0;
}