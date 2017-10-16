#include "string.hpp"
#include "iostream/iostream.hpp"

template<OBuf obuf, IBuf ibuf>
void compress(const stringbuf& str)
{
    stringbuf buffer;
    {
	obuf ob(buffer);
	utilstream::copy(stringbuf(str), ob);
    }
    sos << "total size:" << buffer.size() << '\n';
    stringbuf exit;
    {
	ibuf ib(buffer);
	utilstream::copy(ib, exit);
    }
    sos << string_compare(exit,str) << '\n';
}

int main()
{
    stringbuf str;
    for(size_t i = 0; i <  10000; ++i)
	make_ostream(str) << i << ' ';
    sos << "total size:" << str.size() << '\n';
    sos << "z\n";
    compress<ozbuf<stringbuf&>,izbuf<stringbuf&>>(str);
    sos << "bz\n";
    compress<obzbuf<stringbuf&>,ibzbuf<stringbuf&>>(str);
    sos << "lzma\n";
    compress<olzmabuf<stringbuf&>,ilzmabuf<stringbuf&>>(str);
    return 0;
}
