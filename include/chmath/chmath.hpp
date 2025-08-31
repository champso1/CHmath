#ifndef __CHMATH__HPP
#define __CHMATH__HPP

#include <algorithm>
#include <cstdlib>
#include <ios>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <ranges>

typedef unsigned int uint;

namespace chmath
{

	/** @brief returns @a num equally space points between @min and @max
	 *  @param min: The minimum bound (inclusive)
	 *  @param max: The maximum bound (exclusive)
	 *  @return An std::vector containing the points
	 *  @note For a version of linspace that has an inclusive upper bound
	 *  see @a linspace2
	 */
    template <typename T>
	std::vector<T> linspace(T min, T max, uint num)
	{
		if (max < min)
		{
			std::cerr << "linspace(): max(" << max << ") is smaller than min(" << min << ")\n";
			exit(EXIT_FAILURE);
		}
	    
		uint i{0};
		T diff{max-min};
		std::vector<T> points(num);
		std::ranges::generate(points, [min,max,num,diff,&i](){
			return min + (max-min)/num * i++;
		});
		return points;
	}

	/** @brief returns @a num equally space points between @min and @max
	 *  @param min: The minimum bound (inclusive)
	 *  @param max: The maximum bound (inclusive)
	 *  @return An std::vector containing the points
	 *  @note For a version of linspace that has an exclusive upper bound
	 *  see @a linspace
	 */
	template <typename T>
	std::vector<T> linspace2(T min, T max, uint num)
	{
		if (max < min)
		{
			std::cerr << "linspace2(): max(" << max << ") is smaller than min(" << min << ")\n";
			exit(EXIT_FAILURE);
		}

		// we simply make `max` one higher and add one more num for the inclusive bound
		T delta = (max-min)/static_cast<T>(num);
		return linspace(min, max+delta, num+1);
	}




	/** @brief provides a (relatively) simple Gnuplot interface
	 */
	class Plotter
	{
		using range_type = float;
		
	private:
		std::string _file_name{}; //!< name of output file
		std::ofstream _script{}; //!< filestream for handling output file
		std::filesystem::path _data_file_path{}; //!< name of data file
		std::fstream _data_file{}; //!< filestream for handling data

		/** @name plot data
		 */
		///@{
		std::vector<double> _X{}, _y{};
		std::string _legend{};
		///@}

		/** @name Plot style
		 */
		///@{
		std::string _title{}; //!< title of plot
		std::string _xlabel{}; //!< label of x axes
		std::string _ylabel{}; //!< label of y axes

		range_type _xmin{}, _xmax{}; //!< x range
		range_type _ymin{}, _ymax{}; //!< y range
		///@}

		uint _plot_num{};
		std::vector<std::string> _plots{}; //!< list of created plots

	public:
		/** @brief Enum to provide various line styles
		 */
		enum LineStyle : uint
		{
			SOLID = 0,
			DASHED
		};

		/** @brief Enum to provide locations for a plot legend
		 */
		enum LegendLoc : uint
		{
			NONE = 0,
			TOPLEFT,
			TOPRIGHT,
			BOTTOMRIGHT,
			BOTTOMLEFT
		};

		/** @brief Enum to provide different output file types
		 */
		enum OutputFileType : uint
		{
			AUTO = 0, //!< guess based on extension
			PNG,
			PDF
		};


	private:
		/** @name Defaults/constants
		 */
		///@{
		constexpr static uint _DEFAULT_LW = 1; //!< default line-width
		constexpr static uint _DEFAULT_LS = SOLID; //!< default line-style
		constexpr static uint _DEFAULT_LEGEND_LOC = NONE; //!< default legend location
		///@}

	public:
		/** @brief Default destructor. Does nothing.
		 */
		Plotter() = default;

		/** @brief Default destructor. Does nothing.
		 */
		virtual ~Plotter() = default;

		
		/** @brief sets the plot's title to @a title
		 *  @param title: The desired title
		 */
		inline Plotter& setTitle(std::string const& title)
		{
			_title = title;
			return *this;
		}

