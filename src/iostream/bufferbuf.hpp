#ifndef BUFFERBUF_HPP
#define BUFFERBUF_HPP

template<IBuf ibuf>
class ibufferbuf
{
    public:
        template<IBuf T>
        ibufferbuf(T&& i_, const size_t _c): i(std::forward<T>(i_)), b(new byte[_c]), c(_c), s(0), ii(0)
        {
        }
        ibufferbuf(ibufferbuf&& oth): i(std::forward<ibuf>(oth.i)), b(oth.b), c(oth.c), s(oth.s), ii(oth.ii)
        {
            oth.b = nullptr;
        }
        ~ibufferbuf()
        {
            if(b)
            {
                delete[] b;
            }
        }

        size_t geta()//available
        {
            return s - ii;
        }
        void getb()//bump
        {
            if(!geta())
                getf();
            ++ii;
        }
        void getb(size_t n)//bump
        {
            //todo test
            {
                const size_t m = std::min<size_t>(s - ii, n);
                ii += m;
                n -= m;
            }
            if(n)
            {
                getf();
                const size_t m = n / c;
                for(size_t j = 0; j < m; ++j)
                    getf();
                ii = n % c;
            }
        }
        void getf()
        {
            ii = 0;
            s = streambuf::getg(i, b, c);
        }
        int geti()
        {
            return (s == ii ? 1 : 0);
        }
        int getg()//get
        {
            if(!geta())
                getf();
            if(s)
            {
                const int ret = static_cast<int>(b[ii]);
                ++ii;
                return ret;
            }
            else
                return -1;
        }
//        size_t getg(byte* c, size_t s)//get
        size_t geto(byte* d, size_t n)//get
        {
            if(!geta())
                getf();
            const size_t m = std::min<size_t>(s - ii, n);
            std::copy(b + ii, b + ii + m, d);
            ii += m;
            return m;
        }
        int getp()//poll
        {
            if(!geta())
                getf();
            if(s)
                return static_cast<int>(b[ii]);
            else
                return -1;
        }
        size_t getp(byte* d, size_t n)//poll
        {
            if(!geta())
                getf();
            const size_t m = std::min<size_t>(s - ii, n);
            std::copy(b + ii, b + ii + m, d);
            return m;
        }
        //int getp()//peak
        byte* getx()//internal buffer
        {
            return b + ii;
        }
        size_t gety()//internal buffer
        {
            return s - ii;
        }
        size_t getc()//internal chunk size
        {
            return c;
        }
        size_t getz(byte* b_)
        {
            return streambuf::getg(i, b_, c);
        }

        //todo
        /*
                uchar* i_begin()
                {
                    return b;
                }
                uchar* i_index()
                {
                    return b + ii;
                }
                uchar* i_size()
                {
                    return b + s;
                }
                uchar* i_end()
                {
                    return b + c;
                }
                uchar* i_seek(ssize_t off)
                {
                    ii += off;
                    return i_index();
                }
                uchar* i_seek_size(ssize_t off)
                {
                    s += off;
                    return i_size();
                }
                uchar* i_pos(uchar* off)
                {
                    ii = off - b;
                    return i_index();
                }
                uchar* i_pos_size(uchar* off)
                {
                    s = off - b;
                    return i_size();
                }
        */
    protected:
        void reset()
        {
            s = 0;
            ii = 0;
        }
        ibuf i;
        byte* b;
        const size_t c;
        size_t s;
        size_t ii;
};

template<class obuf>
class obufferbuf
{
    public:
        template<class T>
        obufferbuf(T&& o_, const size_t _c): o(std::forward<T>(o_)), b(new byte[_c]),  c(_c), i(0)
        {
        }
        obufferbuf(obufferbuf&& oth): o(std::forward<obuf>(oth.o)), b(oth.b),  c(oth.c), i(oth.i)
        {
            oth.b = nullptr;
            oth.i = 0;//we don't flush this sucker
        }

        ~obufferbuf()
        {
            if(b)
            {
                if(i != 0)
                    putf();
                delete[] b;
            }
        }

