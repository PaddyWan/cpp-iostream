mkdir obj
mkdir obj/internal
mkdir obj/iostream
mkdir bin
gcc -c -O0 -g3 -fno-inline -DDEBUG -std=gnu++14 -fdiagnostics-color=always -Wall -fconcepts -DNUMBER_OF_THREADS=12 -I/home/werkmap/lib/src src/internal/string.cpp -o obj/internal/string-d.opp 
gcc -c -O0 -g3 -fno-inline -DDEBUG -std=gnu++14 -fdiagnostics-color=always -Wall -fconcepts -DNUMBER_OF_THREADS=12 -I/home/werkmap/lib/src src/std.cpp -o obj/std-d.opp 
gcc -c -O0 -g3 -fno-inline -DDEBUG -std=gnu++14 -fdiagnostics-color=always -Wall -fconcepts -DNUMBER_OF_THREADS=12 -I/home/werkmap/lib/src src/iostream/stdutil.cpp -o obj/iostream/stdutil-d.opp 
gcc -c -O0 -g3 -fno-inline -DDEBUG -std=gnu++14 -fdiagnostics-color=always -Wall -fconcepts -DNUMBER_OF_THREADS=12 -I/home/werkmap/lib/src src/main.cpp -o obj/main-d.opp
g++ -lstdc++ -lm -lbz2 -lz -llzma -lcrypto obj/internal/string-d.opp obj/iostream/stdutil-d.opp obj/main-d.opp obj/std-d.opp /home/werkmap/lib/bin/lib-d.app -o bin/stream-d.epp
