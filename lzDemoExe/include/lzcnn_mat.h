#ifndef LZCNN_MAT_H
#define LZCNN_MAT_H

#include "lzcnn_allocator.h"

namespace lzcnn
{
    // the three dimension matrix
    class zMat
    {
    public:
        // empty
        zMat();
        // vec
        zMat(int w, size_t elemsize = 4u, Allocator* allocator = 0);
        // image
        zMat(int w, int h, size_t elemsize = 4u, Allocator* allocator = 0);
        // dim
        zMat(int w, int h, int c, size_t elemsize = 4u, Allocator* allocator = 0);
        // external packed image
        zMat(int w, int h, void* data, size_t elemsize, int elempack, Allocator* allocator = 0);
        // release
        ~zMat();

        // set all
        void fill(float v);
        void fill(int v);

        // allocate vec
        void create(int w, size_t elemsize = 4u, Allocator* allocator = 0);
        // allocate image
        void create(int w, int h, size_t elemsize = 4u, Allocator* allocator = 0);
        // allocate dim
        void create(int w, int h, int c, size_t elemsize = 4u, Allocator* allocator = 0);

        // refcount--
        void release();

        bool empty() const;
        size_t total() const;

        zMat channel(int _c);
        const zMat channel(int c) const;
        const float* zMat::row(int y) const;

        // access raw data
        template<typename T> operator T*();
        template<typename T> operator const T*() const;

        // convenient access float vec element
        float& operator[](int i);
        const float& operator[](int i) const;
        // assign
        zMat& operator=(const zMat& m);

        // pointer to the data
        void* data;

        // pointer to the reference counter
        // when points to user-allocated data, the pointer is NULL
        int* refcount;

        // element size in bytes
        // 4 = float32/int32
        // 2 = float16
        // 1 = int8/uint8
        // 0 = empty
        size_t elemsize;

        // packed count inside element
        // c/1-h-w-1  h/1-w-1  w/1-1  scalar
        // c/4-h-w-4  h/4-w-4  w/4-4  sse/neon
        // c/8-h-w-8  h/8-w-8  w/8-8  avx/fp16
        int elempack;

        // the allocator
        Allocator* allocator;

        // the dimensionality
        int dims;

        int w;
        int h;
        int c;

        size_t cstep;
    };

    template <typename T>
    zMat::operator T*()
    {
        return (T*)data;
    }

    template <typename T>
    zMat::operator const T*() const
    {
        return (const T*)data;
    }
}

#endif
