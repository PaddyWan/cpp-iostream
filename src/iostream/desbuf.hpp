#ifndef DESBUF_HPP
#define DESBUF_HPP

#include <cstdlib>
#include <openssl/des.h>

class desutil
{
    public:
        desutil() = delete;
        static void init_random_key()
        {
            srand(time(nullptr));
        }
        static ulong get_random_key()
        {
            ulong key;
            DES_random_key(reinterpret_cast<DES_cblock*>(&key));
            while(DES_is_weak_key(reinterpret_cast<DES_cblock*>(&key)))
                DES_random_key(reinterpret_cast<DES_cblock*>(&key));
            return key;
        }
};


template<IBuf ibuf>
class idesbuf
{
    public:
        template<class T>
        idesbuf(T&& i_, ulong key): i(std::forward<T>(i_)), s(8)
        {
            DES_set_key(reinterpret_cast<DES_cblock*>(&key), &k);
        }

        int getg()//get
        {
            if(s == 8)
                decrypt();
            if(s < 8)
            {
                int ret = std::to_integer<int>(d[s]);
                ++s;
                return ret;
            }
            else
                return -1;
        }
    protected:
        void decrypt()
        {
            size_t r = streambuf::getg(i, d, 8);
            if(r)
            {
                DES_ecb_encrypt(reinterpret_cast<DES_cblock*>(d), reinterpret_cast<DES_cblock*>(d), &k, DES_DECRYPT);
                s = 0;
            }
        }
        ibuf i;
        DES_key_schedule k;
        byte d[8];
        size_t s;
};


template<OBuf obuf>
class odesbuf
{
    public:
        template<class T>
        odesbuf(T&& o_, ulong key): o(std::forward<T>(o_)), s(0)
        {
            DES_set_key(reinterpret_cast<DES_cblock*>(&key), &k);
        }
        ~odesbuf()
        {
            if(s != 0)
                encrypt(d);
        }
        int putp(byte c)
        {
            if(s == 8)
            {
                encrypt(d);
                s = 0;
            }
            d[s] = c;
            s++;
            return std::to_integer<int>(c);
        }
    protected:
        void encrypt(byte* c)
        {
            DES_ecb_encrypt(reinterpret_cast<DES_cblock*>(c), reinterpret_cast<DES_cblock*>(d), &k, DES_ENCRYPT);
            streambuf::putp(o, d, 8);
        }
        obuf o;
        DES_key_schedule k;
        byte d[8];
        size_t s;
};




#endif//DESBUF_HPP
