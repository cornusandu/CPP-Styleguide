// don't mind me

#include <cstdint>
#include <cstdlib>
#include <iostream>

template <typename T>
struct Pointer {
    T* pointer;
    Pointer(T* ptr) : pointer(ptr) {}
    Pointer() : pointer(nullptr) {}
    ~Pointer() { if (pointer == nullptr) {return;}; delete pointer; };
};

int main() {
    Pointer<int64_t> p[1024];
    for (size_t i = 0; i < 1024; ++i) {
        char* d;
        std::cin.read(d, 8);
        int64_t d2 = *reinterpret_cast<int64_t*>(d);
        if (d2 < 0) {
            break;
        }
        if (i < 50) {
            Pointer<int64_t> p2((int64_t*)alloca(8));
            *(p2.pointer) = d2;

            p[i] = p2;
        } else {
            Pointer<int64_t> p2(new int64_t);
            *(p2.pointer) = d2;

            p[i] = p2;
        }
    }

    // Entry point for the program
    return 0;
}