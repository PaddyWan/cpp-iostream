#ifndef STREAMBUF_IPP
#define STREAMBUF_IPP

#include <cstddef>

#include "../std.hpp"

using std::byte;

//IBuf

template<class T>
concept bool SingleGetIBuf = requires(T x)
{
    {
        x.getg()
    }
    -> int;
};
template<class T>
concept bool MultiGetIBuf = requires(T x, byte* b, size_t s)
{
    {
        x.getg(b, s)
    }
    -> size_t;
};
template<class T>
concept bool GetIBuf = SingleGetIBuf<T> || MultiGetIBuf<T>;
template<class T>
concept bool SinglePollIBuf = requires(T x)
{
    {
        x.getp()
    }
    -> int;
};
template<class T>
concept bool MultiPollIBuf = requires(T x, byte* b, size_t s)
{
    {
        x.getp(b, s)
    }
    -> size_t;
};
template<class T>
concept bool PollIBuf = SinglePollIBuf<T> || MultiPollIBuf<T>;
template<class T>
concept bool SingleBumpIBuf = requires(T x)
{
    {
        x.getb()
    };
};
template<class T>
concept bool MultiBumpIBuf = requires(T x, size_t s)
{
    {
        x.getb(s)
    };
};
template<class T>
concept bool BumpIBuf = SingleBumpIBuf<T> || MultiBumpIBuf<T>;
template<class T>
concept bool OptimizedIBuf = requires(T x, byte* b, size_t s)
{
    {
        x.geto(b, s)
    }
    -> size_t;
};
template<class T>
concept bool IBuf = GetIBuf<T> || OptimizedIBuf<T> || (PollIBuf<T>&& BumpIBuf<T>);

template<class T>
concept bool SinglePeakIBuf = requires(T x)
{
    {
        x.gete()
    }
    -> int;
};
template<class T>
concept bool MultiPeakIBuf = requires(T x, byte* b, size_t s)
{
    {
        x.gete(b, s)
    }
    -> size_t;
};
template<class T>
concept bool PeakIBuf = SinglePeakIBuf<T> || MultiPeakIBuf<T>;
template<class T>
concept bool AvailableIBuf = requires(T x)
{
    {
        x.geta()
    }
    -> size_t;
};
template<class T>
concept bool FillIBuf = requires(T x)
{
    {
        x.getf()
    };
};
template<class T>
concept bool InvestigateIBuf = requires(T x)
{
    {
        x.geti()
    }
    -> int;
};
template<class T>
concept bool InternalBufferArrayIBuf = requires(T x)
{
    {
        x.getx()
    }
    -> byte*;
};
template<class T>
concept bool InternalBufferSizeIBuf = requires(T x)
{
    {
        x.gety()
    }
    -> size_t;
};
template<class T>
concept bool ChunkSizeIBuf = requires(T x)
{
    {
        x.getc()
    }
    -> size_t;
};
template<class T>
concept bool ChunkGetIBuf = requires(T x, byte* b)
{
    {
        x.getz(b)
    }
    -> size_t;
};


template<class T>
concept bool SinglePutOBuf = requires(T x, byte b)
{
    {
        x.putp(b)
    }
    -> int;
};
template<class T>
concept bool MultiPutOBuf = requires(T x, const byte* b, size_t s)
{
    {
        x.putp(b, s)
    }
    -> size_t;
};
template<class T>
concept bool PutOBuf = SinglePutOBuf<T> || MultiPutOBuf<T>;
template<class T>
concept bool OptimizedOBuf = requires(T x, const byte* b, size_t s)
{
    {
        x.puto(b, s)
    }
    -> size_t;
};
template<class T>
concept bool OBuf = PutOBuf<T> || OptimizedOBuf<T>;

template<class T>
concept bool SingleBumpOBuf = requires(T x)
{
    {
        x.putb()
    };
};
template<class T>
concept bool MultiBumpOBuf = requires(T x, size_t s)
{
    {
        x.putb(s)
    };
};
template<class T>
concept bool BumpOBuf = SingleBumpOBuf<T> || MultiBumpOBuf<T>;
template<class T>
concept bool SinglePadOBuf = requires(T x)
{
    {
        x.putd()
    };
};
template<class T>
concept bool MultiPadOBuf = requires(T x, size_t s)
{
    {
        x.putd(s)
    };
};
template<class T>
concept bool PadOBuf = SinglePadOBuf<T> || MultiPadOBuf<T>;
template<class T>
concept bool InvestigateOBuf = requires(T x)
{
    {
        x.puti()
    }
    -> int;
};
template<class T>
concept bool FlushOBuf = requires(T x)
{
    {
        x.putf()
    };
};
template<class T>
concept bool InternalBufferArrayOBuf = requires(T x)
{
    {
        x.putx()
    }
    -> byte*;
};
template<class T>
concept bool InternalBufferSizeOBuf = requires(T x)
{
    {
        x.puty()
    }
    -> size_t;
};
template<class T>
concept bool ChunkSizeOBuf = requires(T x)
{
    {
        x.putc()
    }
    -> size_t;
};
template<class T>
concept bool ChunkPutOBuf = requires(T x, const byte* b)
{
    {
        x.putz(b)
    };
};

#endif//STREAMBUF_IPP
