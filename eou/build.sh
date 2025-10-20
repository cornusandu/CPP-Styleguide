x86_64-w64-mingw32-g++ -o eou_windows.o -c pointermanagement.cpp -static -Ofast -fno-common -Wl,--dynamicbase -Wl,--high-entropy-va -Wl,--nxcompat -Ofast -ffast-math -funsafe-math-optimizations -fno-trapping-math -finline-functions -fno-math-errno -fivopts -faggressive-loop-optimizations -flto -fipa-pta -fdevirtualize-at-ltrans -march=native -mtune=native -funroll-loops -fomit-frame-pointer -Wl,--enable-icf -Wl,--gc-sections -Wl,-O2
g++ -o eou_linux.o -c pointermanagement.cpp -static -Ofast -fPIC -Ofast -ffast-math -funsafe-math-optimizations -fno-trapping-math -finline-functions -fno-math-errno -fivopts -faggressive-loop-optimizations -flto -fipa-pta -fdevirtualize-at-ltrans -march=native -mtune=native -funroll-loops -fomit-frame-pointer -fprefetch-loop-arrays -Wl,--icf=all -Wl,--gc-sections -Wl,-O2

ar rcs eou_windows.lib eou_windows.o
ar rcs eou_linux.a eou_linux.o

cp eou_linux.a ../dist/eou_linux.a
cp eou_windows.lib ../dist/eou_windows.lib

rm -f *.o *.lib *.a