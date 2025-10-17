x86_64-w64-mingw32-g++ -o eou_windows.o -c pointermanagement.cpp -static -O2 -fno-common -Wl,--dynamicbase -Wl,--high-entropy-va -Wl,--nxcompat
g++ -o eou_linux.o -c pointermanagement.cpp -static -O3 -fPIC

ar rcs eou_windows.lib eou_windows.o
ar rcs eou_linux.a eou_linux.o

cp eou_linux.a ../dist/eou_linux.a
cp eou_windows.lib ../dist/eou_windows.lib

rm -f *.o *.lib *.a