#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// NOTE: If using C11, use #include <stdatomic.h> and _Atomic specfiier instead of volatile

// type defs
typedef struct
{
	uint8_t* pBuf;			// ring buffer address
	size_t capacity;		// ring buffer capacity (actually 1 byte less, this way w edistinguish between empty and full buffer)
	volatile size_t head;	// index to where the next byte will be written
	volatile size_t tail;	// index to where the next byte will be read from
}RingBuffer;

// function defs
void rb_init(RingBuffer* rb, size_t capacity)
{
	rb->capacity = capacity;
	rb->pBuf = malloc(rb->capacity);
	rb->head = rb->tail = 0;
}

void rb_free(RingBuffer* rb)
{
	free(rb->pBuf);
}

bool rb_push(RingBuffer* rb, uint8_t data)
{
	// check if buffer has room
	if (((rb->head + 1) % rb->capacity) == rb->tail)
	{
		return false; // full
	}

	// store the data at the current head index
	rb->pBuf[rb->head] = data;

	// advance head and wrap around automatically using modulo
	rb->head = (rb->head + 1) % rb->capacity;

	return true;
}

bool rb_pop(RingBuffer* rb, uint8_t* data)
{
	// check for empty buffer
	if (rb->head == rb->tail)
	{
		return false;	// empty
	}

	// retrieve the data
	*data = rb->pBuf[rb->tail];

	// advance tail
	rb->tail = (rb->tail + 1) % rb->capacity;

	return true;
}

int main()
{
	RingBuffer rb;
	// Capacity of 4 means it can hold up to 3 elements safely
	rb_init(&rb, 4);

	printf("Pushing A, B, C...\n");
	rb_push(&rb, 'A');
	rb_push(&rb, 'B');
	rb_push(&rb, 'C');

	// This push should fail because the buffer is full (3/3 items filled)
	if (!rb_push(&rb, 'D')) {
		printf("Buffer full! Correctly blocked 'D'.\n");
	}

	uint8_t popped_val;
	while (rb_pop(&rb, &popped_val)) {
		printf("Popped: %c\n", popped_val);
	}

	rb_free(&rb);
	return 0;
}
