#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 线性分配器 Bump Allocator

#define POOL_SIZE 1024
#define ALIGNMENT 8

typedef struct BlockHeader{
    size_t size;
    struct BlockHeader *next;
} BlockHeader;


typedef struct {
    char pool[POOL_SIZE];
    size_t offset;

    BlockHeader *free_list;
} MemPool;

void mp_init(MemPool *mp)
{
    mp->offset = 0;
    mp->free_list = NULL;
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
    if (mp->free_list != NULL) {
        BlockHeader *prev = NULL;
        BlockHeader *cur = mp->free_list;

        while (cur != NULL) {
            if (size <= cur->size) {
                if (prev == NULL) 
                    mp->free_list = cur->next;
                else 
                    prev->next = cur->next;

                cur->next = NULL;
                return cur + 1;
            }

            prev = cur;
            cur = cur->next;
        }
    }


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
    mp->free_list = NULL;
}

void mp_free(MemPool *mp, void *p)
{
    BlockHeader *header = (BlockHeader *)p - 1;
    header->next = mp->free_list;
    mp->free_list = header;
}

int main()
{
    MemPool mp;
    mp_init(&mp);

    char *a = mp_alloc(&mp, 64);
    char *b = mp_alloc(&mp, 32);
    char *c = mp_alloc(&mp, 16);

    mp_free(&mp, a);
    mp_free(&mp, b);
    mp_free(&mp, c);

    // BlockHeader *cur = mp.free_list;
    // while (cur) {
    //     printf("%p\t%zu\n", (void *)cur, cur->size);
    //     cur = cur->next;
    // }

    char *buf = mp_alloc(&mp, 24);
    BlockHeader *buf_header = (BlockHeader *)buf - 1;
    printf("%p\t%zu\n", buf_header, buf_header->size);

    return 0;
}