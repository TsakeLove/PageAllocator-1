//
// Created by Sasha on 04.11.2020.
//

#ifndef ALOCATORPAGE_MAIN_H
#define ALOCATORPAGE_MAIN_H
#define EMPTY 0
#define DIVIDED 1
#define PAGED 2

typedef struct block {
    struct block* nextBlock;
    struct block* prevBlock;
    void* addr;
    size_t size;
    size_t isFree;
} Block;

typedef struct usedBlock {
    struct usedBlock* nextBlock;
    struct usedBlock* prevBlock;
    Block* originalBlock;
} UsedBlock;

typedef struct page {
    int blockAmount;
    int size;
    char mode;
    void* buffer;
    struct page* nextPage;
    Block* block;
} Page;

typedef struct allocator {
    char allocationUsed;
    int pageSize;
    UsedBlock * usedBlocks;
    Page* emptyPage;
    Page* dividedPages;
    Page* fullSizePage;
} Allocator;
#endif //ALOCATORPAGE_MAIN_H
