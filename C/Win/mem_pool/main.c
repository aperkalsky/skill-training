#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <inttypes.h>

typedef struct {
    void* memory;
    size_t block_size;
    size_t block_count;

    bool* block_free;
    size_t free_count;
} MemoryPool;

static bool get_free_block_index(MemoryPool* pool, size_t* pindex)
{
    for (size_t i = 0; i < pool->block_count; i++)
    {
        if (pool->block_free[i])
        {
            *pindex = i;
            printf("get_free_block_index returns %zu\n", i);
            return true;
        }
    }

    return false;
}

bool pool_init(MemoryPool* pool, size_t block_size, size_t block_count)
{
    if (pool == NULL || block_size == 0 || block_count == 0)
    {
        return false;
    }

    // check for oversize
    if (block_count > SIZE_MAX / block_size)
    {
        return false;
    }

    // allocate blocks
    pool->memory = malloc(block_size * block_count);

    if (pool->memory == NULL)
    {
        return false;
    }

    // allocate block management
    pool->block_free = malloc(sizeof(bool) * block_count);

    if (pool->block_free == NULL)
    {
        free(pool->memory);
        return false;
    }

    // init block management
    for (size_t i = 0; i < block_count; i++)
    {
        pool->block_free[i] = true;
    }

    // finalize filling struct
    pool->block_size = block_size;
    pool->block_count = block_count;
    pool->free_count = block_count;


    return true;
}

void* pool_alloc(MemoryPool* pool)
{
    if (pool == NULL || pool->free_count == 0)
    {
        return NULL;
    }

    size_t block_index;
    
    if (!get_free_block_index(pool, &block_index))
    {
        return NULL;
    }

    pool->block_free[block_index] = false;
    pool->free_count--;
    return (void*)((uintptr_t)pool->memory + pool->block_size * block_index);
}

bool pool_free(MemoryPool* pool, void* ptr)
{
    if (pool == NULL || ptr == NULL)
    {
        return false;
    }

    // check that the pointer is in the valid range
    uintptr_t start = (uintptr_t)pool->memory;
    uintptr_t address = (uintptr_t)ptr;
    uintptr_t end = start + pool->block_size * pool->block_count;

    if (address < start || address >= end)
    {
        return false;
    }

    // calculate and validate block index
    uintptr_t offset_within_buf = address - start;
 
    if (offset_within_buf % pool->block_size != 0)
    {
        return false;
    }

    size_t buffer_index = (size_t)(offset_within_buf / pool->block_size);

    // check of it was already freed
    if (pool->block_free[buffer_index])
    {
        return false;
    }
    else
    {
        pool->block_free[buffer_index] = true;
        pool->free_count++;
        return true;
    }
}

void pool_destroy(MemoryPool* pool)
{
    if (pool == NULL)
        return;

    free(pool->memory);
    free(pool->block_free);

    pool->memory = NULL;
    pool->block_free = NULL;
    pool->block_size = 0;
    pool->block_count = 0;
    pool->free_count = 0;
}

int main()
{
    MemoryPool pool;

    pool_init(&pool, 32, 100);

    void* p1 = pool_alloc(&pool);
    printf("p1 = 0x%" PRIxPTR "\n", (uintptr_t)p1);
    void* p2 = pool_alloc(&pool);
    printf("p2 = 0x%" PRIxPTR "\n", (uintptr_t)p2);

    pool_free(&pool, p1);

    pool_destroy(&pool);

    return 0;
}