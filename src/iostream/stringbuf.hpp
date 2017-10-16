#ifndef STRINGBUF_HPP
#define STRINGBUF_HPP

#include <algorithm>

#include "../string.hpp"

using std::copy;
using std::move;
using std::allocator;
using std::max;
using std::min;
using std::string;
using std::swap;

template<class Allocator = allocator<byte>>
class basic_stringbuf
{
    public:
        using allocator_type = Allocator;
        basic_stringbuf(const size_t c = 16, Allocator && alloc = Allocator()): m(alloc, alloc.allocate(max<size_t>(c, 1))), r(m.m), w(m.m), e(m.m + max<size_t>(c, 1))
        {
        }
        template<String stringT>
        basic_stringbuf(const stringT& str, Allocator&& alloc = Allocator()): m(alloc, alloc.allocate(max<size_t>(string_size(str), 1))), r(m.m), w(m.m + string_size(str)), e(m.m + max<size_t>(string_size(str), 1))
        {
            copy(reinterpret_cast<const byte*>(string_data(str)), reinterpret_cast<const byte*>(string_data(str) + string_size(str)), m.m);
        }
        basic_stringbuf(const basic_stringbuf& str): m(str.m, str.get_allocator().allocate(max<size_t>(str.size(), 1))), r(m.m), w(m.m + str.size()), e(m.m + max<size_t>(str.size(), 1))
        {
            copy(str.begin(), str.end(), m.m);
        }
        basic_stringbuf(basic_stringbuf&& str): m(str.m, str.m.m), r(str.r), w(str.w), e(str.e)
        {
            str.m.m = nullptr;
        }
        ~basic_stringbuf()
        {
            if(m.m)
                m.deallocate(m.m, distance(m.m, e));
        }
        allocator_type get_allocator() const
        {
            return m;
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
            ensure(gety() + 1);
            ++w;
        }
        void putb(size_t n)
        {
            ensure(gety() + n);
            w += n;
        }
        void putd()
        {
            ensure(gety() + 1);
            *w = std::to_byte(0);
            ++w;
        }
        void putd(size_t n)
        {
            ensure(gety() + n);
            fill(w, w + n, 0);
            w += n;
        }
        int puti()
        {
            return 0;
        }
        int putp(byte c)
        {
            ensure(gety() + 1);
            *w = c;
            ++w;
            return std::to_integer<int>(c);
        }
        size_t putp(const byte* c, size_t s)
        {
            ensure(gety() + s);
            copy(c, c + s, w);
            w += s;
            return s;
        }
        size_t puto(const byte* c, size_t s)
        {
            if(w == e)
                ensure(gety() + s);
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
            return distance(m.m, e);
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
            return m.m;
        }
        byte* real_end()
        {
            return e;
        }
        const byte* real_begin() const
        {
            return m.m;
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
            if(capacity() < str.size())
            {
                m.deallocate(m.m, capacity());
                m.m = m.allocate(str.size());
                e = m.m + str.size();
            }
            r = m.m;
            w = m.m + str.size();
            copy(str.data(), str.data() + str.size(), m.m);
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
            r = w = m.m;
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
            while(r != w && f(*r))
                ++r;
            while(r != w && f(*(w - 1)))
                --w;
        }

        //special
        void ensure(const size_t c)
        {
            //todo debug test this
            if(static_cast<size_t>(distance(r, e)) < c)
            {
                //we need to do something
                if(static_cast<size_t>(distance(m.m, e)) < c)
                {
                    //we need to resize
                    const size_t oc = distance(m.m, e);
                    const size_t nc = std::max(2 * oc, c);
                    byte* const nb = m.allocate(nc);
                    copy(r, w, nb);
                    w = nb + distance(r, w);
                    r = nb;
                    e = nb + nc;
                    m.deallocate(m.m, oc);
                    m.m = nb;
                }
                else
                {
                    //we need to shrink (=move to front)
                    move(r, w, m.m);
                    w -= (r - m.m);
                    r = m.m;
                }
            }
        }

    protected:

        EBO<allocator_type, byte*> m;//byte* b;
        byte* r;
        byte* w;
        byte* e;
};

using stringbuf = basic_stringbuf<allocator<byte>>;

template<class Allocator>
size_t string_size_internal(const basic_stringbuf<Allocator>& s)
{
    return s.size();
}
template<class Allocator>
const char* string_cstring_internal(basic_stringbuf<Allocator>& s)
{
    s.push_back(0);
    s.pop_back();
    return s.data();
}//no const version availble
template<class Allocator>
const char* string_data_internal(const basic_stringbuf<Allocator>& s)
{
    return s.data();
}
template<class Allocator>
std::back_insert_iterator<basic_stringbuf<Allocator>> string_inserter_internal(basic_stringbuf<Allocator>& s)
{
    return back_inserter(s);
}


#endif//STRINGBUF_HPP
