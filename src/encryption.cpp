#include "string.hpp"
#include "iostream/iostream.hpp"


int main()
{
    desutil::init_random_key();
    ulong key = desutil::get_random_key();
    stringbuf str;
    for(size_t i = 0; i <  1000; ++i)
        make_ostream(str) << i << ' ';
    sos << "total size:" << str.size() << '\n';
    stringbuf buffer;
    {
        odesbuf<stringbuf&> ob(buffer, key);
        utilstream::copy(stringbuf(str), ob);
    }
    sos << "total size:" << buffer.size() << '\n';
    stringbuf exit;
    {
        idesbuf<stringbuf&> ib(buffer, key);
        utilstream::copy(ib, exit);
        while(exit.size() > str.size())
            exit.pop_back();
    }
    sos << string_compare(exit, str) << '\n';
    return 0;
}
