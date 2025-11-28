module;
#include <cstdlib>
#include <vector>
#include <print>
#include <algorithm>

export module chmath;
export import :table;
export import :plot;

export using uint = unsigned;

export namespace chmath
{

	/** @brief returns @a num equally space points between @a min and @a max
	 *  @param min: The minimum bound (inclusive)
	 *  @param max: The maximum bound (exclusive)
	 *  @return An std::vector containing the points
	 */
    template <typename T>
	std::vector<T> linspace(T min, T max, uint num)
	{
		if (max < min)
		{
			std::println(stderr, "linspace(): max ({}) is smaller than min ({})", max, min);
			exit(EXIT_FAILURE);
		}
	    
		uint i = 0;
		std::vector<T> points(num);
		std::ranges::generate(points, [min,max,num,&i]() -> T {
			return min + (max-min)/num * i++;
		});
		return points;
	}

	template <typename T>
	std::vector<T> logspace(T min, T max, uint num)
	{
		if (max < min)
		{
			std::println(stderr, "linspace(): max ({}) is smaller than min ({})", max, min);
			exit(EXIT_FAILURE);
		}

		double logmin = std::log10(min);
		double logmax = std::log10(max);
		uint i = 0;
		std::vector<T> points(num);
		std::ranges::generate(points, [logmin,logmax,num,&i]() -> T {
			double l = logmin + (logmax-logmin)/num * i++;
			return std::pow(10, l);
		});
		return points;
	}
	
} // namespace chmath

