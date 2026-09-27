#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    uint8_t* data;
    size_t size;
} Buffer;

Buffer* create_buffer(size_t size)
{
    Buffer* b = malloc(sizeof(b));

    if (!b)
        return NULL;

    b->data = malloc(size);

    if (!b->data)
        return NULL;

    b->size = size;
    return b;
}

void fill_buffer(const Buffer* b, uint8_t value)
{
    memset(b->data, value, b->size);
}

void resize_buffer(Buffer* b, size_t new_size)
{
    b->data = realloc(b->data, new_size);
    b->size = new_size;
}

void destroy_buffer(Buffer* b)
{
    free(b->data);
    free(b);
}

int main(void)
{
    Buffer* b = create_buffer(100);

    fill_buffer(b, 0xAA);

    uint8_t* p = b->data + 90;

    resize_buffer(b, 1000);

    printf("%02X\n", *p);

    destroy_buffer(b);

    printf("%02X\n", b->data[0]);
}