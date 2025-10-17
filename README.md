# C++ Styleguide
## Disclaimer
This is my own styleguide. You don't have to agree or follow anything that's written here.
## Formatting
### Function Braces (1)
The opening brace of a function should be placed on the same line as the function definition, seperated by a space from the class definition. The closing brace should be placed on the line following the return statement.
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
Identation should be done using TABS of 4 spaces each.
### File Format (1)
Files should be formatted with UTF-8 and LF, regardless of operating system. (This does not include binaries)
## Building
### Position-Independent
All output binaries should be as position-independent as possible, regardless of usecase. (Excluding kernel-level programming, or binaries where size is a priority)
On Linux, position-independent binaries should be made with the use of `-fPIC` or `-fPIE -pie` flags for g++.
On Windows, position-independent binaries should be made ith the use of `-O2 -fno-common -Wl,—dynamicbase -Wl,—high-entropy-va -Wl,—nxcompat` flags for g++. Because Windows uses PE instead of ELF, this is the closest you can get to truly position-independent Windows code.