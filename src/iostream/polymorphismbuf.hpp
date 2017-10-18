#ifndef POLYMORPHISMBUF_HPP
#define POLYMORPHISMBUF_HPP

#include "streambuf.hpp"

class ipolymorphismbuf
{
    public:
        struct placeholderibuf;
        ipolymorphismbuf(placeholderibuf* ptr_ = nullptr): ptr(ptr_)
        {}
        ipolymorphismbuf(ipolymorphismbuf&& buf): ptr(buf.ptr)
        {
            buf.ptr = nullptr;
        }
        ~ipolymorphismbuf()
        {
            delete ptr;
        }
        ipolymorphismbuf& operator=(ipolymorphismbuf&& buf)
        {
            delete ptr;
            ptr = buf.ptr;
            buf.ptr = nullptr;
            return *this;
        }
        int getg()
        {
            return ptr->getg();
        }
        struct placeholderibuf
        {
            virtual ~placeholderibuf() {}
            virtual int getg() = 0;
        };
        template<IBuf ibuf>
        struct implementationibuf: placeholderibuf
        {
            implementationibuf(ibuf&& buf_): buf(std::forward<ibuf>(buf_)) {}
            int getg()
            {
                return streambuf::getg(buf);
            }
            ibuf buf;
        };
        placeholderibuf* ptr;
};

class opolymorphismbuf
{
    public:
        struct placeholderobuf;
        opolymorphismbuf(placeholderobuf* ptr_ = nullptr): ptr(ptr_)
        {}
        opolymorphismbuf(opolymorphismbuf&& buf): ptr(buf.ptr)
        {
            buf.ptr = nullptr;
        }
        ~opolymorphismbuf()
        {
            delete ptr;
        }
        opolymorphismbuf& operator=(opolymorphismbuf&& buf)
        {
            delete ptr;
            ptr = buf.ptr;
            buf.ptr = nullptr;
            return *this;
        }

        template<class S>
        int putp(byte c)
        {
            return ptr->putp(c);
        }

        struct placeholderobuf
        {
            virtual ~placeholderobuf() {}
            virtual int putp(byte c) = 0;
        };
        template<OBuf obuf>
        struct implementationobuf: placeholderobuf
        {
            implementationobuf(obuf&& buf_): buf(std::forward<obuf>(buf_)) {}
            int putp(byte c)
            {
                return streambuf::putp(buf, c);
            }
            obuf buf;
        };
        placeholderobuf* ptr;
};

template<IBuf ibuf, class T>
ipolymorphismbuf make_ipolymorphismbuf(T&& buf)
{
    return ipolymorphismbuf(reinterpret_cast<ipolymorphismbuf::placeholderibuf*>(new ipolymorphismbuf::implementationibuf<ibuf>(std::forward<T>(buf))));
}
template<OBuf obuf, class T>
opolymorphismbuf make_opolymorphismbuf(T&& buf)
{
    return opolymorphismbuf(reinterpret_cast<opolymorphismbuf::placeholderobuf*>(new opolymorphismbuf::implementationobuf<obuf>(std::forward<T>(buf))));
}

#endif//POLYMORPHISMBUF_HPP
