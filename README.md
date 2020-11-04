Memory allocator in C

The struct allocator which can select data in heap, with size that you say.

This allocator use malloc to allocate memory. Each block of memory == size leveling by 4 + Header size. In this realization header size == 13 bytes or 9 bytes (32 bit OS)


Functions:

void init(struct allocator* _allocator) - initialize allocator and create first Header block

void* mem_alloc(struct allocator* _allocator,size_t size) – Search for free block for allocate memory, use first fit algorithm. Size will be leveling by 4 and total size of allocation will be size + header size 

void mem_free(struct allocator* _allocator, void* addr) – mark this block as free. If right blocks free so, then unites them( make ptr to new block).

void* mem_realloc(struct allocator* _allocator, void* addr, size_t size) – try to find new block. If success copy data to new block and call mem_free for old block

Examples

Creation of allocator.

   Code:
   
    struct allocator _allocator;
    init(&_allocator, 100);
    
Allocate memory.

   Code:

    struct allocator _allocator;
    init(&_allocator, 100);
    int *res;
    res = (int*)mem_alloc(&_allocator, 4);
    *res = 999;
   
Free of memory.
   Code:

    struct allocator _allocator;
    init(&_allocator, 100);
    int *res;
    res = (int*)mem_alloc(&_allocator, 4);
    *res = 999;
    mem_free(res)
    
