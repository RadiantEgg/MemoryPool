#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 线性分配器 Bump Allocator

#define POOL_SIZE 1024
#define ALIGNMENT 8

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
    
    if (aligned + size > POOL_SIZE)
        return NULL;
    
    // void *p = &mp->pool[mp->offset];
    void *p = mp->pool + aligned;    // 起始量 + 偏移量

    mp->offset = aligned + size;

    return p;
}


int main()
{
    MemPool mp;
    mp_init(&mp);

    int *a = mp_alloc(&mp, sizeof(int));
    printf("%zu\n", mp.offset);

    char *buf = mp_alloc(&mp, 16);
    printf("%zu\n", mp.offset);

    *a = 123;
    strcpy(buf, "memory pool");

    printf("%d\n%s\n", *a, buf);
    return 0;
}