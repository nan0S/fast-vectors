# fast-vectors
Different fast vectors/containers in `C++`. My bachelor's thesis at UWr.

All vectors are implemented for `C++20` although they are guaranteed to be compatible with `C++17`. Vectors implement standard methods although they may break the standard sometimes if it is time-complexity beneficial.

# Status
- `static_vector` (done) - vector with static storage - that is fixed-size array 
- `vector` (in progress) - standard vector with dynamic allocation but allocator is using `mmap`/`munmap`/`mremap` 
<!-- - `small_vector` (future) - combination of `static_vector` and `vector`, at first uses static array, but can expand further -->
<!-- - `deque` (future) - double-ended vector, adding to front and back possible -->

# Info
Besides container implementations, repository containts different benchmarks and tests. If you are interested in using the containers only, type `make install` or copy whole `include` folder to your project destination, and include proper header, e.g.:
* `#include "uwr/container/static_vector.hpp"` if you did `make install`
* or `#include "include/container/static_vector.hpp"` if you copied `include` directory manually.

Of course `-I` compilation flag is recommended to avoid typing the whole path.

# Usage:
- `make install` - install code locally
- `make uninstall` - uninstall code locally
- `make all` - compile all sources
- `make run-all` - compile and run all tests and benchmarks
- `make run-benchmarks` - compile run all benchmarks one after another (warning: increase stack limit size with e.g. `ulimit -s unlimited`)
- `make run-tests` - compile and run all tests one after another
- `make run-bench_name` - compile and run only benchmark named `bench_name` (e.g. `make run-simple_benchmark`)
- `make run-test_name` - compile and run only test named `test_name` (e.g. `make run-simple_test`)

# Example
`make install`
```c
/* main.cpp */

#include <uwr/container/static_vector.hpp>
#include <uwr/container/vector.hpp>

int main() {
    uwr::static_vector<int, 5> sv { 1, 2, 3 };
    sv.push_back(10);
    sv.erase(sv.begin() + 1, sv.begin() + 3);

    uwr::vector<std::string> v { "this", "is", "example" };
    v.push_back("code");
    v.insert(v.end(), "1");
    v.clear();

    return 0;
}
```

# Requirements
Only for benchmarks and tests - [BOOST library](https://github.com/boostorg/boost), [GoogleTest](https://github.com/google/googletest) and [GoogleBenchmark](https://github.com/google/benchmark).
Containers itself do not require any additional libraries.

# Note
If you want to run benchmarks type: `sudo cpupower frequency-set --governor performance` before to get reliable results. This command disables CPU Frequency Scaling. To restore CPU Frequnecy Scaling type `sudo cpupower frequency-set --governor powersave` after the benchmarks.
