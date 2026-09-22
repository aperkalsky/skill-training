Implement memcpy() and memmove()

Implement:

void *my_memcpy(void *dest, const void *src, size_t n);
void *my_memmove(void *dest, const void *src, size_t n);

Then demonstrate why these two functions cannot have identical implementations.

For example:

src:   A B C D E F G H
              ↑
dest:     ?

Test overlapping regions such as:

my_memmove(buffer + 2, buffer, 6);

Focus:

void * and casting.
Pointer arithmetic.
Overlapping memory.
Forward vs backward copying.
unsigned char as the appropriate byte-level type.