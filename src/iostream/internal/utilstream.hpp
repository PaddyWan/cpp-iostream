class internal_utilstream
{
    public:
        internal_utilstream() = delete;

        template<IBuf ibuf, OBuf obuf, class Allocator>
        static void btob(ibuf& i, obuf& o, Allocator& aloc) requires ChunkGetIBuf<ibuf>&& ChunkPutOBuf<obuf>
        {
            if(!(i.gety() == 0 && o.puty() == o.putc()))
                btob_(i, o, aloc);
            else if(i.getc() != o.putc())//todo deze else if kan beter
                btob_(i, o, aloc);
            else
            {
                byte* const x = o.putx(); //this one is safer, putx always return the same
                size_t r = i.getz(x);
                while(r == i.getc())
                {
                    o.putz(x);
                    r = i.getz(x);
                }
                o.putb(r);
            }
        }
        template<IBuf ibuf, OBuf obuf, class Allocator>
        static void btob(ibuf& i, obuf& o, Allocator& aloc) requires !(ChunkGetIBuf<ibuf>&& ChunkPutOBuf<obuf>)
        {
            btob_(i, o, aloc);
        }
        template<IBuf ibuf, OBuf obuf, class Allocator>
        static void btob_(ibuf& i, obuf& o, Allocator& aloc) requires OptimizedIBuf<ibuf>
        {
            constexpr static size_t BUFFER_SIZE = 4 * 1024;
            byte* buffer = aloc.allocate(BUFFER_SIZE);
            size_t r = i.geto(buffer, BUFFER_SIZE);
            while(r != 0)
            {
                streambuf::putp(o, buffer, r);
                r = i.geto(buffer, BUFFER_SIZE);
            }
            aloc.deallocate(buffer, BUFFER_SIZE);
        }
        template<IBuf ibuf, OBuf obuf, class Allocator>
        static void btob_(ibuf& i, obuf& o, Allocator&) requires !OptimizedIBuf<ibuf>
        {
            int r = streambuf::getg(i);
            while(r != -1)
            {
                streambuf::putp(o, std::to_byte(r));
                r = streambuf::getg(i);
            }
        }
        template<IBuf ibuf, class tint>
        static size_t btoi(ibuf& i, tint& r)
        {
            return sizeof(tint) - streambuf::getg(i, reinterpret_cast<byte*>(&r), sizeof(tint));
        }
        template<IBuf ibuf, class tint>
        static void dtoi(ibuf& i, tint& r, int& lastchar)
        {
            r = atoi<tint>(ibuf_iterator<ibuf>(i), lastchar);
        }
        template<IBuf ibuf, class OutputIt>
        static void dtoa(ibuf& i, OutputIt it, char delim, int& lastchar)
        {
            lastchar = streambuf::getg(i);
            while(lastchar != -1 && lastchar != std::to_integer<int>(static_cast<std::byte>(delim)))
            {
                it = static_cast<char>(lastchar);
                lastchar = streambuf::getg(i);
            }
        }
        template<IBuf ibuf, class OutputIt>
        static void dtoa(ibuf& i, OutputIt it, char delim1, char delim2, int& lastchar)
        {
            lastchar = streambuf::getg(i);
            while(lastchar != -1 && lastchar != std::to_integer<int>(static_cast<std::byte>(delim1)) && lastchar != std::to_integer<int>(static_cast<std::byte>(delim2)))
            {
                it = static_cast<char>(lastchar);
                lastchar = streambuf::getg(i);
            }
        }
        template<IBuf ibuf, class OutputIt>
        static void dtoa(ibuf& i, OutputIt it, const bool* const delim, int& lastchar)
        {
            lastchar = streambuf::getg(i);
            while(lastchar != -1 && (!delim[lastchar]))
            {
                it = static_cast<char>(lastchar);
                lastchar = streambuf::getg(i);
            }
        }
        template<IBuf ibuf, class OutputIt, class Func>
        static void dtoa(ibuf& i, OutputIt it, Func f, int& lastchar)
        {
            lastchar = streambuf::getg(i);
            while(lastchar != -1 && !f(lastchar))
            {
                it = static_cast<char>(lastchar);
                lastchar = streambuf::getg(i);
            }
        }
        template<OBuf obuf, class tint>
        static size_t itob(obuf& b, tint r)
        {
            return sizeof(tint) - streambuf::putp(b, reinterpret_cast<std::byte*>(&r), sizeof(tint));
        }
        template<OBuf obuf, class tint>
        static size_t itod(obuf& b, tint r)
        {
            return itod(b, r, identity<tint>());
        }
        static constexpr size_t stringintsize = 22;//ulong 20 + sign + nullterminate
        template<OBuf obuf, class tint>
        static size_t itod(obuf& b, tint r, identity<tint>)
        {
            //also int
            char BUF[stringintsize];
            const size_t ret = itoa(r, BUF);
            return ret - streambuf::putp(b, reinterpret_cast<std::byte*>(BUF), ret);
        }
        template<OBuf obuf>
        static size_t itod(obuf& b, uint r, identity<uint>)
        {
            char BUF[stringintsize];
            const size_t ret = utoa(r, BUF);
            return ret - streambuf::putp(b, reinterpret_cast<std::byte*>(BUF), ret);
        }
        template<OBuf obuf>
        static size_t itod(obuf& b, long r, identity<long>)
        {
            char BUF[stringintsize];
            const size_t ret = ltoa(r, BUF);
            return ret - streambuf::putp(b, reinterpret_cast<std::byte*>(BUF), ret);
        }
        template<OBuf obuf>
        static size_t itod(obuf& b, ulong r, identity<ulong>)
        {
            char BUF[stringintsize];
            const size_t ret = ultoa(r, BUF);
            return ret - streambuf::putp(b, reinterpret_cast<std::byte*>(BUF), ret);
        }
        template<OBuf obuf, String stringT>
        static size_t atob(obuf& b, const stringT& str)
        {
            return string_size(str) - streambuf::putp(b, reinterpret_cast<const std::byte*>(string_data(str)), string_size(str));
        }

};
