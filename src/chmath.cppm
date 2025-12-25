module;
#include <cstdlib>

export module chmath;
export import :table;
export import :plot;
import std;

export using uint = unsigned;

export namespace chmath
{

	/** @brief returns @a num equally spaced points between @a min and @a max
	 *  @param min: The minimum bound (inclusive)
	 *  @param max: The maximum bound (exclusive)
	 *  @return An std::vector containing the points
	 */
    template <typename T>
	std::vector<T> linspace(T min, T max, uint num)
	{
		if (max < min)
		{
			std::println("[ERROR] linspace(): 'max' ({}) is smaller than 'min' ({}).", max, min);
			exit(1);
		}
		if (num == 0)
		{
			std::println("[WARNING] linspace(): 'num' is zero -- returning empty vector");
			return {};
		}
	    
		uint i = 0;
		std::vector<T> points(num);
		std::ranges::generate(points, [&]() -> T {
			return min + (max-min)/num * i++;
		});
		return points;
	}

	/** @brief returns @a num logarithmically space points between @a min and @a max
	 *  @param min: The minimum bound (inclusive)
	 *  @param max: The maximum bound (exclusive)
	 *  @return An std::vector containing the points
	 */
	template <typename T>
	std::vector<T> logspace(T min, T max, uint num)
	{
		if (max < min)
		{
			std::println("linspace(): max ({}) is smaller than min ({})", max, min);
			exit(1);
		}
		if (num == 0)
		{
			std::println("[WARNING] linspace(): 'num' is zero -- returning empty vector");
			return {};
		}

		double logmin = std::log10(min);
		double logmax = std::log10(max);
		uint i = 0;
		std::vector<T> points(num);
		std::ranges::generate(points, [&]() -> T {
			double l = logmin + (logmax-logmin)/num * i++;
			return std::pow(10, l);
		});
		return points;
	}

	
	
} // namespace chmath

