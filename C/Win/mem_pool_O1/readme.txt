Fixed-size memory pool — optimized

Implement a fixed-size memory pool similar to your #3, but with O(1) allocation and freeing.

Requirements:



typedef struct {
    void *memory;
    size_t block_size;
    size_t block_count;
    /* allocator state */
} MemoryPool;
Implement:



bool pool_init(MemoryPool *pool,
               size_t block_size,
               size_t block_count);
void *pool_alloc(MemoryPool *pool);
bool pool_free(MemoryPool *pool, void *ptr);
void pool_destroy(MemoryPool *pool);

Constraints:

pool_alloc() must be O(1).

pool_free() must be O(1).

Don't use malloc()/free() for individual blocks.

All blocks must be usable by arbitrary objects requiring normal alignment.

Detect:

pointer outside the pool

pointer not aligned to a block

double free

Focus: intrusive free lists, alignment, allocator invariants.