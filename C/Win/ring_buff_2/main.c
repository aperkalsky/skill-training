#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h> // for memset

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

bool rb_init(RingBuffer* rb, uint8_t* storage, size_t size)
{
    if (rb == NULL || storage == NULL || size == 0)
    {
        return false;
    }

    rb->buffer = storage;
    rb->capacity = size;
    rb->head = rb->tail = rb->count = 0;

    memset(rb->buffer, 0, rb->capacity);

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
    return 0;
}