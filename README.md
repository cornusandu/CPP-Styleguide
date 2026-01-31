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
Always forward-declare functions that you MAY need later at the start of a file. It takes a few seconds, and saves you some time later.

### Forward-Declarations (2)
Always use `extern` when forward-declaring a function that is NOT defined in the same file. This informs the reader that the function is defined somewhere else.

### Forward-Declarations (3)
Always use `extern` when forward-declaring a variable in C++, as just `T variable;` may initialise it.

### Identation (1)
Identation should be done using TABS.

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
* Linux: `-O3`/`-Ofast`, `-fstack-protector-strong`/`-fstack-protector-all`, `-fPIE -pie`/`-fPIC`, `-fno-delete-null-pointer-checks`, `-Wtrampolines`, `-D_FORTIFY_SOURCE=2`/`-D_FORTIFY_SOURCE=3 -Wshadow`
* Windows: `-O3`/`-Ofast`, `-fstack-protector-strong`/`-fstack-protector-all`, `-fno-delete-null-pointer-checks`, `-Wtrampolines`, `-D_FORTIFY_SOURCE=2`/`-D_FORTIFY_SOURCE=3 -Wshadow`

Please note that `-Ofast` introduces aggressive optimization that my slightly alter floating point math, and may break some things/

### Stack size
For builds making excessive use of `alloca()`, consider giving them a bigger stack size.

## Naming
Naming conventions that should be used:
* **Variables:** snake_case
* **Top-Level Constants:** SCREAMING_SNAKE_CASE
* **Functions:** snake_case
* **Classes:** camelCase, PascalCase, or snake_case_t (snake_case ending with `_t`)
* **Macros:** SCREAMING_SNAKE_CASE

Temporary variables must start with an underscore, or end in `_tmp`.

## Reproducibility

For the sake of reproducibility, only use compilers that are part of the GNU compiler set (gcc/g++/ld/nasm).

### UB

UB (Undefined Behaviour) is a scary topic for everyone.<br>
You **shouldn't avoid it**. You **should understand what happens when you run different mechanisms that cause 'UB'**, and you should **avoid behaviour that is unpredictable/unexpected/error-prone/unreproducible**.<br>
Understanding the compiler is always better than fearing the compiler.

<!--

## Compiler Optimizations

Not all of the following optimization flags have to be used, but they are valid options.

### 1. Use `-Ofast`

`-Ofast` enables all optimizations from `-O3` plus aggressive transformations that may violate strict IEEE or ISO compliance rules. It includes `-ffast-math`, allowing algebraic simplifications and less strict floating-point semantics.

**Command:**
`g++ -Ofast your_code.cpp -o your_program`

**Additional Enhancements:**
- Combine with strict aliasing and pointer analysis for slightly improved performance:
  `-Ofast -fipa-pta -fipa-cp-clone`
- On Linux or macOS, consider linker optimizations:
  `-Wl,--icf=all -Wl,--gc-sections -Wl,-O2`
- On Windows (MinGW/MSYS2), use:
  `-Wl,--enable-icf -Wl,--gc-sections -Wl,-O2`

---

### 2. Profile-Guided Optimization (PGO)

PGO uses runtime profiling data to guide the compiler’s optimization decisions, improving instruction layout, branch prediction, and cache locality.

**Step 1 – Instrument the program**
`g++ -O3 -fprofile-generate your_code.cpp -o your_program_gen`

**Step 2 – Run with representative data**
`./your_program_gen <test_input>`

**Step 3 – Recompile using the profile data**
`g++ -O3 -fprofile-use -fprofile-correction your_code.cpp -o your_program_pgo`

**Alternative (Linux only):**
Use automatic feedback from performance data with AutoFDO:
`g++ -O3 -fauto-profile=perf.data your_code.cpp -o your_program_autofdo`

**Notes:**
- Always use realistic workloads for profiling to ensure accurate branch and cache behavior.
- Combine with LTO for the best results (see below).
- `-fprofile-correction` corrects incomplete or mismatched profiling data.

---

### 3. Link-Time Optimization (LTO)

LTO enables whole-program optimization across multiple translation units. It allows interprocedural inlining, devirtualization, and dead code elimination at link time.

**Standard LTO:**
`g++ -O3 -flto your_code.cpp -o your_program`

**ThinLTO (faster linking, similar benefits):**
`g++ -O3 -flto=thin your_code.cpp -o your_program`

**PGO + LTO Combined:**
`g++ -O3 -flto -fprofile-use -fipa-pta -fdevirtualize-at-ltrans your_code.cpp -o your_program`

**Advantages:**
- Interprocedural optimization across files.
- Improved code locality and smaller binaries.
- `-fdevirtualize-at-ltrans` and `-fipa-pta` improve pointer and call devirtualization analysis.
- On Linux/macOS, add linker optimizations:
  `-Wl,--gc-sections -Wl,--icf=all -Wl,-O2`
- On Windows (MinGW/MSYS2), use:
  `-Wl,--enable-icf -Wl,--gc-sections -Wl,-O2`

---

### 4. Target-Specific Optimizations

Optimize code generation for the specific CPU architecture to leverage advanced instruction sets (SSE, AVX, AVX2, AVX-512, etc.).

**Command:**
`g++ -O3 -march=native -mtune=native`

**Examples:**
- For Intel CPUs:
  `-march=skylake-avx512` or `-march=haswell`
- For AMD CPUs:
  `-march=znver4`
- For Apple Silicon (Clang):
  `-mcpu=apple-m1`
- For ARM systems:
  `-march=armv8-a+simd`

**Additional Flags:**
- `-fprefetch-loop-arrays` – enable prefetching for loop data (Linux/macOS only).
- `-funroll-all-loops` – unroll all loops aggressively (caution: larger binaries).
- `-frename-registers` – improve register allocation in tight numeric loops.

**Notes:**
- `-march=native` automatically detects and enables all supported features on the build host.
- For portable binaries, explicitly specify an architecture instead of `native`.

---

### 6. Combine All Advanced Optimizations

For maximum optimization potential across modern systems, combine all major optimizations, profile-guided feedback, LTO, and CPU targeting.

**Command (Linux/macOS):**
`g++ -Ofast -ffast-math -funsafe-math-optimizations -fno-trapping-math -finline-functions -fno-math-errno -fivopts -faggressive-loop-optimizations -flto=thin -fprofile-use -fprofile-correction -fipa-pta -fdevirtualize-at-ltrans -march=native -mtune=native -funroll-loops -fomit-frame-pointer -fprefetch-loop-arrays -Wl,--icf=all -Wl,--gc-sections -Wl,-O2 your_code.cpp -o your_program`

**Command (Windows – MinGW/MSYS2):**
`g++ -Ofast -ffast-math -funsafe-math-optimizations -fno-trapping-math -finline-functions -fno-math-errno -fivopts -faggressive-loop-optimizations -flto=thin -fprofile-use -fprofile-correction -fipa-pta -fdevirtualize-at-ltrans -march=native -mtune=native -funroll-loops -fomit-frame-pointer -Wl,--enable-icf -Wl,--gc-sections -Wl,-O2 your_code.cpp -o your_program.exe`

**Notes:**
- Use `-fauto-profile=perf.data` on Linux for automatic profile feedback instead of manual PGO.
- This configuration activates all major optimization subsystems (PGO, ThinLTO, interprocedural analysis, and CPU specialization).
- Not suitable for debug builds or applications needing strict IEEE floating-point reproducibility.
- Gains vary with workload (commonly +20–50% over `-O3`).

-->
