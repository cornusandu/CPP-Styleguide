# [C++ Styleguide](https://cornusandu.github.io/CPP-Styleguide/)
## Disclaimer
This is my own styleguide, meant mainly for me. You don't have to agree or follow anything that's written here.
## Formatting
### Function Braces (1)
The opening brace of a function should be placed on the same line as the function definition, separated by a space from the class definition. The closing brace should be placed on the line following the last line of the function.
```cpp
int main() {
	// do something
	return 0;
}
```

### Function Braces (2)
Braces should not be used for a function if you are only declaring the signature.
```cpp
int main();
```

### Forward-Declarations (1)
Always use `extern` when forward-declaring a function that is NOT defined in the same file. This informs the reader that the function is defined somewhere else.

### Forward-Declarations (2)
Always use `extern` when forward-declaring a variable in C++, as just `T variable;` may initialise it.

### Identation (1)
Identation should be done using TABS (viewed as 4-space tabs).

### File Format (1)
Files should be formatted with UTF-8 and LF, regardless of operating system. Only exception is Powershell scripts, which should use UTF-16+BOM and CRLF.

### Pointer Management (1)
Avoid smart pointers unless:

* An external library requires them
* Normal memory management becomes difficult or too complex
* You are working with extremely large volumes of data, where manual memory management becomes close to impossible, or difficult

### Pointer Management (2)
Store pointers as:

* `unsigned char*`: for pointer arithmetic
* `void*`: for storage, or function input/output (or pointer arithmetic, on toolchains and platforms where `sizeof(void) == 1`, although this is discouraged)
* `T*`: function input/output, internal type conversions, storage, or pointer arithmetic

### Pointer Management (3)
Always use `unsigned char*` instead of `char*` (or `signed char*`, if you for some reason need it to be signed).

## Building
### Position-Independent
All output binaries should be as position-independent as possible, regardless of usecase. (Excluding kernel-level programming, or binaries where size is a priority) <br>
On Linux, position-independent binaries should be made with the use of `-fPIC` or `-fPIE -pie` flags for g++. <br>
<!-- On Windows, position-independent binaries should be made ith the use of `-O2 -fno-common -Wl,--dynamicbase -Wl,--high-entropy-va -Wl,--nxcompat` flags for g++. Because Windows uses PE instead of ELF, this is the closest you can get to truly position-independent Windows code.<br> -->

### Debug Builds
Debug builds should be built with the following arguments:

* Linux: `g++ -g -O1 -fPIE -pie` or `g++ -g -O1 -fPIC -fsanitize=undefined -Wshadow`
* Windows: `g++ -g -O1 -fno-common, -Wl,--high-entropy-va -fsanitize=undefined -Wshadow`

Production builds should be built with the following arguments:

* Linux: `-O3`/`-Ofast`, `-fstack-protector-strong`/`-fstack-protector-all`, `-fPIE -pie`/`-fPIC`, `-fno-delete-null-pointer-checks`, `-Wtrampolines`, `-D_FORTIFY_SOURCE=2`/`-D_FORTIFY_SOURCE=3`, `-Werror=shadow`, `-Wextra`, `-fwrapv`, `-fstack-clash-protection`, `-Werror=return-type`, `-fno-strict-aliasing`, `-fno-strict-overflow`
* Windows: `-O3`/`-Ofast`, `-fstack-protector-strong`/`-fstack-protector-all`, `-fno-delete-null-pointer-checks`, `-Wtrampolines`, `-D_FORTIFY_SOURCE=2`/`-D_FORTIFY_SOURCE=3`, `-Werror=shadow`, `-Wextra`, `-fwrapv`, `-fstack-clash-protection`, `-Werror=return-type`, `-fno-strict-aliasing`, `-fno-strict-overflow`

!!! caution
	Please note that `-Ofast` introduces aggressive optimization that my slightly alter floating point math, and may break some things/

We also recommend the following flags, to forcefully prevent dangerous behaviour, as well as unused code:

* `-Werror=uninitialized`
* `-Werror=maybe-uninitialized`
* `-Werror=use-after-free`
* `-Werror=use-after-scope`
* `-Werror=free-nonheap-object`
* `-Werror=double-promotion`
* `-Wmisleading-indentation`
* `-Werror=unused-parameter`
* `-Werror=unused-variable`
* `-Werror=sequence-point`
* `-Werror=stringop-overread`
* `-Werror=format-security`
* `-Werror=array-bounds`
* `-Wstringop-overflow`

!!! tip
	With the following flags, the presence of any unused parameters will throw an error. Use the following macro to suppress said errors:

	```cpp
	#define UNUSED(x) ((void)(x))
	```   
	Example:
	```cpp
	void output(int number, int number2) {
	   UNUSED(number2);
	   std::cout << number;
	}
	```

	However, only do this if you plan on shortly starting to use the parameter in question, or have no choice but to keep it.

### Stack size
For builds making excessive use of `alloca()`, consider giving them a bigger stack size.

## Naming
Naming conventions that should be used:

* **Variables:** snake_case or camelCase
* **Top-Level Constants:** SCREAMING_SNAKE_CASE
* **Functions:** snake_case
* **Classes:** camelCase, PascalCase, or snake_case_t (snake_case ending with `_t`)
* **Macros:** SCREAMING_SNAKE_CASE

Temporary variables must start with an underscore, or end in `_tmp`.

## Reproducibility

For the sake of reproducibility, only use compilers that are part of the GNU compiler set (gcc/g++/ld/nasm).
