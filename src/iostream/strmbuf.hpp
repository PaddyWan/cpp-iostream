#ifndef STRMBUF_HPP
#define STRMBUF_HPP

#include "strmbuf.ipp"

template<OBuf obuf, size_t CAP, Ostrm ostrm>
class ostrmbuf
{
    public:
        template<class T>
        ostrmbuf(T&& o_, int level): o(std::forward<T>(o_))
        {
            strm.init(level);
        }
        ~ostrmbuf()
        {
            flush_zip(strm.finish());
            strm.end();
        }
        size_t putp(const byte* d, size_t n)
        {
            strm.strm.avail_in = n;
            strm.strm.next_in = reinterpret_cast<typeof(strm.strm.next_in)>(const_cast<byte*>(d));
            flush_input(strm.no_flush());
            return n;
        }
    protected:
        obuf o;
        byte b[CAP];
        ostrm strm;
        void flush_input(typename ostrm::generic_t flush)
        {
            while(strm.strm.avail_in != 0)
                flush_zip(flush);
        }
        void flush_zip(typename ostrm::generic_t flush)
        {
            strm.strm.avail_out = CAP;
            strm.strm.next_out = reinterpret_cast<typeof(strm.strm.next_out)>(b);
            strm.deflate(flush);
            const size_t have = CAP - strm.strm.avail_out;
            streambuf::putp(o, b, have);
        }
};

template<IBuf ibuf, size_t CAP, Istrm istrm>
class istrmbuf
{
    public:
        template<class T>
        istrmbuf(T&& i_): i(std::forward<T>(i_))
        {
            strm.init();
        }
        ~istrmbuf()
        {
            strm.end();
        }
        size_t getg(byte* d, size_t n)
        {
            strm.strm.avail_out = n;
            strm.strm.next_out = reinterpret_cast<typeof(strm.strm.next_out)>(d);
            flush_zip();
            return n - strm.strm.avail_out;
        }
    protected:
        ibuf i;
        byte b[CAP];//data in strm
        istrm strm;
        void flush_zip()
        {
            if(strm.strm.avail_in != 0)
                strm.inflate();
            while(strm.strm.avail_in == 0 && !streambuf::zoi(i.geta()))
            {
                strm.strm.avail_in = streambuf::getg(i, b, CAP);
                strm.strm.next_in = reinterpret_cast<typeof(strm.strm.next_in)>(b);
                strm.inflate();
            }
        }
};

#endif//STRMBUF_HPP
