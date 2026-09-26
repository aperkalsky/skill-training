#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <inttypes.h>

typedef struct {
    void* memory;
    size_t block_size;
    size_t block_count;

    bool* free_blocks;
    size_t free_count;
} MemoryPool;

static bool get_free_block_index(MemoryPool* pool, size_t* pindex)
{
    for (size_t i = 0; i < pool->block_count; i++)
    {
        if (pool->free_blocks[i])
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

    // allocate blocks
    pool->memory = malloc(block_size * block_count);

    if (pool->memory == NULL)
    {
        return false;
    }

    // allocate block management
    pool->free_blocks = malloc(sizeof(bool*) * block_count);

    if (pool->free_blocks == NULL)
    {
        free(pool->memory);
        return false;
    }

    // init block management
    for (size_t i = 0; i < block_count; i++)
    {
        pool->free_blocks[i] = true;
    }

    // finalize filling struct
    pool->block_size = block_size;
    pool->block_count = block_count;
    pool->free_count = block_count;


    return true;
}

void* pool_alloc(MemoryPool* pool)
{
    if (pool->free_count == 0)
    {
        return NULL;
    }

    size_t block_index;
    
    if (get_free_block_index(pool, &block_index))
    {
        pool->free_blocks[block_index] = false;
        pool->free_count--;
        return (void*)((uintptr_t)pool->memory + pool->block_size * block_index);
    }
    else
    {
        return NULL;
    }
}

bool pool_free(MemoryPool* pool, void* ptr)
{
    if (pool == NULL || ptr == NULL)
    {
        return false;
    }

    // check that the pointer is in the valid range
    if (ptr >= pool->memory && ptr <= (void*)((uintptr_t)pool->memory + pool->block_size * (pool->block_count - 1)))
    {
        // calculate and validate block index
        uintptr_t offset_within_buf = (uintptr_t)ptr - (uintptr_t)pool->memory;

        if (offset_within_buf % pool->block_size == 0)
        {
            size_t buffer_index = (size_t)(offset_within_buf / pool->block_size);

            // check of it was already freed
            if (pool->free_blocks[buffer_index])
            {
                return false;
            }
            else
            {
                pool->free_blocks[buffer_index] = true;
                pool->free_count++;
                return true;
            }
        }
    }

    return false;
}

void pool_destroy(MemoryPool* pool)
{
    if (pool != NULL)
    {
        if (pool->memory != NULL)
        {
            free(pool->memory);
        }

        if (pool->free_blocks != NULL)
        {
            free(pool->free_blocks);
        }
    }
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