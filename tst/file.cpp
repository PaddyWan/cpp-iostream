#include "iostream/iostream.hpp"

int main()
{
    ifilebuf src("random");
    ofilebuf dst("randomnewcpp");
    utilstream::copy(src,dst);
    return 0;
}

#ifdef USE_STD_FSTREAM
#include <fstream>
int main()
{
    std::ifstream  src("random", std::ios::binary);
    std::ofstream  dst("randombasiccpp",   std::ios::binary);

    dst << src.rdbuf();
    return 0;
}
#endif


//std f stream
//real	0m8.023s~0m9.534s
//user	0m0.013s~0m0.021s
//sys	0m0.710s~0m0.765s

//1 by 1
//real	0m9.121s~0m10.983s
//user	0m2.017s~0m2.026s
//sys	0m0.653s~0m0.705s

//n by n with n = 4*1024
//real	0m8.002s~0m9.533s
//user	0m0.037s~0m0.048s
//sys	0m0.711s~0m0.769s

//useof chuncked buffers
//real	0m7.535s~0m8.730s
//user	0m0.005s~0m0.015s
//sys	0m0.690s~0m0.719s