		/** @brief sets the plot's x-axis label
		 *  @param label: The desired label
		 */
		inline Plotter& setXLabel(std::string const& label)
		{
			_xlabel = label;
			return *this;
		}

		/** @brief sets the plot's y-axis label
		 *  @param label: The desired label
		 */
		inline Plotter& setYLabel(std::string const& label)
		{
			_ylabel = label;
			return *this;
		}

		/** @brief sets the plot's y-axis label
		 *  @param labels: A pair representing (xlabel, ylabel)
		 */
		inline Plotter& setAxisLabels(
			std::pair<std::string, std::string> const& labels)
		{
			_xlabel = labels.first;
			_ylabel = labels.second;
			return *this;
		}

		/** @brief sets the plot's ymin value
		 *  @param ymin: The desired minimum y value
		 */
		inline Plotter& setYMin(range_type ymin)
		{
			_ymin = ymin;
			return *this;
		}

		/** @brief sets the plot's ymax value
		 *  @param ymax: The desired maximum y value
		 */
		inline Plotter& setYMax(range_type ymax)
		{
			_ymax = ymax;
			return *this;
		}

		/** @brief sets the plot's y range
		 *  @param range: a pair representing (ymin, ymax)
		 */
		inline Plotter& setYRange(
			std::pair<range_type, range_type> const& range)
		{
			_ymin = range.first;
			_ymax = range.second;
			return *this;
		}

		/** @brief sets the plot's xmin value
		 *  @param xmin: The desired minimum x value
		 */
		inline Plotter& setXMin(range_type xmin)
		{
			_xmin = xmin;
			return *this;
		}

		/** @brief sets the plot's xmax value
		 *  @param xmax: The desired maximum x value
		 */
		inline Plotter& setXMax(range_type xmax)
		{
			_xmax = xmax;
			return *this;
		}

		/** @brief sets the plot's x range
		 *  @param range: a pair representing (xmin, xmax)
		 */
		inline Plotter& setXRange(
			std::pair<range_type, range_type> const& range)
		{
			_xmin = range.first;
			_xmax = range.second;
			return *this;
		}


		/** @brief sets the output file to place the generated plot
		 *  @param file_name: the name of the file (with the extension)
		 *  @param output_file_type: [OPTIONAL] override the output file type
		 *
		 *  By default, i.e. by just specifying a name,
		 *  the function will guess what the type of the output file is
		 *  by the extension.
		 *  This can be overriden (if you want to, for whatever reason
		 *  save a pdf as a .png) by specifying a file type.
		 */
		Plotter& setOutputFile(
			std::string const& file_name,
			uint output_file_type=AUTO);

		


		/** @brief plots X vs. y
		 *  @param X: the x-values
		 *  @param y: the y-values
		 *  @note X and y must have the same size
		 */
		template <typename T>
		requires std::is_arithmetic<T>::value
		void plot(std::vector<T> const& X, std::vector<T> const& y, std::string const& legend="")
		{
			if (X.size() != y.size())
			{
				std::cerr << "plot(): X size(" << X.size() << ") and y size(" << y.size() << ") differ.\n";
				exit(EXIT_FAILURE);
			}

			std::filesystem::path script_dir{"data-dir"};
			if (!std::filesystem::create_directory(script_dir))
			{
				std::cerr << "plot(): Failed to create script output directory\n";
				exit(EXIT_FAILURE);
			}

			std::ostringstream data_file_name{};
			data_file_name << "data" << ++_plot_num << ".dat";

		    _data_file_path = script_dir;
			_data_file_path /= data_file_name.str();
			_data_file.open(_data_file_path.make_preferred(), std::ios_base::out);
			if (!_data_file)
			{
				std::cerr << "plot(): failed to open file '" << _data_file_path.string() << "'.\n";
				exit(EXIT_FAILURE);
			}

			for (std::tuple<T,T> x : std::ranges::zip_view{X, y})
			{
				_data_file << std::get<0>(x) << '\t'
						  << std::get<1>(x) << '\n';
			}

			_data_file.close();

			_legend = legend;
		}

