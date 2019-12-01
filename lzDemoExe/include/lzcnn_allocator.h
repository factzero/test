#ifndef LZCNN_ALLOCATOR_H
#define LZCNN_ALLOCATOR_H

namespace lzcnn
{
    #include <intrin.h>
    #define NCNN_XADD(addr, delta) (int)_InterlockedExchangeAdd((long volatile*)addr, delta)

    #define MALLOC_ALIGN    16

    static inline size_t alignSize(size_t sz, int n)
    {
        return (sz + n - 1) & -n;
    }

    static inline void* fastMalloc(size_t size)
    {
        return _aligned_malloc(size, MALLOC_ALIGN);
    }

    static inline void fastFree(void* ptr)
    {
        if (ptr)
        {
            _aligned_free(ptr);
        }

        return;
    }

    class Allocator
    {
    public:
        virtual ~Allocator() {}
        virtual void* fastMalloc(size_t size) = 0;
        virtual void fastFree(void* ptr) = 0;
    };
}

#endif