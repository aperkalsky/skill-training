Ring buffer with wrap-around
Implement a fixed-size byte FIFO:



typedef struct {
    uint8_t *buffer;
    size_t capacity;
    size_t head;
    size_t tail;
    size_t count;
} RingBuffer;
API:



bool rb_init(RingBuffer *rb, uint8_t *storage, size_t size);
bool rb_put(RingBuffer *rb, uint8_t byte);
bool rb_get(RingBuffer *rb, uint8_t *byte);
size_t rb_count(const RingBuffer *rb);
size_t rb_free_space(const RingBuffer *rb);
Requirements:

No dynamic allocation.

Correct wrap-around.

Correct handling of full and empty states.

const correctness.

size_t rather than int for indexes/sizes.

Bonus: modify it for single-producer/single-consumer use and consider which fields would need to be volatile in a bare-metal environment.