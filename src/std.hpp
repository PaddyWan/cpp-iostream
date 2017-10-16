#ifndef STD_HPP
#define STD_HPP

#include <functional>

using std::less;
using std::minus;
using std::get;

namespace std
{
    //temp fix for std::byte
    enum class byte : unsigned char {} ;
    template <class IntegerType>
    constexpr byte& operator<<=(byte& b, IntegerType shift) noexcept
    {
        return b = std::byte(static_cast<unsigned char>(b) << shift);
    }
    template <class IntegerType>
    constexpr byte operator<<(byte b, IntegerType shift) noexcept
    {
        return std::byte(static_cast<unsigned char>(b) << shift);
    }
    template <class IntegerType>
    constexpr byte& operator>>=(byte& b, IntegerType shift) noexcept
    {
        return b = std::byte(static_cast<unsigned char>(b) >> shift);
    }
    template <class IntegerType>
    constexpr byte operator>>(byte b, IntegerType shift) noexcept
    {
        return std::byte(static_cast<unsigned char>(b) >> shift);
    }
    constexpr byte& operator|=(byte& l, byte r) noexcept
    {
        return l = std::byte(static_cast<unsigned char>(l) | static_cast<unsigned char>(r));
    }
    constexpr byte operator|(byte l, byte r) noexcept
    {
        return std::byte(static_cast<unsigned char>(l) | static_cast<unsigned char>(r));
    }
    constexpr byte& operator&=(byte& l, byte r) noexcept
    {
        return l = std::byte(static_cast<unsigned char>(l) & static_cast<unsigned char>(r));
    }
    constexpr byte operator&(byte l, byte r) noexcept
    {
        return std::byte(static_cast<unsigned char>(l) & static_cast<unsigned char>(r));
    }
    constexpr byte& operator^=(byte& l, byte r) noexcept
    {
        return l = std::byte(static_cast<unsigned char>(l) ^ static_cast<unsigned char>(r));
    }
    constexpr byte operator^(byte l, byte r) noexcept
    {
        return std::byte(static_cast<unsigned char>(l) ^ static_cast<unsigned char>(r));
    }
    constexpr byte operator~(byte b) noexcept
    {
        return std::byte(~static_cast<unsigned char>(b));
    }
    template <class IntegerType>
    constexpr IntegerType to_integer(byte b) noexcept
    {
        return IntegerType(b);
    }
    template <class IntegerType>
    constexpr byte to_byte(IntegerType b) noexcept
    {
        return byte(b);
    }
}

//maxsize is 22?
size_t itoa(const int n, char* const s);
size_t utoa(uint val, char* const s);
size_t ltoa(const long n, char* const s);
size_t ultoa(ulong val, char* const s);

template<class tint, class Iter>
tint atoi(Iter i, int& lastchar)//Iter returns -1 when at end
{
    while(isspace(*i))//-1 is not space so would be false
        ++i;
    tint sign;
    if((*i) == '-')
    {
        sign = -1;
        ++i;
    }
    else
        sign = 1;
    tint val = 0;
    if((*i) == '0')
    {
        ++i;
        if((*i) == 'x' || (*i) == 'X')
        {
            ++i;
            static const uint base16_value[256] =
            {
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0,
                0, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            };
            while(isxdigit(*i))
            {
                val = val << 4 | base16_value[*i];
                ++i;
            }
        }
        else if((*i) == 'b' || (*i) == 'B')
        {
            ++i;
            while((*i) == '0' || (*i) == '1')
            {
                val = val << 1 | ((*i) == '1');
                ++i;
            }
        }
        else
        {
            if((*i) == 'o' || (*i) == 'O')//this doesn't have to be here, starting with 0 is enough to say it's oct
            {
                ++i;
            }
            while((*i) >= '0' && (*i) <= '7')
            {
                val = val << 3 | ((*i) - '0');
                ++i;
            }
        }
    }
    else
    {
        while(isdigit(*i))
        {
            val = val * 10 + ((*i) - '0');
            ++i;
        }
    }
    lastchar = *i;
    return sign * val;
}

template <class T, class Compare = less<T>, class Difference = minus<T>>
struct abs_diff
{
    abs_diff() {}
    abs_diff(Compare comp) {}
    abs_diff(Compare comp, Difference diff) {}
    constexpr T operator()(const T& a, const T& b) const
    {
        return op(a, b);
    }
    static constexpr T op(const T& a, const T& b)
    {
        return (Compare()(a, b) ? Difference()(b, a) : Difference()(a, b));
    }
};

template<class T>
struct sq
{
    constexpr T operator()(const T& t) const
    {
        return op(t);
    }
    static constexpr T op(const T& t)
    {
        return t * t;
    }
};

//return op1(op2(para), ...)
template<class Op1, class Op2>
struct op_op
{
    template<class Para>
    auto operator()(const Para& a) const
    {
        return Op1()(Op2()(a));
    }
    template<class Para>
    auto operator()(const Para& lhs, const Para& rhs) const
    {
        return Op1()(Op2()(lhs), Op2()(rhs));
    }
};

template<class P>
struct first_pair
{
    auto operator()(const P& p) const
    {
        return p.first;
    }
};

template<class P>
struct second_pair
{
    auto operator()(const P& p) const
    {
        return p.second;
    }
};

template<class T, size_t n>
struct index_tuple
{
    auto operator()(const T& p) const
    {
        return get<n>(p);
    }
};

constexpr size_t prev_pow2(size_t x)
{
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >> 16);
    return (x + 1) >> 1;
}

template <class T, class Compare = less<T>, class Difference = minus<T>>
struct sgn
{
    sgn() {}
    sgn(Compare comp) {}
    sgn(Compare comp, Difference diff) {}
    constexpr int operator()(const T& val) const
    {
        return op(val);
    }
    static constexpr int op(const T& val)
    {
        return Difference()(Compare()(T(0), val), Compare()(val, T(0))) ;
    }
};

constexpr size_t gcd(size_t a, size_t b)
{
    while(b != 0)
    {
        auto c = a % b;
        a = b;
        b = c;
    }
    return a;
//    return b == 0 ? a : gcd(b, a % b);
}

template<class T>
struct identity
{
    typedef T type;
};

struct null_iterator
{
    template<class T>
    constexpr null_iterator& operator=(const T&)
    {
        return *this;
    }
    template<class T>
    constexpr null_iterator& operator++()
    {
        return *this;
    }
    template<class T>
    constexpr null_iterator& operator++(int)
    {
        return *this;
    }
    template<class T>
    constexpr null_iterator& operator*()
    {
        return *this;
    }
};

template<class tint>
constexpr void* next(void* ptr, tint offset = 1)
{
    return reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(ptr) + offset);
}

template<class T>
constexpr T& zombie(T&& zombie)
{
    T z = move(zombie);
    return zombie;
}

template <class Base, class Member> //Base = allocator, Member first node for tree
struct EBO : Base //Empty Base Optimization
{
    //std::remove_reference<Base>::type as base
    Member m;
    template<class B, class M>
    EBO(B&& b, M&& mem): Base(std::forward<B>(b)), m(std::forward<M>(mem))
    {
    }
};

template<class ForwardIt, class T, class Compare = std::less<T>>
ForwardIt binary_find(ForwardIt first, ForwardIt last, const T& value, Compare comp = {})
{
    first = std::lower_bound(first, last, value, comp);
    return first != last && !comp(value, *first) ? first : last;
}

#endif//STD_HPP
