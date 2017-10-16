#ifndef ALOCSTREAM_HPP
#define ALOCSTREAM_HPP

template <IBuf ibuf>
class alocistream
{
        ibuf& i;
        byte* b;
        size_t c;
    public:
        alocistream(ibuf& i_, size_t c_): i(i_), b(new byte[c_]), c(c_) {}
        alocistream(ibuf&& i_, size_t c_): i(i_), b(new byte[c_]), c(c_) {}
        ~alocistream()
        {
            if(b)
                delete[] b;
        }
        std::pair<byte*, size_t>  get(size_t s)
        {
            return make_pair(b, streambuf::getg(i, b, s));
        }
        std::pair<byte*, size_t> get()
        {
            return  get(c);
        }
        static membuf buf(std::pair<byte*, size_t> p)
        {
            return membuf(p.first, p.first, p.first + p.second, p.first + p.second);
        }
};
template <OBuf obuf>
class alocostream
{
        obuf& o;
        byte* b;
        size_t c;
    public:
        alocostream(obuf& o_, size_t c_): o(o_), b(new byte[c_]), c(c_) {}
        alocostream(obuf&& o_, size_t c_): o(o_), b(new byte[c_]), c(c_) {}
        ~alocostream()
        {
            if(b)
                delete[] b;
        }
        std::pair<byte*, size_t>  put(size_t s)
        {
            return make_pair(b, s);
        }
        std::pair<byte*, size_t> put()
        {
            return put(c);
        }
        void bump(size_t s)
        {
            streambuf::putp(o, b, s);
        }
        void bump(std::pair<byte*, size_t> p)
        {
            streambuf::putp(o, p.first, p.second);
        }
        static membuf buf(std::pair<byte*, size_t> p)
        {
            return membuf(p.first, p.first, p.first, p.first + p.second);
        }
        static size_t size(const membuf& m)
        {
            return distance(m.real_begin(), m.end());
        }
        static size_t pos(const membuf& m)
        {
            return make_pair(m.begin(), distance(m.begin(), m.end()));
        }
};


#endif//ALOCSTREAM_HPP
