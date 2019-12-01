#include "lzcnn_mat.h"

namespace lzcnn
{
    zMat::zMat()
        : data(0), refcount(0), elemsize(0), elempack(0), allocator(0), dims(0), w(0), h(0), c(0), cstep(0)
    {
    }

    zMat::zMat(int _w, size_t _elemsize, Allocator* _allocator)
        : data(0), refcount(0), elemsize(0), elempack(0), allocator(0), dims(0), w(0), h(0), c(0), cstep(0)
    {
        create(_w, _elemsize, _allocator);
    }

    zMat::zMat(int _w, int _h, size_t _elemsize, Allocator* _allocator)
        : data(0), refcount(0), elemsize(0), elempack(0), allocator(0), dims(0), w(0), h(0), c(0), cstep(0)
    {
        create(_w, _h, _elemsize, _allocator);
    }

    zMat::zMat(int _w, int _h, int _c, size_t _elemsize, Allocator* _allocator)
        : data(0), refcount(0), elemsize(0), elempack(0), allocator(0), dims(0), w(0), h(0), c(0), cstep(0)
    {
        create(_w, _h, _c, _elemsize, _allocator);
    }

    zMat::zMat(int _w, int _h, void* _data, size_t _elemsize, int _elempack, Allocator* _allocator)
        : data(_data), refcount(0), elemsize(_elemsize), elempack(_elempack), allocator(_allocator), dims(2), w(_w), h(_h), c(1)
    {
        cstep = w * h;
    }

    zMat::~zMat()
    {
        release();
    }

    void zMat::fill(float _v)
    {
        int size = (int)total();
        float* ptr = (float*)data;
        int remain = size;

        for (; remain > 0; remain--)
        {
            *ptr++ = _v;
        }

        return;
    }

    void zMat::fill(int _v)
    {
        int size = (int)total();
        int* ptr = (int*)data;
        int remain = size;

        for (; remain > 0; remain--)
        {
            *ptr++ = _v;
        }

        return;
    }

    void zMat::create(int _w, size_t _elemsize, Allocator* _allocator)
    {
        if (dims == 1 && w == _w && elemsize == _elemsize && elempack == 1 && allocator == _allocator)
            return;

        release();

        elemsize = _elemsize;
        elempack = 1;
        allocator = _allocator;

        dims = 1;
        w = _w;
        h = 1;
        c = 1;

        cstep = w;

        if (total() > 0)
        {
            size_t totalsize = alignSize(total() * elemsize, 4);
            if (allocator)
                data = allocator->fastMalloc(totalsize + (int)sizeof(*refcount));
            else
                data = fastMalloc(totalsize + (int)sizeof(*refcount));
            refcount = (int*)(((unsigned char*)data) + totalsize);
            *refcount = 1;
        }
    }

    void zMat::create(int _w, int _h, size_t _elemsize, Allocator* _allocator)
    {
        if (dims == 2 && w == _w && h == _h && elemsize == _elemsize && elempack == 1 && allocator == _allocator)
            return;

        release();

        elemsize = _elemsize;
        elempack = 1;
        allocator = _allocator;

        dims = 2;
        w = _w;
        h = _h;
        c = 1;

        cstep = w * h;

        if (total() > 0)
        {
            size_t totalsize = alignSize(total() * elemsize, 4);
            if (allocator)
                data = allocator->fastMalloc(totalsize + (int)sizeof(*refcount));
            else
                data = fastMalloc(totalsize + (int)sizeof(*refcount));
            refcount = (int*)(((unsigned char*)data) + totalsize);
            *refcount = 1;
        }
    }

    void zMat::create(int _w, int _h, int _c, size_t _elemsize, Allocator* _allocator)
    {
        if (dims == 3 && w == _w && h == _h && c == _c && elemsize == _elemsize && elempack == 1 && allocator == _allocator)
            return;

        release();

        elemsize = _elemsize;
        elempack = 1;
        allocator = _allocator;

        dims = 3;
        w = _w;
        h = _h;
        c = _c;

        cstep = alignSize(w * h * elemsize, 16) / elemsize;

        if (total() > 0)
        {
            size_t totalsize = alignSize(total() * elemsize, 4);
            if (allocator)
                data = allocator->fastMalloc(totalsize + (int)sizeof(*refcount));
            else
                data = fastMalloc(totalsize + (int)sizeof(*refcount));
            refcount = (int*)(((unsigned char*)data) + totalsize);
            *refcount = 1;
        }
    }

    void zMat::release()
    {
        if (refcount && NCNN_XADD(refcount, -1) == 1)
        {
            if (allocator)
                allocator->fastFree(data);
            else
                fastFree(data);
        }

        data = 0;

        elemsize = 0;
        elempack = 0;

        dims = 0;
        w = 0;
        h = 0;
        c = 0;

        cstep = 0;

        refcount = 0;
    }

    bool zMat::empty() const
    {
        return data == 0 || total() == 0;
    }

    size_t zMat::total() const
    {
        return cstep * c;
    }

    zMat zMat::channel(int _c)
    {
        return zMat(w, h, (unsigned char*)data + cstep * _c * elemsize, elemsize, elempack, allocator);
    }

    const zMat zMat::channel(int _c) const
    {
        return zMat(w, h, (unsigned char*)data + cstep * _c * elemsize, elemsize, elempack, allocator);
    }

    const float* zMat::row(int y) const
    {
        return (const float*)((unsigned char*)data + w * y * elemsize);
    }

    float& zMat::operator[](int i)
    {
        return ((float*)data)[i];
    }

    const float& zMat::operator[](int i) const
    {
        return ((const float*)data)[i];
    }

    zMat& zMat::operator=(const zMat& m)
    {
        if (this == &m)
            return *this;

        if (m.refcount)
            NCNN_XADD(m.refcount, 1);

        release();

        data = m.data;
        refcount = m.refcount;
        elemsize = m.elemsize;
        elempack = m.elempack;
        allocator = m.allocator;

        dims = m.dims;
        w = m.w;
        h = m.h;
        c = m.c;

        cstep = m.cstep;

        return *this;
    }
}