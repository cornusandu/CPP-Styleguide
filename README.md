# [C++ Styleguide](https://cornusandu.github.io/CPP-Styleguide/)
## Disclaimer
This is my own styleguide, meant mainly for me. You don't have to agree or follow anything that's written here.
## Formatting
### Function Braces (1)
The opening brace of a function should be placed on the same line as the function definition, seperated by a space from the class definition. The closing brace should be placed on the line following the last line of the function.
```cpp
void main() {
    // do something
    return 0;
}
```
### Function Braces (2)
Braces should not be used for a function if you are only declaring the signature.
```cpp
void main();
```
### Identation (1)
Identation should be done using TABS.
### File Format (1)
Files should be formatted with UTF-8 and LF, regardless of operating system. (This does not include binaries)
### Pointer Management (1)
Avoid using smart pointers unless custom management becomes too complex.
### Pointer Management (2)
Use `_alloca()` or `alloca()` whenever you need to store basic types such as integers, but don't know EXACTLY how many there are. Make sure to never allocate too much memory with _alloca() or you'll run into Stack overflow errors. If you get a stack overflow error, always take a look at your _alloca() calls and try logging the amount of memory you allocate each time and in total on each line.
Use `malloc()` (or `new`) whenever you can't use `_alloca()` (long loops, large memory, types that have deconstructors).
### Pointer Management (3)
Storing pointer locations as `uintptr_t` instead of `char*` for pointer arithmetic is recommended. Only convert to `void*` when returning from a function. This is mainly for readability.
## Building
### Position-Independent
All output binaries should be as position-independent as possible, regardless of usecase. (Excluding kernel-level programming, or binaries where size is a priority)
On Linux, position-independent binaries should be made with the use of `-fPIC` or `-fPIE -pie` flags for g++.
On Windows, position-independent binaries should be made ith the use of `-O2 -fno-common -Wl,--dynamicbase -Wl,--high-entropy-va -Wl,--nxcompat` flags for g++. Because Windows uses PE instead of ELF, this is the closest you can get to truly position-independent Windows code.
### Debug Builds
Debug builds should be built with the following arguments:
* Linux: `g++ -g -O1 -fPIE -pie` or `g++ -g -O1 -fPIC`
* Windows: `g++ -g -O1 -fno-common, -Wl,--high-entropy-va`
### Stack size
For builds making excessive use of `_alloca()`, consider giving them a bigger stack size.