#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
size_t align(int n) {
    int align = 8;
    if (n % 4 == 0 && n != 0) return n;
    if (n < 4) return 4;
    while (n > align) align = align + 4;
    return align;
}

Page* findPage(Allocator *allocator, size_t memSize){
    Page* dividedPages = allocator->dividedPages;
    Page* emptyPages = allocator->emptyPage;
    Page* page = dividedPages;
    while (page != NULL){
        if(page->size >= memSize && page->blockAmount > 0){
            return page;
        }
        page = page->nextPage;
    }
    return emptyPages;
}

void *first_fit(Allocator* allocator, Page* page){
    Block* block = page->block;
    UsedBlock * usedBlocks = allocator->usedBlocks;
    while (block != NULL) {
        if (block->isFree) {
            if (usedBlocks == NULL) {
                UsedBlock * usedBlock = (UsedBlock *) malloc(sizeof(Block));
                usedBlock->nextBlock = NULL;
                usedBlock->prevBlock = NULL;
                usedBlock->originalBlock = block;
                allocator->usedBlocks = usedBlock;
            } else {
                while (usedBlocks->nextBlock != NULL) usedBlocks = usedBlocks->nextBlock;
                UsedBlock* newBlock = (UsedBlock *) malloc(sizeof(Block));
                newBlock->prevBlock = usedBlocks;
                newBlock->nextBlock = NULL;
                newBlock->originalBlock = block;
            }
            page->blockAmount--;
            block->isFree = 0;
            return block->addr;
        }
        block = block->nextBlock;
    }
    return NULL;
}

void dividePage(Page *page, size_t blockSize) {
    Block* block = (Block*) malloc(sizeof(Block));
    int blockAmount = (int) (page->size / blockSize);
    block->size = blockSize;
    block->addr = page->buffer;
    block->isFree = 1;
    page->block = block;
    page->blockAmount++;
    page->size = blockSize;

    for (int i = 1; i < blockAmount; ++i) {
        Block* newBlock = (Block*) malloc(sizeof(Block));
        newBlock->size = blockSize;
        newBlock->addr = (char*) page->buffer + (i * blockSize);
        newBlock->nextBlock = NULL;
        newBlock->prevBlock = block;
        newBlock->isFree = 1;

        block->nextBlock = newBlock;
        block = newBlock;

        page->blockAmount++;
    }
}

void init (Allocator* allocator, int numPages, int size){
    if (numPages < 1) return;
    size = align(size);
    Page* page = (Page*) malloc(sizeof(Page));
    page->size = size;
    page->mode = EMPTY;
    page->nextPage = NULL;
    page->buffer = malloc(sizeof(char) * size);
    page->blockAmount = 0;
    page->block = NULL;

    allocator->emptyPage = page;
    allocator->allocationUsed = 0;
    allocator->pageSize = size;
    allocator->fullSizePage = NULL;
    allocator->dividedPages = NULL;
    allocator->usedBlocks = NULL;

    for (int i = 1; i < numPages; ++i) {
        Page *nextPage = (Page*) malloc(sizeof(Page));
        nextPage->size = size;
        nextPage->buffer = malloc(sizeof(char) * size);
        nextPage->mode = EMPTY;
        nextPage->nextPage = NULL;
        nextPage->blockAmount = 0;
        nextPage->block = NULL;

        page->nextPage = nextPage;
        page = nextPage;
    }
}

void* mem_alloc(Allocator* allocator, int size){
    Page * page;
    size = align(size);
    if (!allocator->allocationUsed) {
        allocator->allocationUsed = 1;
        page = allocator->emptyPage;
        if (size > (allocator->pageSize / 2)) {
            page->mode = PAGED;
            allocator->emptyPage = allocator->emptyPage->nextPage;
            allocator->fullSizePage = page;
            allocator->fullSizePage->nextPage = NULL;
            return page->buffer;
        }
        dividePage(page, size);

        page->mode = DIVIDED;

        allocator->emptyPage = allocator->emptyPage->nextPage;
        allocator->dividedPages = page;
        allocator->dividedPages->nextPage = NULL;
        return first_fit(allocator, page);
    }
    page = findPage(allocator, size);
    if (page == NULL) {
        return NULL;
    } else if (page->mode == DIVIDED) {
        return first_fit(allocator, page);
    } else if (page->mode == EMPTY && size > (page->size / 2)) {
        page->mode = PAGED;
        page->nextPage = allocator->fullSizePage;
        allocator->emptyPage = allocator->emptyPage->nextPage;
        return page->buffer;
    } else {
        dividePage(page, size);

        page->nextPage = allocator->dividedPages;
        page->mode = DIVIDED;

        allocator->dividedPages = page;
        allocator->emptyPage = allocator->emptyPage->nextPage;
        return first_fit(allocator, page);
    }
}

void mem_free(Allocator* allocator, void* addr) {
    UsedBlock * block = allocator->usedBlocks;
    Page* page = allocator->fullSizePage;
    while (block != NULL) {
        if (block->originalBlock->addr == addr && !block->originalBlock->isFree) {
            block->originalBlock->isFree = 1;
            UsedBlock * prevBlock = block->prevBlock;
            UsedBlock* nextBlock = block->nextBlock;
            if (prevBlock != NULL) { prevBlock->nextBlock = nextBlock; }
            if (nextBlock != NULL) { nextBlock->prevBlock = prevBlock; }
            return;
        }
        block = block->nextBlock;
    }
    while (page != NULL) {
        if(page->buffer == addr) {
            page->mode = EMPTY;
            return;
        }
        page = page->nextPage;
    }
}

void* mem_realoc(Allocator* allocator, void* addr, int size) {
    void* newAddr = mem_alloc(allocator, size);
    if (newAddr == NULL) return NULL;
    memcpy(newAddr, addr, size);
    mem_free(allocator, addr);
    return newAddr;
}

void mem_dump(Allocator *allocator){}


int main() {
    Allocator allocator;
    init(&allocator, 2, 256);
    int *a = (int*)mem_alloc(&allocator, 64);
    *a = 666;
    mem_alloc(&allocator, 64);
    mem_alloc(&allocator, 64);
    mem_alloc(&allocator, 64);
    int *_a = (int*)mem_realoc(&allocator, a, 64);
    printf("Hello, World!\n");
    return 0;
}
