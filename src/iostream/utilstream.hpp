#ifndef UTILSTREAM_HPP
#define UTILSTREAM_HPP

#include "../string.hpp"
#include "buf_iterator.hpp"
#include "stringbuf.hpp"
#include "internal/utilstream.hpp"

class utilstream
{
    public:
        utilstream() = delete;

        template<IBuf ibuf, OBuf obuf>
        static void copy(ibuf&& i, obuf&& o)
        {
            copy(i, o, std::allocator<std::byte>());
        }
        template<IBuf ibuf, OBuf obuf, class Allocator>
        static void copy(ibuf&& i, obuf&& o, Allocator&& alloc)
        {
            internal_utilstream::btob(i, o, alloc);
        }
        template<IBuf ibuf>
        static stringbuf buffer(ibuf&& i)
        {
            const size_t a = i.geta();
            stringbuf str(max<size_t>(16, streambuf::zoi(a) ? 16 : a));
            internal_utilstream::btob(i, str);
            return str;
        }

        //b binary
        //d delimited ascii string
        //a ascii string
        //i int
        //return number of bytes not succesfull readed/written, 0 => succes, or else specified
        template<IBuf ibuf, OBuf obuf>
        static void btob(ibuf&& i, obuf&& o)
        {
            btob(i, o, std::allocator<std::byte>());
        }
        //read
        template<IBuf ibuf, OBuf obuf, class Allocator>
        static void btob(ibuf&& i, obuf&& o, Allocator&& alloc)
        {
            internal_utilstream::btob(i, o, alloc);
        }
        template<IBuf ibuf, class tint>
        static void btoi(ibuf&& i, tint& r)
        {
            internal_utilstream::btoi(i, r);
        }
        //stoi will try to read the character after the int to see if it's ended!
        //example, a tcpsocketstream that ends with a int without an extra delimiter after it, will wait for the next tcpdata
        template<IBuf ibuf, class tint>
        static void dtoi(ibuf&& i, tint& r, int& lastchar)
        {
            internal_utilstream::dtoi(i, r, lastchar);
        }
        template<IBuf ibuf, class OutputIt>
        static void dtoa(ibuf&& i, OutputIt it, char delim, int& lastchar)
        {
            internal_utilstream::dtoa(i, it, delim, lastchar);
        }
        template<IBuf ibuf, class OutputIt>
        static void dtoa(ibuf&& i, OutputIt it, char delim1, char delim2, int& lastchar)//delim[0..255] true if it is a delimiter
        {
            internal_utilstream::dtoa(i, it, delim1, delim2, lastchar);
        }
        template<IBuf ibuf, class OutputIt>
        static void dtoa(ibuf&& i, OutputIt it, const bool* const delim, int& lastchar)//delim[0..255] true if it is a delimiter
        {
            internal_utilstream::dtoa(i, it, delim, lastchar);
        }
        template<IBuf ibuf, class OutputIt, class Func>
        static void dtoa(ibuf&& i, OutputIt it, Func f, int& lastchar)//(int(*)(int))&std::isspace or std::not1(std::ptr_fun((int(*)(int))&std::isspace))
        {
            internal_utilstream::dtoa(i, it, f, lastchar);
        }

        //write
        template<OBuf obuf, class tint>
        static size_t itob(obuf&& b, tint r)
        {
            return internal_utilstream::itob(b, r);
        }
        template<OBuf obuf, class tint>
        static size_t itod(obuf&& b, tint r)//return number of bytes written
        {
            return internal_utilstream::itod(b, r);
        }
        template<OBuf obuf, String stringT>
        static size_t atob(obuf&& b, const stringT& str)
        {
            return internal_utilstream::atob(b, str);
        }

        /*
                    //oldfunctions
                    //use of peak
                        template<class buf>
                        static void ignore(buf&& b, std::byte ignoreme)
                        {
                        int r = b.gete(b);
                            while(r == to_integer(ignoreme) && r != -1)
                        {
                                b.bump(b, 1);
                    	r = b.gete(b);
                        }
                    }
                        template<class buf, class Func>
                        static void ignore(buf&& b, Func f)
                        {
                        int r = b.gete(b);
                            while(f(r) && r != -1)
                        {
                                b.bump(b, 1);
                    	r = b.gete(b);
                        }
                        }
        */
};

#endif//UTILSTREAM_HPP
