# CHmath

This is going to be a repository for me to place all of the mathematical routines I have done in Computational Physics 1 and will do in Computational Physics 2. The former will require conversion from Python to C++, but that's fine. 

# Building

C++23 is required. It doesn't have to be, I could've not written C++23 code, but I want to learn modern C++, so I did it anyway. This project also uses CMake >= 3.30.

The building process is standard CMake building:

```
mkdir build
cd build
cmake .. -G <generator> -DCMAKE_BUILD_TYPE=<build_type>
cmake --build . -- <build_tool_args>
```

Of course, if you want to leave everything default just a `cmake ..` and `cmake --build .` are sufficient. If using Ninja or GNU Makefiles, `<build_tool_args>` can be `-jN` to build in parallel, or just `-j` to utilize all cores. Performance differences between Release and Debug at this stage are insignificant, so that flag can be dropped as well.

Along with the library itself, it will build a single "test" called `main`, the executable for which is (from within the build directory) `tests/main/main` (or `tests/main/main.exe` on Windows). This file can be edited in (from the root directory) `tests/main/main.cpp`. It currently plots sin(x).


# Mathematical Routines

This will be populated with a list and brief description of the major mathematical routines that will be implemented.

- `linspace(min, max, num)`: Generates a list of `num` evenly spaced points between `min` and `max`. The upper bound is not inclusive.


# Changelog

A changelog for each version is present [here](Changelog.md). I'll try and keep it updated with each major commit. The version numbering system is in three parts, the rightmost number is very minor and reserved for bug fixes. The next-to-rightmost number (i.e. the middle number) is reserved for a new routine implementation. The leftmost number is reserved at the moment for once everything is implemented, meaning it will be in version 0.X.X until everything is implemented, after which it will become version 1.X.X. I'm not sure what will update that number afterwards, but oh well.

