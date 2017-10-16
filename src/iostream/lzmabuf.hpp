#ifndef LZMABUF_HPP
#define LZMABUF_HPP

#include <lzma.h>

#include "strmbuf.hpp"

struct olzmastrm
{
    typedef lzma_action generic_t;
    void init(int level)
    {
        strm = LZMA_STREAM_INIT;
        lzma_ret ret = lzma_easy_encoder(&strm, level, LZMA_CHECK_CRC64);
        if(ret == LZMA_OK)
        {}
        strm.avail_in = 0;
    }
    void end()
    {
        lzma_end(&strm);
    }
    lzma_action no_flush()
    {
        return LZMA_RUN;
    }
    lzma_action finish()
    {
        return LZMA_FINISH;
    }
    void deflate(lzma_action flush)
    {
        lzma_ret ret = lzma_code(&strm, flush);
        if(ret == LZMA_OK)
        {}
    }
    lzma_stream strm;
};
struct ilzmastrm
{
    void init()
    {
        strm = LZMA_STREAM_INIT;
        lzma_ret ret = lzma_stream_decoder(&strm, UINT64_MAX, LZMA_CONCATENATED);
        if(ret == LZMA_OK)
        {}
    }
    void end()
    {
        lzma_end(&strm);
    }
    void inflate()
    {
        lzma_ret ret = lzma_code(&strm, LZMA_RUN);
        if(ret == LZMA_OK)
        {}
    }
    lzma_stream strm;
};

template<class obuf, size_t CAP = 262144>
class olzmabuf: public ostrmbuf<obuf, CAP, olzmastrm>
{
    public:
        template<class T>
        olzmabuf(T&& o_, int level = 9): ostrmbuf<obuf, CAP, olzmastrm>(std::forward<T>(o_), level)
        {
        }
};
template<class ibuf, size_t CAP = 262144>
class ilzmabuf: public istrmbuf<ibuf, CAP, ilzmastrm>
{
    public:
        template<class T>
        ilzmabuf(T&& i_): istrmbuf<ibuf, CAP, ilzmastrm>(std::forward<T>(i_))
        {
        }
};

#endif//LZMABUF_HPP
