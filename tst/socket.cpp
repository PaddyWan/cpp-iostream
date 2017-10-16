#include "string.hpp"
#include "iostream/iostream.hpp"

int main()
{
//    {
//	bufferbuf<tcpbuf> s(tcpbuf("example.com", 80), 64 * 1024);
//	make_ostream(s) << "GET /index.html HTTP/1.1\nHost: www.example.com\n\n";
//	s.putf();
//	s.getf();
//	sos << membuf(s.getx(), s.gety(), s.gety()) << '\n';
//    }
    {
	tcpbuf s("example.com", 80);
	make_ostream(s) << "GET /index.html HTTP/1.1\nHost: www.example.com\n\n";
	alocistream<tcpbuf> ais(s,64*1024);
	sos << alocistream<tcpbuf>::buf(ais.get()) << '\n';
    }
    return 0;
}
