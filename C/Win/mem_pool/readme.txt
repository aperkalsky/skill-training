Implement a fixed-size memory pool:



typedef struct {
    void *memory;
    size_t block_size;
    size_t block_count;
    ...
} MemoryPool;
API:



bool pool_init(MemoryPool *pool,
               size_t block_size,
               size_t block_count);
void *pool_alloc(MemoryPool *pool);
void pool_free(MemoryPool *pool, void *ptr);
void pool_destroy(MemoryPool *pool);
Example:



MemoryPool pool;
pool_init(&pool, 32, 100);
void *p1 = pool_alloc(&pool);
void *p2 = pool_alloc(&pool);
pool_free(&pool, p1);
Requirements:

No malloc() after initialization.

Allocation should be O(1).

Detect invalid pointers passed to pool_free().

Detect double-free.

Use a free-list implemented with pointers stored inside the free blocks.