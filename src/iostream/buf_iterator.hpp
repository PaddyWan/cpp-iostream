#ifndef BUF_ITERATOR_HPP
#define BUF_ITERATOR_HPP

#include <iterator>

template<IBuf ibuf>
class ibuf_iterator
{
};


template<OBuf obuf>
class obuf_iterator
{
    public:
        obuf_iterator(obuf& o_): o(o_)
        {}
        obuf_iterator operator=(std::byte c) const
        {
            o.putc(c);
            return *this;
        }
        obuf_iterator operator*() const
        {
            return *this;
        }
        obuf_iterator& operator++()
        {
            return *this;
        }
        obuf_iterator& operator++(int)
        {
            return *this;
        }

    private:
        obuf& o;
};

#include "internal/buf_iterator.hpp"

#endif//BUF_ITERATOR_HPP
