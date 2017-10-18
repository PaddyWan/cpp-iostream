#!/bin/bash
mkdir -p obj
mkdir -p obj/internal
mkdir -p obj/iostream
mkdir -p bin
gcc -c -O0 -g3 -fno-inline -DDEBUG -std=gnu++14 -fdiagnostics-color=always -Wall -fconcepts src/internal/string.cpp -o obj/internal/string-d.opp 
gcc -c -O0 -g3 -fno-inline -DDEBUG -std=gnu++14 -fdiagnostics-color=always -Wall -fconcepts src/std.cpp -o obj/std-d.opp 
gcc -c -O0 -g3 -fno-inline -DDEBUG -std=gnu++14 -fdiagnostics-color=always -Wall -fconcepts src/iostream/stdutil.cpp -o obj/iostream/stdutil-d.opp 
gcc -c -O0 -g3 -fno-inline -DDEBUG -std=gnu++14 -fdiagnostics-color=always -Wall -fconcepts src/compression.cpp -o obj/compression-d.opp
gcc -c -O0 -g3 -fno-inline -DDEBUG -std=gnu++14 -fdiagnostics-color=always -Wall -fconcepts src/encryption.cpp -o obj/encryption-d.opp
gcc -c -O0 -g3 -fno-inline -DDEBUG -std=gnu++14 -fdiagnostics-color=always -Wall -fconcepts src/file.cpp -o obj/file-d.opp
gcc -c -O0 -g3 -fno-inline -DDEBUG -std=gnu++14 -fdiagnostics-color=always -Wall -fconcepts src/socket.cpp -o obj/socket-d.opp
g++ -lstdc++ -lm -lbz2 -lz -llzma -lcrypto obj/internal/string-d.opp obj/iostream/stdutil-d.opp obj/compression-d.opp obj/std-d.opp -o bin/compression-d.epp
g++ -lstdc++ -lm -lbz2 -lz -llzma -lcrypto obj/internal/string-d.opp obj/iostream/stdutil-d.opp obj/encryption-d.opp obj/std-d.opp -o bin/encryption-d.epp
g++ -lstdc++ -lm -lbz2 -lz -llzma -lcrypto obj/internal/string-d.opp obj/iostream/stdutil-d.opp obj/file-d.opp obj/std-d.opp -o bin/file-d.epp
g++ -lstdc++ -lm -lbz2 -lz -llzma -lcrypto obj/internal/string-d.opp obj/iostream/stdutil-d.opp obj/socket-d.opp obj/std-d.opp -o bin/socket-d.epp
