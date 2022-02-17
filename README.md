# fast-vectors
Fast vectors/containers library in `C++`.

All vectors are implemented for `C++20` although they are guaranteed to be compatible with `C++17`. Vectors implement standard methods although they may break the standard sometimes if it is time-complexity beneficial.

# Containers
- `static_vector` - vector with static storage
- `vector` - standard vector with dynamic storage, by default using `mremap` Linux system call for high performance

# Installation
This is headers-only library. You can manually copy `include` folder into your project/destination:

    # Clone the repo.
    git clone https://github.com/nan0S/fast-vectors
    # Change directory into repo.
    cd fast-vectors
    # Copy include folder to custom destination.
    cp -r include PROJECT/PATH   

or install library system-wide:

    # Clone the repo.
    git clone https://github.com/nan0S/fast-vectors
    # Change directory into repo.
    cd fast-vectors
    # Install library.
    sudo make install

# Other commands:
- `make install` - install code locally
- `make uninstall` - uninstall code locally
- `make all` - compile all sources
- `make run-all` - compile and run all tests and benchmarks
- `make run-benchmarks` - compile run all benchmarks one after another (warning: increase stack limit size with e.g. `ulimit -s unlimited`)
- `make run-tests` - compile and run all tests one after another
- `make run-bench_name` - compile and run only benchmark named `bench_name` (e.g. `make run-simple_vector_benchmark`)
- `make run-test_name` - compile and run only test named `test_name` (e.g. `make run-simple_test`)

# Example
`make install`
```c
/* main.cpp */

#include <uwr/static_vector.hpp>
#include <uwr/vector.hpp>

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
If you want to run benchmarks run command `sudo cpupower frequency-set --governor performance` before to get reliable results. This command disables CPU Frequency Scaling. To restore CPU Frequnecy Scaling type `sudo cpupower frequency-set --governor powersave` after the benchmarks. Also make sure to increase stack limit size with: `ulimit -s unlimited` for some benchmarks/tests.
