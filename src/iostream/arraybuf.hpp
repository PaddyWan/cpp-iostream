#ifndef ARRAYBUF_HPP
#define ARRAYBUF_HPP

#include <algorithm>

#include "../string.hpp"

using std::min;
using std::copy;
using std::move;

template<size_t N>
class arraybuf
{
    public:
        arraybuf(): r(b), w(b)
        {}

//ibuf
        size_t geta()//available
        {
            return distance(r, w);
        }
        void getb()//bump
        {
            ++r;
        }
        void getb(size_t n)//bump
        {
            r += n;
        }
        int geti()
        {
            return r == w;
        }
        int getg()//get
        {
            if(r == w)
                return -1;
            else
            {
                const int ret = std::to_integer<int>(*r);
                ++r;
                return ret;
            }
        }
        size_t getg(byte* c, size_t s)//get
        {
            const size_t m = min<size_t>(distance(r, w), s);
            copy(r, r + m, c);
            r += m;
            return m;
        }
        int getp()//poll
        {
            if(r == w)
                return -1;
            else
                return std::to_integer<int>(*r);
        }
        size_t getp(byte* c, size_t s)//poll
        {
            const size_t m = min<size_t>(distance(r, w), s);
            copy(r, r + m, c);
            return m;
        }
        byte* getx()//internal buffer
        {
            return r;
        }
        size_t gety()//internal buffer
        {
            return distance(r, w);
        }

//obuf
        void putb()
        {
            ++w;
        }
        void putb(size_t n)
        {
            w += n;
        }
        void putd()
        {
            *w = 0;
            ++w;
        }
        void putd(size_t n)
        {
            fill(w, w + n, 0);
            w += n;
        }
        int puti()
        {
            shrink(1);
            return w == (b + N) ? 1 : 0;
        }
        int putp(byte c)
        {
            shrink(1);
            if(puty() < 1)
                return -1;
            *w = c;
            ++w;
            return std::to_integer<int>(c);
        }
        size_t putp(const byte* c, size_t s)
        {
            shrink(s);
            const size_t m = min<size_t>(distance(w, b + N), s);
            copy(c, c + m, w);
            w += m;
            return m;
        }
        size_t puto(const byte* c, size_t s)
        {
            shrink(1);
            const size_t m = min<size_t>(distance(w, b + N), s);
            copy(c, c + m, w);
            w += m;
            return m;
        }
        byte* putx()//internal buffer
        {
            return w;
        }
        size_t puty()
        {
            return distance(w, b + N);
        }

//container
        bool empty() const
        {
            return r == w;
        }
        size_t size() const
        {
            return distance(r, w);
        }
        size_t capacity() const
        {
            return N;
        }
        byte* begin()
        {
            return r;
        }
        byte* end()
        {
            return w;
        }
        const byte* begin() const
        {
            return r;
        }
        const byte* end() const
        {
            return w;
        }
        byte* real_begin()
        {
            return b;
        }
        byte* real_end()
        {
            return b + N;
        }
        const byte* real_begin() const
        {
            return b;
        }
        const byte* real_end() const
        {
            return b + N;
        }
        string str() const
        {
            return string(reinterpret_cast<const char*>(r), reinterpret_cast<const char*>(w));
        }
        void str(const string& str)
        {
            r = b;
            const size_t m = min<size_t>(N, str.size());
            w = b + m;
            copy(str.data(), str.data() + m, b);
        }

//std::string
        const char* data() const
        {
            return reinterpret_cast<const char*>(r);
        }
        char* data()
        {
            return reinterpret_cast<char*>(r);
        }
        void clear()
        {
            r = w = b;
        }
        char& front()
        {
            return *reinterpret_cast<char*>(r);
        }
        const char& front() const
        {
            return *reinterpret_cast<const char*>(r);
        }
        char& back()
        {
            return *reinterpret_cast<char*>(w - 1);
        }
        const char& back() const
        {
            return *reinterpret_cast<const char*>(w - 1);
        }
        void pop_back()
        {
            --w;
        }
        void pop_front()
        {
            ++r;
        }
        void push_back(char c)
        {
            putp(c);
        }
        char& operator[](size_t pos)
        {
            return reinterpret_cast<char*>(r)[pos];
        }
        const char& operator[](size_t pos) const
        {
            return reinterpret_cast<const char*>(r)[pos];
        }

        //trim
        void trim()
        {
            trim((int(*)(int))&std::isspace);
        }
        template<class Func>
        void trim(Func f)//(int(*)(int))&std::isspace
        {
            while(r != w && f(*r))
                ++r;
            while(r != w && f(*(w - 1)))
                --w;
        }

//special
        void shrink(size_t a)
        {
            if(distance(w, b + N) < a)
                shrink();
        }
        void shrink()
        {
            if(b != r)
            {
                //todo debug test
                move(r, w, b);
                w = b + distance(r, w);
                r = b;
            }
        }

    protected:
        byte b[N];
        byte* r;
        byte* w;
};

template<size_t N>
size_t string_size_internal(const arraybuf<N>& s)
{
    return s.size();
}
template<size_t N>
const char* string_cstring_internal(arraybuf<N>& s)
{
    s.push_back(0);
    s.pop_back();
    return s.data();
}//no const version availble
template<size_t N>
const char* string_data_internal(const arraybuf<N>& s)
{
    return s.data();
}
template<size_t N>
std::back_insert_iterator<arraybuf<N>> string_inserter_internal(arraybuf<N>& s)
{
    return back_inserter(s);
}


#endif//ARRAYBUF_HPP
