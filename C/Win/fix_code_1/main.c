#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct {
    uint8_t* data;
    size_t size;
} Buffer;

Buffer* create_buffer(size_t size)
{
    if (size == 0)
    {
        return NULL;
    }

    Buffer* b = malloc(sizeof(*b));

    if (b == NULL)
    {
        return NULL;
    }

    b->data = malloc(size);

    if (b->data == NULL)
    {
        free(b);    // need to free prevoiusly allocated memory
        return NULL;
    }

    b->size = size;

    return b;
}

bool fill_buffer(const Buffer* b, uint8_t value)
{
    // add arguments check
    if (b == NULL)
    {
        return false;
    }

    memset(b->data, value, b->size);

    return true;
}

bool resize_buffer(Buffer* b, size_t new_size)
{
    // add arguments check
    if (b == NULL || new_size == 0)
    {
        return false;
    }

    void* tmp = realloc(b->data, new_size);

    if (tmp == NULL)
    {
        return false;
    }

    b->data = tmp;
    b->size = new_size;

    return true;
}

void destroy_buffer(Buffer* b)
{
    if (b == NULL)
    {
        return;
    }

    free(b->data);
    free(b);
}

int main(void)
{
    Buffer* b = create_buffer(100);

    if (b == NULL)
    {
        puts("Failed to create buffer");
        return -1;
    }

    if (!fill_buffer(b, 0xAA))
    {
        destroy_buffer(b);
        return -1;
    }

    if (!resize_buffer(b, 1000))
    {
        destroy_buffer(b);
        return -1;
    }

    uint8_t* p = b->data + 90;

    printf("%02X\n", *p);

    printf("%02X\n", b->data[0]);

    destroy_buffer(b);

    b = NULL;
}