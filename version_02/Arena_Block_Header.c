#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 线性分配器 Bump Allocator

#define POOL_SIZE 1024
#define ALIGNMENT 8
#define BLOCK_HEADER_SIZE 8

typedef struct {
    size_t size;
} BlockHeader;


typedef struct {
    char pool[POOL_SIZE];
    size_t offset;
} MemPool;

void mp_init(MemPool *mp)
{
    mp->offset = 0;
}

// 内存对齐
size_t align_up(size_t offset, size_t align)
{
    if (offset % align == 0)
        return offset;

    return align * (offset / align + 1); 
}


void *mp_alloc(MemPool *mp, size_t size)
{
    size_t aligned = align_up(mp->offset, ALIGNMENT);

    if (aligned + sizeof(BlockHeader) + size > POOL_SIZE)
    return NULL;
    
    BlockHeader *header = (BlockHeader *)(mp->pool + aligned);
    header->size = size;
    
    // void *p = &mp->pool[mp->offset];
    // void *p = mp->pool + aligned + HEADER_SIZE;    // 起始量 + 偏移量

    mp->offset = aligned + sizeof(BlockHeader) + size;

    return header + 1;
}

void mp_reset(MemPool *mp)
{
    mp->offset = 0;
}


int main()
{
    MemPool mp;
    mp_init(&mp);

    char *buf = mp_alloc(&mp, 16);

    BlockHeader *h = ((BlockHeader *)buf) - 1;

    printf("%zu\n", h->size);
    return 0;
}