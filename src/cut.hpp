#ifndef CUT_HPP
#define CUT_HPP

#include <algorithm>
#include <iterator>

template<class Iter>
struct cut
{
    typedef typename std::iterator_traits<Iter>::value_type value_type;
    typedef std::size_t size_type;
    Iter b;
    Iter e;
    constexpr cut(): b(nullptr), e(nullptr)
    {
    }
    constexpr cut(Iter b_, Iter e_): b(b_), e(e_)
    {
    }
    template<class Cont>
    constexpr cut(const Cont& c): cut(std::begin(c), std::end(c))
    {
    }
    constexpr cut& operator=(cut& other)
    {
        b = other.b;
        e = other.e;
        return *this;
    }
    template<class Cont>
    bool operator==(const Cont& c) const
    {
        return std::equal(b, e, std::begin(c), std::end(c));
    }
    template<class Cont>
    bool operator!=(const Cont& c) const
    {
        return !std::equal(b, e, std::begin(c), std::end(c));
    }
    constexpr Iter begin() const
    {
        return b;
    }
    constexpr Iter end() const
    {
        return e;
    }
    constexpr const value_type& front() const
    {
        return *b;
    }
    constexpr const value_type& back() const
    {
        return *std::next(b, std::distance(b, e) - 1);
    }
    constexpr Iter data() const
    {
        return b;
    }
    constexpr size_type size() const
    {
        return std::distance(b, e);
    }
    constexpr bool empty() const
    {
        return b == e;
    }
    template<class F> //(int(*)(int))&std::isspace
    cut trim(F f) const
    {
        Iter bi = b;
        while(bi != e && f(*bi))
            ++bi;
        Iter ei = e;
        if(bi != ei)
            while(bi != (ei - 1) && f(*(ei - 1)))
                --ei;
        return cut(bi, ei);
    }
};

template<class Iter>
constexpr cut<Iter> make_cut(Iter b, Iter e)
{
    return cut<Iter>(b, e);
}
template<class Cont>
constexpr auto make_cut(const Cont& c)
{
    return make_cut(begin(c), end(c));
}
template<class CharT>
constexpr cut<const CharT*> make_cstring_cut(const CharT* str)
{
    return cut<const CharT*>(str, str + std::char_traits<CharT>::length(str));
}

#endif//CUT_HPP
