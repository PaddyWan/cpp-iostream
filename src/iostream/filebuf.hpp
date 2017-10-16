#ifndef FILEBUF_HPP
#define FILEBUF_HPP

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "fdbuf.hpp"
#include "bufferbuf.hpp"

class native_fileutil
{
    public:
        native_fileutil() = delete;
        static size_t optimal(int fd)
        {
            struct stat buf;
            return ((::fstat(fd, &buf) == 0) ? buf.st_blksize : 4 * 1024);
        }
        template<String stringT>
        static int read_open(const stringT& str)
        {
            return ::open(string_cstring(str), O_RDONLY);
        }
        template<String stringT>
        static int write_open(const stringT& str, bool create, bool append, bool truncate)
        {
            return ::open(string_cstring(str), O_WRONLY | (create ? O_CREAT : 0) | (append ? O_APPEND : 0) | (truncate ? O_TRUNC : 0), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        }
        template<String stringT>
        static int readwrite_open(const stringT& str, bool create, bool append, bool truncate)
        {
            return ::open(string_cstring(str), O_RDWR | (create ? O_CREAT : 0) | (append ? O_APPEND : 0) | (truncate ? O_TRUNC : 0), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        }
};

class native_ifilebuf: public fdbuf
{
    public:
        template<String stringT>
        native_ifilebuf(const stringT& str): native_ifilebuf(native_fileutil::read_open(str))
        {
        }
        native_ifilebuf(int fd): fdbuf(fd)
        {
        }
};
class native_ofilebuf: public fdbuf
{
    public:
        template<String stringT>
        native_ofilebuf(const stringT& str, bool create = true, bool append = false, bool truncate = true): native_ofilebuf(native_fileutil::write_open(str, create, append, truncate))
        {
        }
        native_ofilebuf(int fd): fdbuf(fd)
        {
        }
};
class native_filebuf: public fdbuf
{
    public:
        template<String stringT>
        native_filebuf(const stringT& str, bool create = true, bool append = false, bool truncate = true): native_filebuf(native_fileutil::readwrite_open(str, create, append, truncate))
        {
        }
        native_filebuf(int fd): fdbuf(fd)
        {
        }
};

class ifilebuf: public ibufferbuf<native_ifilebuf>
{
    public:
        template<String stringT>
        ifilebuf(const stringT& str): ifilebuf(native_fileutil::read_open(str))
        {}
    protected:
        ifilebuf(int fd): ibufferbuf<native_ifilebuf>(native_ifilebuf(fd), native_fileutil::optimal(fd))
        {
        }
};
class ofilebuf: public obufferbuf<native_ofilebuf>
{
    public:
        template<String stringT>
        ofilebuf(const stringT& str, bool create = true, bool append = false, bool truncate = true): ofilebuf(native_fileutil::write_open(str, create, append, truncate))
        {
        }
    protected:
        ofilebuf(int fd): obufferbuf<native_ofilebuf>(native_ofilebuf(fd), native_fileutil::optimal(fd))
        {
        }
};

#endif//FILEBUF_HPP
