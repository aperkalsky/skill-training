#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdlib.h>

typedef struct MemoryBlockHeader
{
    void* next;
    uint32_t free;
}MemoryBlockHeader;

typedef struct MemoryPool
{
    void* memory;
    size_t block_size;
    size_t block_count;

    struct MemoryBlockHeader* free_list;
} MemoryPool;

bool pool_init(MemoryPool* pool, size_t block_size, size_t block_count)
{
    // argument and boundaries check
    if (pool == NULL || block_size == 0 || block_count == 0)
    {
        return false;
    }

    if (block_size > SIZE_MAX - sizeof(MemoryBlockHeader))
    {
        return false;
    }

    size_t global_block_size = block_size + sizeof(MemoryBlockHeader);

    if (block_count > SIZE_MAX / global_block_size)
    {
        return false;
    }

    // memory allocation
    size_t pool_size = global_block_size * block_count;

    void* pmem = malloc(pool_size);

    if (pmem == NULL)
    {
        return false;
    }

    // format the pool (init headers)
    MemoryBlockHeader* p_curr_header;
    MemoryBlockHeader* p_prev_header = NULL;

    for (size_t i = block_count; i > 0; i--)
    {
        p_curr_header = (MemoryBlockHeader*)((uintptr_t)pmem + global_block_size * (i - 1));
        p_curr_header->free = true;
        p_curr_header->next = (void*)p_prev_header;
        p_prev_header = p_curr_header;
    }

    // finalize the init
    pool->memory = pmem;
    pool->free_list = (MemoryBlockHeader*)pmem;
    pool->block_size = block_size;
    pool->block_count = block_count;

    return true;
}

void* pool_alloc(MemoryPool* pool);
bool pool_free(MemoryPool* pool, void* ptr);
void pool_destroy(MemoryPool* pool);

int main()
{
    MemoryPool pool;

    bool result = pool_init(&pool, 128, 100);

    if (result)
    {
        puts("Pool init succeeded\n");
    }
    else
    {
        puts("Pool init failed\n");
        return -1;
    }

    return 0;
}