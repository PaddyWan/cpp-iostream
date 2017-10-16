#ifndef BASICSTREAM_HPP
#define BASICSTREAM_HPP

#include "streambuf.hpp"
#include "utilstream.hpp"
#include "../string.hpp"

template <IBuf ibuf>
class basicistream
{
        ibuf& i;
    public:
        basicistream(ibuf& i_): i(i_) {}
        basicistream(ibuf&& i_): i(i_) {}
        template<class Primitive>
        void get(Primitive& ret) //reads sizeof(Primitive) bytes
        {
            utilstream::btoi(i, ret);
        }
        template<class Primitive>
        void read(Primitive& ret) //reads string (with trimming before)
        {
            utilstream::dtoi(i, ret);
        }
        template<String stringT>
        void readWord(stringT& str)
        {
            int temp;
            utilstream::dtoa(i, string_inserter(str), (int(*)(int))&std::isspace, temp);
        }
        template<String stringT>
        void readLine(stringT& str)
        {
            int temp;
            utilstream::dtoa(i, string_inserter(str), '\n', temp);
        }
        template<class Primitive>
        basicistream& operator>>(Primitive& ret)
        {
            read(ret);
            return *this;
        }
        template<String stringT>
        basicistream& operator>>(stringT& str)
        {
            readLine(str);
            return *this;
        }
};
template <OBuf obuf>
class basicostream
{
        obuf& o;
    public:
        basicostream(obuf& o_): o(o_) {}
        basicostream(obuf&& o_): o(o_) {}
        template<class Primitive>
        void put(Primitive p) //writes sizeof(Primitive) bytes
        {
            utilstream::itob(o, p);
        }
        template<class Primitive>
        void write(Primitive p) //writes as string
        {
            utilstream::itod(o, p);
        }
        void write(char p) //writes as string
        {
            streambuf::putp(o, static_cast<std::byte>(p));
        }
        template<String stringT>
        void write(const stringT& str)
        {
            utilstream::atob(o, str);
        }
        template<class Primitive>
        basicostream& operator<<(Primitive p)
        {
            write(p);
            return *this;
        }
        template<String stringT>
        basicostream& operator<<(const stringT& str)
        {
            write(str);
            return *this;
        }
};

template <IBuf ibuf>
using istream = basicistream<ibuf>;
template <OBuf obuf>
using ostream = basicostream<obuf>;
template <IBuf ibuf>
istream<ibuf> make_istream(ibuf& i)
{
    return istream<ibuf>(i);
}
template <OBuf obuf>
ostream<obuf> make_ostream(obuf& o)
{
    return ostream<obuf>(o);
}

#endif//BASICSTREAM_HPP
