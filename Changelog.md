# 0.1.0 (31 August, 2025)

Basic plotting functionality via Gnuplot is present, as well as a `linspace()` method that mirrors that of Numpy's. At the moment, the `Plotter` class is in very rough shape, it was tossed together quite quickly. There is definitely rooom for improvement in that respect.

# 0.1.1 (16 August, 2025)

Removed `linspace2()` because why would I have that, if the user wants an inclusive upper bound then they can work it out. Cleaned up the interface and improved the `plot()` and `save()` functions a bit. Some styles (and associated set-functions) are currently still unused, but things do work. These will be implemented next.

# 0.2.1 (28 November, 2025)

Moved to using C++ Modules. In principle, I'd like to also be able to use the `import std;` thing, but as far as I'm aware that's very experimental, and since modules are already experimental enough as it is (despite being introduced in C++20), I want to avoid that for now. I've also moved to using `std::print` as much as possible; it's so much better than the `<<` operator... Introduced `logspace` just to have another feature. Next steps are finishing the table implementation, followed by actual computational stuff.

# 0.3.1 (24 December, 2025)

Actually implemented a super basic table structure. Need to figure out a way to more easily/dynamically handle widths/precisions for various different types. Also would like to have a way to print a LaTeX table. Another big addition is the `CHutil` submodule which at the moment just provides a way to handle the creation of processes on Windows and UNIX-like systems and basic logging (still need to update the code to handle this new change). Sometime soon I'll actually add mathematical methods!
