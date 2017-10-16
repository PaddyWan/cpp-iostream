#ifndef ZBUF_HPP
#define ZBUF_HPP

#include <zlib.h>

#include "strmbuf.hpp"

struct ozstrm
{
    typedef int generic_t;
    void init(int level)
    {
        strm.zalloc = nullptr;
        strm.zfree = nullptr;
        strm.opaque = nullptr;
        deflateInit(&strm, level);
        strm.avail_in = 0;
    }
    void end()
    {
        deflateEnd(&strm);
    }
    int no_flush()
    {
        return Z_NO_FLUSH;
    }
    int finish()
    {
        return Z_FINISH;
    }
    void deflate(int flush)
    {
        ::deflate(&strm, flush);
    }
    z_stream strm;
};
struct izstrm
{
    void init()
    {
        strm.zalloc = nullptr;
        strm.zfree = nullptr;
        strm.opaque = nullptr;
        strm.avail_in = 0;
        strm.next_in = nullptr;
        inflateInit(&strm);
    }
    void end()
    {
        inflateEnd(&strm);
    }
    void inflate()
    {
        ::inflate(&strm, Z_NO_FLUSH);
    }
    z_stream strm;
};

template<class obuf, size_t CAP = 262144>
class ozbuf: public ostrmbuf<obuf, CAP, ozstrm>
{
    public:
        template<class T>
        ozbuf(T&& o_, int level = 9): ostrmbuf<obuf, CAP, ozstrm>(std::forward<T>(o_), level)
        {
        }
};
template<class ibuf, size_t CAP = 262144>
class izbuf: public istrmbuf<ibuf, CAP, izstrm>
{
    public:
        template<class T>
        izbuf(T&& i_): istrmbuf<ibuf, CAP, izstrm>(std::forward<T>(i_))
        {
        }
};

#endif//ZBUF_HPP
