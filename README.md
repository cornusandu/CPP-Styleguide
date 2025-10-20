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
