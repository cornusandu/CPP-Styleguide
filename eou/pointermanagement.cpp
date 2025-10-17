#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <cstring>

template <typename T>
struct Pointer {
    T* pointer = nullptr;
    Pointer(T* ptr) : pointer(ptr) {}
    Pointer() : pointer(nullptr) {}
    Pointer(const Pointer&) = delete; // No copy
    Pointer& operator=(const Pointer& other) = delete; // No copy
    Pointer operator=(Pointer ptr) = delete; // No copy
    Pointer& operator=(Pointer&& other) = delete;
    ~Pointer() { delete pointer; };
};

template <typename T>
struct betterReference {
    T* ref;

    // Constructors
    betterReference(T* r) : ref(r) {};
    betterReference(T& r) : ref(&r) {};
    betterReference(void* r, size_t size) : ref((T*)r) {
        #ifdef THROWON_BREFERENCE_TOVOID
           throw std::invalid_argument("betterReference recieved a void* pointer.");
        #endif
    };

    const betterReference& operator=(const betterReference& other) { return other; };
    betterReference& operator=(betterReference& other) { this->ref = other.ref; return *this; };
    betterReference& operator=(T* r) { ref = r; return *this; };
    betterReference& operator=(T& r) { ref = &r; return *this; };

    // Implicit conversion to T
    operator T() const { return *ref; }

    // Optional: allow implicit conversion to pointer if needed
    operator T*() const { return ref; }

    // Optional: reference access
    T& operator*() const { return *ref; }
    T* operator->() const { return ref; }

    // Optional: assignment through the reference
    betterReference& operator=(const T& value) {
        *ref = value;
        return *this;
    }
};

void* stacktoheap(void* p, size_t size) {
    void* h = malloc(size);
    memcpy(h, p, size);
    return h;
}

void* stacktoheap(nullptr_t p, size_t size) {
    #ifdef THROWON_INVALIDPTR
        throw std::invalid_argument("Invalid pointer (nullptr)");
    #endif
    return nullptr;
}