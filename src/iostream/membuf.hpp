#ifndef MEMBUF_HPP
#define	MEMBUF_HPP

#include <algorithm>
#include <iterator>

#include "../cut.hpp"
#include "../string.hpp"

using std::min;
using std::copy;
using std::move;

class membuf
{
    public:
        //empty buffers
        explicit membuf(byte* data, size_t capacity): membuf(data, data, data, data + capacity)
        {}
        explicit membuf(byte* begin, byte* end): membuf(begin, begin, begin, end)
        {}
        //buffer filled from current read to current write
        explicit membuf(byte* data, size_t size, size_t capacity): membuf(data, data, data + size, data + capacity)
        {
        }
        explicit membuf(byte* begin, byte* currentread, byte* currentwrite, byte* end): b(begin), r(currentread), w(currentwrite), e(end)
        {
        }

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
            *w = std::to_byte(0);
            ++w;
        }
        void putd(size_t n)
        {
            fill(w, w + n, std::to_byte(0));
            w += n;
        }
        int puti()
        {
            shrink(1);
            return w == e ? 1 : 0;
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
            const size_t m = min<size_t>(distance(w, e), s);
            copy(c, c + m, w);
            w += m;
            return m;
        }
        size_t puto(const byte* c, size_t s)
        {
            shrink(1);
            const size_t m = min<size_t>(distance(w, e), s);
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
            return distance(w, e);
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
            return distance(b, e);
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
            return e;
        }
        const byte* real_begin() const
        {
            return b;
        }
        const byte* real_end() const
        {
            return e;
        }
        string str() const
        {
            return string(reinterpret_cast<const char*>(r), reinterpret_cast<const char*>(w));
        }
        void str(const string& str)
        {
            r = b;
            const size_t m = min<size_t>(distance(b, e), str.size());
            w = b + m;
            copy(reinterpret_cast<const byte*>(str.data()), reinterpret_cast<const byte*>(str.data() + m), b);
        }

//std::string
        using value_type = char;
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
            putp(std::to_byte(c));
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
            while(r != w && f(std::to_integer<int>(*r)))
                ++r;
            while(r != w && f(std::to_integer<int>(*(w - 1))))
                --w;
        }

//special

        cut<byte*> getline(const char delim = '\n')
        {
            byte* const ro = r;
            byte* const rn = std::find(r, w, std::to_byte(delim));
            r = (rn == w ? rn : rn + 1);
            return make_cut(ro, rn);
        }
        void shrink(size_t a)
        {
            if(static_cast<size_t>(distance(w, e)) < a)
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
        byte* const b;
        byte* r;
        byte* w;
        byte* const e;
};

size_t string_size_internal(const membuf& s)
{
    return s.size();
}
const char* string_cstring_internal(membuf& s)
{
    s.push_back(0);
    s.pop_back();
    return s.data();
}//no const version availble
const char* string_data_internal(const membuf& s)
{
    return s.data();
}
std::back_insert_iterator<membuf> string_inserter_internal(membuf& s)
{
    return std::back_inserter(s);
}


#endif//MEMBUF_HPP

