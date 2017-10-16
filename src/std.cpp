#include "std.hpp"

constexpr char digit_pairs[201] =
{
    "00010203040506070809"
    "10111213141516171819"
    "20212223242526272829"
    "30313233343536373839"
    "40414243444546474849"
    "50515253545556575859"
    "60616263646566676869"
    "70717273747576777879"
    "80818283848586878889"
    "90919293949596979899"
};

size_t itoa(const int n, char* const s)
{
    if(n == 0)
    {
        *s = '0';
        return 1;
    }

    const int sign = -(n < 0);
    uint val = (n ^ sign) - sign;

    size_t size;
    if(val >= 10000)
    {
        if(val >= 10000000)
        {
            if(val >= 1000000000)
                size = 10;
            else if(val >= 100000000)
                size = 9;
            else
                size = 8;
        }
        else
        {
            if(val >= 1000000)
                size = 7;
            else if(val >= 100000)
                size = 6;
            else
                size = 5;
        }
    }
    else
    {
        if(val >= 100)
        {
            if(val >= 1000)
                size = 4;
            else
                size = 3;
        }
        else
        {
            if(val >= 10)
                size = 2;
            else
                size = 1;
        }
    }
    size -= sign;
    char* c = s;
    if(sign)
        *c = '-';

    c += size - 1;
    while(val >= 100)
    {
        int pos = val % 100;
        val /= 100;
        *reinterpret_cast<short*>(c - 1) = *reinterpret_cast<const short*>(digit_pairs + 2 * pos);
        c -= 2;
    }
    while(val > 0)
    {
        *c-- = '0' + (val % 10);
        val /= 10;
    }
    return size;
}

size_t utoa(uint val, char* const s)
{
    if(val == 0)
    {
        *s = '0';
        return 1;
    }

    size_t size;
    if(val >= 10000)
    {
        if(val >= 10000000)
        {
            if(val >= 1000000000)
                size = 10;
            else if(val >= 100000000)
                size = 9;
            else
                size = 8;
        }
        else
        {
            if(val >= 1000000)
                size = 7;
            else if(val >= 100000)
                size = 6;
            else
                size = 5;
        }
    }
    else
    {
        if(val >= 100)
        {
            if(val >= 1000)
                size = 4;
            else
                size = 3;
        }
        else
        {
            if(val >= 10)
                size = 2;
            else
                size = 1;
        }
    }

    char* c = s + size - 1;
    while(val >= 100)
    {
        int pos = val % 100;
        val /= 100;
        *reinterpret_cast<short*>(c - 1) = *reinterpret_cast<const short*>(digit_pairs + 2 * pos);
        c -= 2;
    }
    while(val > 0)
    {
        *c-- = '0' + (val % 10);
        val /= 10;
    }
    return size;
}



size_t ltoa(const long n, char* const s)
{
    if(n == 0)
    {
        *s = '0';
        return 1;
    }

    const long sign = -(n < 0);
    ulong val = (n ^ sign) - sign;

    size_t size;
    if(val >= 10000UL)
    {
        if(val >= 1000000000000UL)
        {
            if(val >= 10000000000000000UL)
            {
                if(val >= 1000000000000000000UL)
                {
                    if(val >= 10000000000000000000UL)
                        size = 20;
                    else
                        size = 19;
                }
                else
                {
                    if(val >= 100000000000000000UL)
                        size = 18;
                    else
                        size = 17;
                }
            }
            else
            {
                if(val >= 100000000000000UL)
                {
                    if(val >= 1000000000000000UL)
                        size = 16;
                    else
                        size = 15;
                }
                else
                {
                    if(val >= 10000000000000UL)
                        size = 14;
                    else
                        size = 13;
                }
            }
        }
        else
        {
            if(val >= 100000000UL)
            {
                if(val >= 10000000000UL)
                {
                    if(val >= 100000000000UL)
                        size = 12;
                    else
                        size = 11;
                }
                else
                {
                    if(val >= 1000000000UL)
                        size = 10 ;
                    else
                        size = 9;
                }
            }
            else
            {
                if(val >= 1000000UL)
                {
                    if(val >= 10000000UL)
                        size = 8;
                    else
                        size = 7;
                }
                else
                {
                    if(val >= 100000UL)
                        size = 6;
                    else
                        size = 5;
                }
            }
        }
    }
    else
    {
        if(val >= 100UL)
        {
            if(val >= 1000UL)
                size = 4;
            else
                size = 3;
        }
        else
        {
            if(val >= 10UL)
                size = 2;
            else
                size = 1;
        }
    }

    size -= sign;
    char* c = s;
    if(sign)
        *c = '-';

    c += size - 1;
    while(val >= 100UL)
    {
        int pos = val % 100;
        val /= 100;
        *reinterpret_cast<short*>(c - 1) = *reinterpret_cast<const short*>(digit_pairs + 2 * pos);
        c -= 2;
    }
    while(val > 0)
    {
        *c-- = '0' + (val % 10);
        val /= 10;
    }
    return size;
}



size_t ultoa(ulong val, char* const s)
{
    if(val == 0)
    {
        *s = '0';
        return 1;
    }

    size_t size;
    if(val >= 10000UL)
    {
        if(val >= 1000000000000UL)
        {
            if(val >= 10000000000000000UL)
            {
                if(val >= 1000000000000000000UL)
                {
                    if(val >= 10000000000000000000UL)
                        size = 20;
                    else
                        size = 19;
                }
                else
                {
                    if(val >= 100000000000000000UL)
                        size = 18;
                    else
                        size = 17;
                }
            }
            else
            {
                if(val >= 100000000000000UL)
                {
                    if(val >= 1000000000000000UL)
                        size = 16;
                    else
                        size = 15;
                }
                else
                {
                    if(val >= 10000000000000UL)
                        size = 14;
                    else
                        size = 13;
                }
            }
        }
        else
        {
            if(val >= 100000000UL)
            {
                if(val >= 10000000000UL)
                {
                    if(val >= 100000000000UL)
                        size = 12;
                    else
                        size = 11;
                }
                else
                {
                    if(val >= 1000000000UL)
                        size = 10 ;
                    else
                        size = 9;
                }
            }
            else
            {
                if(val >= 1000000UL)
                {
                    if(val >= 10000000UL)
                        size = 8;
                    else
                        size = 7;
                }
                else
                {
                    if(val >= 100000UL)
                        size = 6;
                    else
                        size = 5;
                }
            }
        }
    }
    else
    {
        if(val >= 100UL)
        {
            if(val >= 1000UL)
                size = 4;
            else
                size = 3;
        }
        else
        {
            if(val >= 10UL)
                size = 2;
            else
                size = 1;
        }
    }

    char* c = s + size - 1;
    while(val >= 100)
    {
        int pos = val % 100;
        val /= 100;
        *reinterpret_cast<short*>(c - 1) = *reinterpret_cast<const short*>(digit_pairs + 2 * pos);
        c -= 2;
    }
    while(val > 0)
    {
        *c-- = '0' + (val % 10);
        val /= 10;
    }
    return size;
}

