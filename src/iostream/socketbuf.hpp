#ifndef SOCKETBUF_HPP
#define SOCKETBUF_HPP

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "fdbuf.hpp"

class socketbuf: public fdbuf
{
    public:

        socketbuf(int fd): fdbuf(fd)
        {
        }


        void time(size_t sec, size_t usec)
        {
            struct timeval tv;
            tv.tv_usec = usec;
            tv.tv_sec = sec;
            setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (struct timeval*)&tv, sizeof(struct timeval));
        }
        void block(bool b)
        {
            const int flags = fcntl(fd, F_GETFL, 0);
            if(b)
                fcntl(fd, F_SETFL, flags & (~O_NONBLOCK));
            else
                fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }

        template<String stringT>
        static uint get_addr(const stringT& ip)
        {
            struct addrinfo hints;
            hints.ai_family = AF_INET;
            hints.ai_socktype = 0;
            hints.ai_flags = AI_PASSIVE;
            hints.ai_protocol = 0;
            hints.ai_canonname = nullptr;
            hints.ai_addr = nullptr;
            hints.ai_next = nullptr;
            addrinfo* res;
            getaddrinfo(string_cstring(ip), nullptr, &hints, &res);
            uint ret = ((struct sockaddr_in*)(res->ai_addr))->sin_addr.s_addr;
            freeaddrinfo(res);
            return ret;
        }
};

class tcpbuf: public socketbuf
{
    public:
        template<String stringT>
        tcpbuf(const stringT& ip, const uint poort): socketbuf(getsocket(ip, poort))
        {
        }
        tcpbuf(int fd): socketbuf(fd)
        {
        }
        /*size_t getg(byte* c, size_t s)//get
          {
          return ::recv(fd, c, s, 0);
          }
          size_t putp(const byte* c, size_t s)
          {
          return ::send(fd, c, s, 0);
          }*/
        //recv/send
    protected:
        template<String stringT>
        static int getsocket(const stringT& ip, const uint poort)
        {
            const int fd = socket(AF_INET, SOCK_STREAM, 0);
            if(fd <= 0)
                return -1;
            int enable = 1;
            if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
                return -1;
            sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = socketbuf::get_addr(ip);
            addr.sin_port = htons(poort);
            if(::connect(fd, (struct sockaddr*) & (addr), sizeof(sockaddr_in)) != 0)
                return -1;
            return fd;
        }
};

/*class udpbuf: public socketbuf
{
    public:
        udpbuf(const uint poort): socketbuf(socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))
        {
            reset.sin_family = AF_INET;
            reset.sin_addr.s_addr = INADDR_ANY;
            reset.sin_port = htons(poort);
            send = reset;
            recv = reset;
        }

    //todo recvfrom sendto
        bool is_okrecv() const
        {
            if(recv.sin_addr.s_addr != INADDR_ANY)
            {
                uint slen = sizeof(sockaddr_in);
		std::byte t;
                sockaddr_in temp(reset);
                recvfrom(fd, &t, 1, MSG_PEEK, (struct sockaddr*)&temp, &slen);
                if(recv.sin_addr.s_addr != temp.sin_addr.s_addr)
                    return false;
            }
            return true;
        }

        uint get_sendto() const
        {
            return send.sin_addr.s_addr;
        }

        void set_sendto(uint addr)//1728161984 is 192.168.1.103
        {
            send.sin_addr.s_addr = addr;
        }

        uint get_recvfrom() const
        {
            return recv.sin_addr.s_addr;
        }

        void set_recvfrom(uint addr = INADDR_ANY)
        {
            recv.sin_addr.s_addr = addr;
        }

    protected:
        sockaddr_in send;
        sockaddr_in recv;
        sockaddr_in reset;
	};*/

class tcpserver
{
    public:
        tcpserver(const uint poort, const uint listen = 5): fd(getsocket(poort, listen))
        {}
        tcpbuf accept()
        {
            sockaddr_in cli_addr;
            socklen_t clilen = sizeof(cli_addr);
            int newfd = ::accept(fd, (struct sockaddr*) &cli_addr, &clilen);
            return tcpbuf(newfd);
        }
    protected:
        int fd;
        static int getsocket(const uint poort, const uint listenn)
        {
            const int fd = socket(AF_INET, SOCK_STREAM, 0);
            if(fd <= 0)
                return -1;
            int enable = 1;
            if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
                return -1;
            sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = INADDR_ANY;
            addr.sin_port = htons(poort);
            if(::bind(fd, (struct sockaddr*) & (addr), sizeof(sockaddr_in)) != 0)
                return -1;
            ::listen(fd, listenn);
            return fd;
        }
};

#endif//SOCKETBUF_HPP
