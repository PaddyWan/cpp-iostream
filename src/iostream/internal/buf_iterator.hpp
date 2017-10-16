template<IBuf ibuf> requires !SinglePollIBuf<ibuf>
class ibuf_iterator<ibuf>
{
    public:
//        typedef typename A::difference_type difference_type;
//        typedef typename A::value_type value_type;
//        typedef typename A::reference reference;
//        typedef typename A::pointer pointer;
//        typedef std::random_access_iterator_tag iterator_category;
        constexpr ibuf_iterator(): i(nullptr), r(-1)
        {
        }
        ibuf_iterator(ibuf& i_): i(&i_)
        {
            r = i->getg();
        }
//    ibuf_iterator& operator=(const ibuf_iterator&);
        bool operator==(const ibuf_iterator& rhs) const
        {
            return r == rhs.r && i == rhs.i;
        }
        bool operator!=(const ibuf_iterator& rhs) const
        {
            return !(operator==(rhs));
        }
        const int& operator*() const
        {
            return r;
        }
        ibuf_iterator& operator++()
        {
            r = i->getg();
            if(r == -1)//end
                i = nullptr;
            return *this;
        }
    private:
        typename std::remove_reference<ibuf>::type* i;//pointer gives us nullptr, for enditerator
        int r;
};

template<IBuf ibuf> requires SinglePollIBuf<ibuf>
class ibuf_iterator<ibuf>
{
    public:
//        typedef typename A::difference_type difference_type;
//        typedef typename A::value_type value_type;
//        typedef typename A::reference reference;
//        typedef typename A::pointer pointer;
//        typedef std::random_access_iterator_tag iterator_category;
        constexpr ibuf_iterator(): i(nullptr)
        {
        }
        ibuf_iterator(ibuf& i_): i(&i_)
        {
        }
//    ibuf_iterator& operator=(const ibuf_iterator&);
        bool operator==(const ibuf_iterator& rhs) const
        {
            return i == rhs.i;
        }
        bool operator!=(const ibuf_iterator& rhs) const
        {
            return !(operator==(rhs));
        }
        const int& operator*() const
        {
            return i->getp();
        }
        ibuf_iterator& operator++()
        {
            streambuf::getb(*i);
            if(i->getp() == -1)//end
                i = nullptr;
            return *this;
        }

    private:
        typename std::remove_reference<ibuf>::type* i;//pointer gives us nullptr, for enditerator
};
