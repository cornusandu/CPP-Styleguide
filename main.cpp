// don't mind me

#include "eou/pointermanagement.hpp"

int main() {
    betterReference<int64_t> *p[1024];
    for (size_t i = 0; i < 1024; ++i) {
        char* d = new char[8];
        std::cin.read(d, 8);
        int64_t d2 = *reinterpret_cast<int64_t*>(d);
        if (d2 < 0) {
            break;
        }
        if (i < 50) {
            betterReference<int64_t> p2((int64_t*)alloca(8));
            betterReference<int64_t> p3(stacktoheap(p2.ref, 8), (size_t) 8);
        } else {
            
        }
        delete[] d;
    }

    // Entry point for the program
    return 0;
}