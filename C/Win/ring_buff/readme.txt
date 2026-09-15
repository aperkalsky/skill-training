Themes: Pointers, Memory Management, Data Structures, volatile Qualifier

Background:
In embedded systems, an Interrupt Service Routine (ISR) often receives byte data from a hardware peripheral (like a UART) and pushes it into a queue. A main loop thread pops data from that queue to process it. Because the hardware/ISR can change memory outside the main loop's control, compiler optimizations must be handled carefully.

Your Task:
Implement a thread-safe, fixed-size Circular Ring Buffer (FIFO) for bytes.

 

Define a struct RingBuffer that contains a buffer pointer, a capacity, a head index, and a tail index.

The head and tail variables must be qualified with volatile so the compiler knows they can change asynchronously via an ISR. [1]

Write the following functions:

void rb_init(RingBuffer *rb, size_t capacity); (dynamically allocate the internal buffer).

bool rb_push(RingBuffer *rb, uint8_t data); (returns false if full).

bool rb_pop(RingBuffer *rb, uint8_t *data); (returns false if empty).

void rb_free(RingBuffer *rb); (safely cleans up dynamically allocated memory).