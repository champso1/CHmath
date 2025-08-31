#include <cmath>
#include <iterator>
#include <numbers>
#include <algorithm>
#include "chmath/chmath.hpp"

int main()
{
	std::vector<double> X{chmath::linspace(0.0, 2.0*std::numbers::pi, 100)};
	std::vector<double> y{};
	std::ranges::transform(
		X, std::back_inserter(y),
		[](const double x) -> double { return std::sin(x); }
	);

	chmath::Plotter plotter;
	plotter.setOutputFile("plot.pdf")
		.setTitle("Title for the plot")
		.setAxisLabels({"x-axis", "y-axis"});
	plotter.plot(X,y, "sin(x)");
	plotter.save();
	
	return 0;
}
