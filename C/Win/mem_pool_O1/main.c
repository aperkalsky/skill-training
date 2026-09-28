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
    size_t global_block_size;

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

    pool->global_block_size = block_size + sizeof(MemoryBlockHeader);

    if (block_count > SIZE_MAX / pool->global_block_size)
    {
        return false;
    }

    // memory allocation
    size_t pool_size = pool->global_block_size * block_count;

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
        p_curr_header = (MemoryBlockHeader*)((uintptr_t)pmem + pool->global_block_size * (i - 1));
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

void* pool_alloc(MemoryPool* pool)
{
    if (pool == NULL)
    {
        return NULL;
    }

    if (pool->free_list == NULL)
    {
        return NULL;    // no free blocks
    }

    MemoryBlockHeader* free_block = pool->free_list;

    // mark that it's in use
    free_block->free = false;

    // remove it from free list
    pool->free_list = free_block->next;

    return (void*)((uintptr_t)free_block + sizeof(MemoryBlockHeader));
}

bool pool_free(MemoryPool* pool, void* ptr)
{
    if (pool == NULL || ptr == NULL)
    {
        return false;
    }

    // check that the pointer is in the valid range
    uintptr_t block_start_addr = (uintptr_t)ptr - sizeof(MemoryBlockHeader);
    uintptr_t pool_start_addr = (uintptr_t)pool->memory;
    uintptr_t last_block_end_addr = pool_start_addr + (pool->block_size + sizeof(MemoryBlockHeader)) * (pool->block_count -1);

    if (block_start_addr < pool_start_addr || block_start_addr > last_block_end_addr)
    {
        return false;
    }

    // calculate and validate block index
    uintptr_t offset_within_buf = block_start_addr - pool_start_addr;

    if (offset_within_buf % pool->global_block_size != 0)
    {
        return false;
    }

    // check if this buffer is already free
    MemoryBlockHeader* pheader = block_start_addr;

    if (pheader->free)
    {
        return false;
    }

    // mark it as free and add to the free list
    pheader->free = true;
    pheader->next = pool->free_list;
    pool->free_list = pheader;

    return true;
}

void pool_destroy(MemoryPool* pool)
{
    if (pool == NULL)
    {
        return;
    }

    if (pool->memory)
    {
        free(pool->memory);
        pool->memory = NULL;
        pool->block_count = 0;
        pool->block_size = 0;
        pool->global_block_size = 0;
        pool->free_list = NULL;
    }
}

#define NUM_BLOCK_ADDR_TO_KEEP 5

int main()
{
    MemoryPool pool;
    size_t block_size = 128;
    size_t block_count = 100;

    bool result = pool_init(&pool, block_size, block_count);

    if (result)
    {
        puts("Pool init succeeded\n");
    }
    else
    {
        puts("Pool init failed\n");
        return -1;
    }

    // test blocks allocation
    const size_t num_iterations = block_count + 3;
    void* blocks[NUM_BLOCK_ADDR_TO_KEEP];

    for (size_t i = 0; i < num_iterations; i++)
    {
        void* pblock = pool_alloc(&pool);

        if (i < NUM_BLOCK_ADDR_TO_KEEP)
        {
            blocks[i] = pblock;
        }

        printf("Iteration %zu: ", i);
        if (pblock == NULL)
        {
            printf("No free blocks\n");
        }
        else
        {
            printf("got 0x%" PRIxPTR "\n", (uintptr_t)pblock);
        }
    }

    // attempt to free invalid buffer
    result = pool_free(&pool, (void*)(uintptr_t)0xAABBCCDD);
    printf("Result of freeing of invalid pointer = %d\n", result);

    // now try freeing the valid buffers
    for (size_t i = 0; i < NUM_BLOCK_ADDR_TO_KEEP; i++)
    {
        result = pool_free(&pool, blocks[i]);
        printf("Result of freeing of valid pointer %zu (0x%"PRIxPTR")= %d\n", i, blocks[i], result);
    }

    pool_destroy(&pool);

    return 0;
}