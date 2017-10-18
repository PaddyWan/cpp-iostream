#ifndef STREAMBUF_HPP
#define STREAMBUF_HPP

#include "streambuf.ipp"
#include "internal/streambuf.hpp"

class streambuf
{
    public:
        streambuf() = delete;

        static constexpr bool zoi(size_t x)
        {
            return x + 1 < 2;    //return true if value == 0 or -1 (for getn) (Zero Or Inverse one)
        }

        static constexpr size_t zoi_z(size_t x)
        {
            return zoi(x) ? 0 : x;
        }

        //basic functions
        template<IBuf ibuf>
        static void getb(ibuf&& b)
        {
            internal_streambuf::getb(b);
        }
        template<IBuf ibuf>
        static int getg(ibuf&& b)
        {
            return internal_streambuf::getg(b);
        }
        template<IBuf ibuf>
        static size_t getg(ibuf&& b, byte* d, size_t s)
        {
            return internal_streambuf::getg(b, d, s);
        }

        template<OBuf obuf>
        static void putf(obuf&& o)
        {
            internal_streambuf::putf(o);
        }
        template<OBuf obuf>
        static int putp(obuf&& s, byte c)
        {
            return internal_streambuf::putp(s, c);
        }
        template<OBuf obuf>
        static int putp(obuf&& b, const byte* d, size_t s)
        {
            return internal_streambuf::putp(b, d, s);
        }
        /*        template<IBuf ibuf>
                static size_t getg(ibuf&& b, uchar* d, size_t n)
                {
                    size_t l = b.geto(d, n);
                    while((!streambuf::zoi(l)) && l != n)
                    {
                        d += l;
                        n -= l;
                        l = b.geto(d, n);
                    }
                    return n - l;
                }

            template<class buf>
            static size_t putn(buf&& b, const uchar* d, size_t n)
            {
                size_t l = b.putn(b, d, n);
                while((!streambuf::zoi(l)) && l != n)
                {
                    d += l;
                    n -= l;
                    l = b.putn(b, d, n);
                }
                return n - l;
            }
        */
};

#endif//STREAMBUF_HPP
