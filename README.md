Memory allocator in C

It allocates memory which divided by pages. Each pages has 3 stated: 
1) EMPTY - it means that page is free and not divided
2) PAGED - it means that allocation size was > size of page divided by 2 so we just return all page memory block
3) DIVIDED - it means that page already been divided, either can be free or not.

Each page has 3 linked list: 
1) EmptyPages - list for EMPTY pages
2) DividedPages - list for DIVIDED pages
3) FullSizePages - list for PAGED pages



Functions:

void init(struct allocator* allocator, int numPages, int size) - initialize allocator and create pages  

void* mem_alloc(struct allocator* allocator,size_t size) – Firstly try to find DIVIDED page with block size that >= allocated size and page has free blocks, if such page exist return first free block if such page not exist try to find EMPTY page if such page exist try to divide it by block if page not exist return NULL
pseudoCode: 
    find_page_with_block_size <= page_block_size
    if (page_exist)
        find_free_block
    else 
        find_empty_page
            if (page_exist)
                try_to_divide
                    if (success)
                    find_free_block
                    else 
                        return fullPage 

void mem_free(struct allocator* allocator, void* addr) – find usedBlock if exist mark as free if no such block check pages and mark page as EMPTY 

void* mem_realloc(struct allocator* allocator, void* addr, size_t size) – call mem_alloc if result of mem_alloc != NULL than copy data to new memory address by calling memcpy and free memory by old address

Examples

Creation of allocator.

   Code:
   
    Allocator* allocator;
    init(&_allocator, 4, 256);
    
Allocate memory.

   Code:

    Allocator* allocator;
    init(&_allocator, 4, 256);
    int *res;
    res = (int*)mem_alloc(&_allocator, 64);
    *res = 999;
   
Free of memory.
   Code:

    Allocator* allocator;
    init(&_allocator, 4, 256);
    int *res;
    res = (int*)mem_alloc(&_allocator, 64);
    *res = 999;
    mem_free(res)
    
