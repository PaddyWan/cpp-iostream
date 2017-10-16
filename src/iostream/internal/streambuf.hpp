class internal_streambuf
{
    public:
        internal_streambuf() = delete;

        //basic functions
        //bump
        template<IBuf ibuf>
        static void getb(ibuf& i) requires SingleBumpIBuf<ibuf>
        {
            i.getb();
        }
        //get single
        template<IBuf ibuf>
        static int getg(ibuf& i) requires SingleGetIBuf<ibuf>
        {
            return i.getg();
        }
        template<IBuf ibuf>
        static int getg(ibuf& i) requires !SingleGetIBuf<ibuf>&& SinglePollIBuf<ibuf>&& SingleBumpIBuf<ibuf>
        {
            int ret = i.getp();
            i.getb();
            return ret;
        }
        template<IBuf ibuf>
        static int getg(ibuf& i) requires !SingleBumpIBuf<ibuf>&& !(SinglePollIBuf<ibuf>&& SingleBumpIBuf<ibuf>)&& OptimizedIBuf<ibuf>
        {
            byte ret;
            size_t s = i.geto(&ret, 1);
            return s == 1 ? std::to_integer<int>(ret) : -1;
        }
        template<IBuf ibuf>
        static int getg(ibuf& i) requires !SingleBumpIBuf<ibuf>&& !(SinglePollIBuf<ibuf>&& SingleBumpIBuf<ibuf>)&& !OptimizedIBuf<ibuf>&& MultiGetIBuf<ibuf>
        {
            byte ret;
            size_t s = i.getg(&ret, 1);
            return s == 1 ? std::to_integer<int>(ret) : -1;
        }
        //get multi
        template<IBuf ibuf>
        static size_t getg(ibuf&& b, byte* d, size_t s) requires MultiGetIBuf<ibuf>
        {
            return b.getg(d, s);
        }

        //put single
        template<OBuf obuf>
        static int putp(obuf& i, byte c) requires SinglePutOBuf<obuf>
        {
            return i.putp(c);
        }
        template<OBuf obuf>
        static int putp(obuf& i, byte c) requires !SinglePutOBuf<obuf>&& OptimizedOBuf<obuf>
        {
            return i.puto(&c, 1) == 1 ? std::to_integer<int>(c) : -1;
        }
        template<OBuf obuf>
        static int putp(obuf& i, byte c) requires !SinglePutOBuf<obuf>&& !OptimizedOBuf<obuf>&& MultiPutOBuf<obuf>
        {
            return i.putp(&c, 1) == 1 ? std::to_integer<int>(c) : -1;
        }
        //put multi
        template<OBuf obuf>
        static int putp(obuf&& b, const byte* d, size_t s) requires MultiPutOBuf<obuf>
        {
            return b.putp(d, s);
        }
        template<OBuf obuf>
        static int putp(obuf&& b, const byte* d, size_t s) requires !MultiPutOBuf<obuf>&& OptimizedOBuf<obuf>
        {
            size_t t;
            size_t r = t = b.puto(d, s);
            while(r > 0 && r != s)
            {
                d += r;
                s -= r;
                t += (r = b.puto(d, s));
            }
            return t;
        }
};
