#ifndef FDBUF_HPP
#define FDBUF_HPP

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/eventfd.h>
#include <sys/wait.h>

#include "../cut.hpp"
#include "../std.hpp"

using std::byte;

class fdbuf
{
    public:

        fdbuf(int fd_): fd(fd_)
        {
            if(fd != -1)
                fcntl(fd, F_SETFD, fcntl(fd, F_GETFD) | FD_CLOEXEC);//close on exec call
        }
        fdbuf(fdbuf&& oth): fd(oth.fd)
        {
            oth.fd = -1;
        }
        ~fdbuf()
        {
            if(fd != -1)
                close(fd);
        }
        int get_fd() const
        {
            return fd;
        }
        int geti()
        {
            return (fcntl(fd, F_GETFD) == -1 && errno == EBADF) ? -1 : 0;
        }
        size_t geta()
        {
            int s;
            ioctl(fd, FIONREAD, &s);
            return s;
        }
        size_t getg(byte* c, size_t s)//get
        {
            return ::read(fd, c, s);
        }
        size_t putp(const byte* c, size_t s)
        {
            return ::write(fd, c, s);
        }
    protected:
        int fd;
};

class eventfdobj
{
    public:
        eventfdobj(size_t initvalue = 0, bool readperone = false): fd(::eventfd(initvalue, readperone ? EFD_SEMAPHORE : 0))
        {}
        eventfdobj(eventfdobj&& oth): fd(oth.fd)
        {
            oth.fd = -1;
        }
        ~eventfdobj()
        {
            if(fd != -1)
                close(fd);
        }
        size_t consume()
        {
            size_t r;
            ::read(fd, &r, 8);
            return r;
        }
        void produce(size_t a = 1)
        {
            ::write(fd, &a, 8);
        }
        int get_fd() const
        {
            return fd;
        }
    protected:
        int fd;
};

#endif//FDBUF_HPP
