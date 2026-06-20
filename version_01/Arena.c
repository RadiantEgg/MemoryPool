#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 线性分配器 Bump Allocator

#define POOL_SIZE 1024

typedef struct {
    char pool[POOL_SIZE];
    size_t offset;
} MemPool;

void mp_init(MemPool *mp)
{
    mp->offset = 0;
}

void *mp_alloc(MemPool *mp, size_t size)
{
    if (mp->offset + size > POOL_SIZE)
        return NULL;

    // void *p = &mp->pool[mp->offset];
    void *p = mp->pool + mp->offset;    // 起始量 + 偏移量

    mp->offset += size;

    return p;
}


int main()
{
    MemPool mp;
    mp_init(&mp);

    int *a = mp_alloc(&mp, sizeof(int));
    printf("%lu\n", mp.offset);

    char *buf = mp_alloc(&mp, 16);
    printf("%lu\n", mp.offset);

    *a = 123;
    strcpy(buf, "memory pool");

    printf("%d\n%s\n", *a, buf);
    return 0;
}