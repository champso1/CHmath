# 0.1.0 (31 August, 2025)

Basic plotting functionality via Gnuplot is present, as well as a `linspace()` method that mirrors that of Numpy's. At the moment, the `Plotter` class is in very rough shape, it was tossed together quite quickly. There is definitely rooom for improvement in that respect.

# 0.1.1 (16 August, 2025)

Removed `linspace2()` because why would I have that, if the user wants an inclusive upper bound then they can work it out. Cleaned up the interface and improved the `plot()` and `save()` functions a bit. Some styles (and associated set-functions) are currently still unused, but things do work. These will be implemented next.
