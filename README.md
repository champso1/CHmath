# CHmath

This is going to be a repository for me to place all of the mathematical routines I have done in Computational Physics 1 and will do in Computational Physics 2. The former will require conversion from Python to C++, but that's fine. 

# Building

CMake >= 3.30 and C++23 is required, as well as the Ninja build tool. Further, your compiler must have support for modules.

The building process is standard CMake building:

```
mkdir build
cd build
cmake ..
ninja
```

Alternatively, there are some presets setup for Unix `clang` users, which allows one to do

```
cmake -S . --preset clang-X-linux
cd bin/clang-X-linux
ninja
```

where `X` can be either `release` or `debug`. Successfully compiling will yield a `tests` folder, with two tests inside: `plotting` and `table`, inside of which each contain an executable that showcases the corresponding functionality.

# Mathematical Routines

This will be populated with a list and brief description of the major mathematical routines that will be implemented.

- `linspace(min, max, num)`: Generates a list of `num` linearly spaced points between `min` and `max`. The upper bound is not inclusive.
- `logspace(min, max, num)`: Generates a list of `num` logarithmically points between `min` and `max`. The upper bound is not inclusive.


# Changelog

A changelog for each version is present [here](Changelog.md). I'll try and keep it updated with each major commit. The version numbering system is in three parts, the rightmost number is very minor and reserved for bug fixes. The next-to-rightmost number (i.e. the middle number) is reserved for a new routine implementation. The leftmost number is reserved at the moment for once everything is implemented, meaning it will be in version 0.X.X until everything is implemented, after which it will become version 1.X.X. I'm not sure what will update that number afterwards, but oh well.

