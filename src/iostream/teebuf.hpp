#ifndef TEEBUF_HPP
#define TEEBUF_HPP

#include "streambuf.hpp"

template<class obuf1, class obuf2>
class teebuf//always an output
{
    public:

        explicit teebuf(obuf1&& o1_, obuf2&& o2_): o1(o1_), o2(o2_)
        {
        }

        int putp(byte c)
        {
            return min(streambuf::putp(o1, c), streambuf::putp(o2, c));
        }
    protected:
        obuf1 o1;
        obuf2 o2;
};

#endif//TEEBUF_HPP
