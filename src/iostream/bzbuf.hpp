#ifndef BZBUF_HPP
#define BZBUF_HPP

#include <bzlib.h>

#include "strmbuf.hpp"

struct obzstrm
{
    typedef int generic_t;
    void init(int level)
    {
        strm.bzalloc = nullptr;
        strm.bzfree = nullptr;
        strm.opaque = nullptr;
        BZ2_bzCompressInit(&strm, level, 0, 30);
        strm.avail_in = 0;
    }
    void end()
    {
        BZ2_bzCompressEnd(&strm);
    }
    int no_flush()
    {
        return BZ_RUN;
    }
    int finish()
    {
        return BZ_FINISH;
    }
    void deflate(int flush)
    {
        BZ2_bzCompress(&strm, flush);
    }
    bz_stream strm;
};
struct ibzstrm
{
    void init()
    {
        strm.bzalloc = nullptr;
        strm.bzfree = nullptr;
        strm.opaque = nullptr;
        strm.avail_in = 0;
        strm.next_in = nullptr;
        BZ2_bzDecompressInit(&strm, 0, 0);
    }
    void end()
    {
        BZ2_bzDecompressEnd(&strm);
    }
    void inflate()
    {
        BZ2_bzDecompress(&strm);
    }
    bz_stream strm;
};

template<class obuf, size_t CAP = 262144>
class obzbuf: public ostrmbuf<obuf, CAP, obzstrm>
{
    public:
        template<class T>
        obzbuf(T&& o_, int level = 9): ostrmbuf<obuf, CAP, obzstrm>(std::forward<T>(o_), level)
        {
        }
};


template<class ibuf, size_t CAP = 262144>
class ibzbuf: public istrmbuf<ibuf, CAP, ibzstrm>
{
    public:
        template<class T>
        ibzbuf(T&& i_): istrmbuf<ibuf, CAP, ibzstrm>(std::forward<T>(i_))
        {
        }
};

#endif//BZBUF_HPP