        void putb()
        {
            if(!puty())
                putf();
            ++i;
        }
        void putb(size_t n)
        {
            //todo test
            {
                const size_t m = std::min<size_t>(puty(), n);
                i += m;
                n -= m;
            }
            if(n)
            {
                putf(c);
                const size_t m = n / c;
                for(size_t j = 0; j < m; ++j)
                    putf(c);
                i = n % c;
            }
        }
        void putd()
        {
            if(!puty())
                putf();
            b[i] = 0;
            ++i;
        }
        void putd(size_t n)
        {
            //todo test
            {
                const size_t m = std::min<size_t>(puty(), n);
                fill(b + i, b + i + m, 0);
                i += m;
                n -= m;
            }
            if(n)
            {
                putf(c);
                fill(b, b + c, 0);
                const size_t m = n / c;
                for(size_t j = 0; j < m; ++j)
                    putf(c);
                i = n % c;
            }
        }
        int puti()
        {
            return 0;//can't we always write?
        }
        void putf()
        {
            putf(i);
            i = 0;
        }
        int putp(byte c)
        {
            if(!puty())
                putf();
            b[i] = c;
            ++i;
            return static_cast<int>(c);
        }
        //size_t putp(const byte* c, size_t s)
        size_t puto(const byte* c, size_t n)
        {
            if(!puty())
                putf();
            const size_t m = std::min<size_t>(puty(), n);
            copy(c, c + m, b + i);
            i += m;
            return m;
        }
        byte* putx()//internal buffer
        {
            return b;
        }
        size_t puty()
        {
            return c - i;
        }
        size_t putc()
        {
            return c;
        }
        size_t putz(const byte* b_)
        {
            return streambuf::putp(o, b_, c);
        }
        /*
            void flush_special(size_t offset, size_t size)
            {
                i = 0;
                o.putn(o, b + offset, size);;
            }

            uchar* o_begin()
            {
                return b;
            }
            uchar* o_index()
            {
                return b + i;
            }
            uchar* o_end()
            {
                return b + c;
            }
            uchar* o_seek(ssize_t off)
            {
                i += off;
                return o_index();
            }
            uchar* o_pos(uchar* off)
            {
                i = off - b;
                return o_index();
            }*/
    protected:
        void putf(size_t s)
        {
            streambuf::putp(o, b, s);
        }
        void reset()
        {
            i = 0;
        }
        obuf o;
        byte* b;
        const size_t c;
        size_t i;
};

template<class iobuf>
class bufferbuf: public ibufferbuf<iobuf&>, public obufferbuf<iobuf&>
{
    public:
        template<class T>
        bufferbuf(T&& io, const size_t _c):                    ibufferbuf<iobuf & >(get(std::forward<T>(io), iob), _c), obufferbuf<iobuf & >(get(std::forward<T>(io), iob), _c), iob(std::forward<T>(io))
        {
        }
        template<class T>
        bufferbuf(T&& io, const size_t _ci, const size_t _co): ibufferbuf<iobuf & >(get(std::forward<T>(io), iob), _ci), obufferbuf<iobuf & >(get(std::forward<T>(io), iob), _co), iob(std::forward<T>(io))
        {
        }
        bufferbuf(bufferbuf&& oth):                            ibufferbuf<iobuf & >(get(oth.iob, iob), move(oth)), obufferbuf<iobuf & >(get(oth.iob, iob), move(oth)), iob(move(oth.iob))
        {
        }
        template<class T>
        void rebase(T&& io)
        {
            iob = std::forward<T>(io);
        }
        iobuf&& movebase()
        {
            return std::move(iob);
        }
        template<class T>
        void swapbase(T&& io)
        {
            std::swap(io, iob);
        }
        void reset()
        {
            ibufferbuf<iobuf&>::reset();
            obufferbuf<iobuf&>::reset();
        }

    protected:
        iobuf iob;

        template<class T>
        static iobuf& get(T&& io1, iobuf& io2)
        {
            if(std::is_lvalue_reference<iobuf>::value)
                return io1;
            else
                return io2;
        }
};

#endif//BUFFERBUF_HPP