		void plot(std::vector<double> const& X, std::vector<double> const& y, std::string const& legend="")
		{
			if (X.size() != y.size())
			{
				std::cerr << "plot(): X size(" << X.size() << ") and y size(" << y.size() << ") differ.\n";
				exit(EXIT_FAILURE);
			}

			std::filesystem::path script_dir{"data-dir"};
			if (!std::filesystem::exists(script_dir))
			{
				if(!std::filesystem::create_directory(script_dir))
				{
					std::cerr << "plot(): Failed to create script output directory\n";
					exit(EXIT_FAILURE);
				}
			}

			std::ostringstream data_file_name{};
			data_file_name << "data" << ++_plot_num << ".dat";

		    _data_file_path = script_dir;
			_data_file_path /= data_file_name.str();
			_data_file.open(_data_file_path.make_preferred(), std::ios_base::out);
			if (!_data_file)
			{
				std::cerr << "plot(): failed to open file '" << _data_file_path.string() << "'.\n";
				exit(EXIT_FAILURE);
			}

			for (std::tuple<double,double> x : std::ranges::zip_view{X, y})
			{
				_data_file << std::get<0>(x) << '\t'
						  << std::get<1>(x) << '\n';
			}
			_X = X;
			_y = y;

			_data_file.close();

			_legend = legend;
		}

		/** @brief plots X vs. all elements of y on one plot
		 *  @param X: the x-values
		 *  @param y: a list of vectors of y-values
		 *  @note X and all elements of y must be the same size
		 *
		 *  Often, multiple functions are calculated
		 *  with the same underlying grid/x-values;
		 *  This allows one to plot the results of those functions
		 *  in one call
		 */
		template <typename T>
		requires std::is_arithmetic<T>::value
		[[deprecated("This function currently does not work")]]
		void plot(
			std::vector<T> const& X,
			std::vector<std::vector<T>> const& y)
		{
			// we can just loop over all and plot each one
			std::ranges::for_each(y, [&](std::vector<T> const& _y){
				if (X.size() != _y.size())
				{
					std::cerr << "plot(): X size(" << X.size() << ") and y size(" << _y.size() << ") differ.\n";
					exit(EXIT_FAILURE);
				}

				plot(X, _y);
			});
		}


		/** @brief this function more or less assembles the script and saves the plot
		 */
		inline void save()
		{
			if (_xmin == 0 and _xmax == 0)
			{
			    auto min_iter = std::min_element(_X.begin(), _X.end());
				auto max_iter = std::max_element(_X.begin(), _X.end());

				_xmin = *min_iter;
				_xmax = *max_iter;
			}
			if (_ymin == 0 and _ymax == 0)
			{
			    auto min_iter = std::min_element(_y.begin(), _y.end());
				auto max_iter = std::max_element(_y.begin(), _y.end());

				_ymin = *min_iter;
				_ymax = *max_iter;
			}
			
			_script << "set xrange [" << _xmin << ":" << _xmax << "]\n";
			_script << "set yrange [" << _ymin << ":" << _ymax << "]\n";
			_script << "set xlabel '" << _xlabel << "'\n";
			_script << "set ylabel '" << _ylabel << "'\n";
			_script << "set title '" << _title << "'\n";

			std::string file_path{_data_file_path.filename().string()};
			std::string parent_path{_data_file_path.parent_path()};
			
			_script << "set loadpath '" << parent_path << "'\n";
			_script << "plot '" << file_path << "' with lines title '" << _legend << "'\n";
			_script.close();

			std::ostringstream command{};
			command << "gnuplot " << _file_name;
			std::cout << "Plotter::save(): the command is: " << command.str() << " \n";
			system(command.str().c_str());
		}


	private:
		/** @name Gnuplot script line generators
		 *  These are helper functions that take some plot information
		 *  and produce the corresponding line that would be placed
		 *  inside a gnuplot script
		 */
		///@{
		/** @brief generates a yrange line */
		std::string _genLine_yrange();
		
		/** @brief generates an xrange line */
		std::string _genLine_xrange();

		/** @brief generates the title line */
		std::string _genLine_title();
		///@}
	};
	
} // namespace chmath

#endif // __CHMATH__HPP
