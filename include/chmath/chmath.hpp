#ifndef __CHMATH__HPP
#define __CHMATH__HPP

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <limits>
#include <type_traits>
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <ranges>

#include "common.hpp"

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
		// TODO: why did I put this here?
		// T diff{max-min};
		std::vector<T> points(num);
		std::ranges::generate(points, [&]() -> T {
			return min + (max-min)/num * i++;
		});
		return points;
	}


	// way to allow aliasing of the filesystem namespace internally
	// worst case, this gives chmath::fs to the user... who cares!
	namespace fs
	{
		using std::filesystem::path;
		using std::filesystem::exists;
		using std::filesystem::create_directory;
	}


	template <typename T>
	concept Arithmetic = std::is_arithmetic<T>::value;
	
	/** @brief provides a (relatively) simple Gnuplot interface
	 */
	template <Arithmetic T>
	class Plotter
	{
		using range_type = float;
		using data_type = T;
		using enum_type = uint32_t;
		
	private:
		/** @name file handling
		 */
		///@{
		std::string _script_file_name{}; //!< name of output file
		fs::path _script_file_path{}; //!< path to script file
		std::ofstream _script_file{}; //!< filestream for handling script file
		std::string _data_file_name{}; //!< name of data file
	    fs::path _data_file_path{}; //!< path to data file
		std::ofstream _data_file{}; //!< filestream for handling data file
		///@}

		static const std::string DATA_DIR;
		static const std::string SCRIPT_DIR;

		/** @name plot data
		 */
		///@{
		std::vector<data_type> _X{}, _y{};
		///@}

		/** @name Plot configuration
		 */
		///@{
		enum_type _output_file_type{}; //!< e.g. PNG, PDF
		std::string _output_file_name{};
		
		std::string _title{};
		std::string _xlabel{};
		std::string _ylabel{};

		range_type _xmin{}, _xmax{};
		range_type _ymin{}, _ymax{};

		enum_type _line_width{};
		enum_type _line_style{};
		enum_type _line_color{}; 
		enum_type _legend_loc{};
		enum_type _border_thickness{}; //!< plot outline border
		///@}

		uint _plot_num{};
		std::vector<std::string> _plots{}; //!< list of created plots

	public:
		/** @brief Enum to provide various line styles
		 */
		enum LineStyle : enum_type
		{
			SOLID = 0,
			DASHED
		};

		/** @brief Enum to provide locations for a plot legend
		 */
		enum LegendLoc : enum_type
		{
			NONE = 0,
			TOPLEFT,
			TOPRIGHT,
			BOTTOMRIGHT,
			BOTTOMLEFT,
		};

		/** @brief Enum to provide different output file types
		 */
		enum OutputFileType : enum_type
		{
			AUTO = 0, //!< guess based on extension
			PNG,
			PDF,
		};

		/** @brief Enum to provide different border widths
		 */
		enum BorderThickness : enum_type
		{
			EXTRA_THIN,
			THIN,
			NORMAL,
			THICK,
			EXTRA_THICK,
		};

		/** @brief Enum to provide various line colors
		 */
		enum LineColor : enum_type // must be specific here!
		{
			WHITE   = 0xFFFFFF,
			BLACK   = 0x000000,
			RED     = 0xFF0000,
			GREEN   = 0x00FF00,
			BLUE    = 0x0000FF,
			YELLOW  = 0xFFFF00,
			TEAL    = 0x00FFFF,
			MAGENTA = 0xFF00FF,
		};


	private:
		/** @name Defaults/constants
		 */
		///@{
		constexpr static enum_type _DEFAULT_LW = 1;
		constexpr static enum_type _DEFAULT_LC = LineColor::BLACK;
		constexpr static enum_type _DEFAULT_LS = LineStyle::SOLID;
		constexpr static enum_type _DEFAULT_LEGEND_LOC = LegendLoc::NONE;
		constexpr static enum_type _DEFAULT_BORDER_THICKNESS = BorderThickness::NORMAL;
		///@}

	public:
		/** @brief Default destructor. Does nothing.
		 */
		Plotter() = default;

		/** @brief Default destructor. Does nothing.
		 */
		virtual ~Plotter() = default;


		
#define GEN_PLOTTER_SET0(name, T)										\
		inline Plotter& CHMATH_JOIN(set_, name)(T const& name)			\
		{																\
			CHMATH_JOIN(_,name) = name;									\
			return *this;												\
		}
#define GEN_PLOTTER_SET(name, T) GEN_PLOTTER_SET0(name, T)

		GEN_PLOTTER_SET(title, std::string)
		GEN_PLOTTER_SET(xlabel, std::string)
		GEN_PLOTTER_SET(ylabel, std::string)
	    GEN_PLOTTER_SET(xmin, range_type)
		GEN_PLOTTER_SET(xmax, range_type)
		GEN_PLOTTER_SET(ymin, range_type)
		GEN_PLOTTER_SET(ymax, range_type)
		GEN_PLOTTER_SET(line_width, enum_type)
		GEN_PLOTTER_SET(line_style, enum_type)
	    GEN_PLOTTER_SET(legend_loc, enum_type)
		GEN_PLOTTER_SET(border_thickness, enum_type)
		GEN_PLOTTER_SET(line_color, enum_type)

		
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
		inline Plotter& setOutputFile(std::string const& file_name, uint output_file_type=AUTO)
		{
			if (output_file_type == AUTO)
			{
				std::string::size_type dot_pos = file_name.find(".");
				if (dot_pos == std::string::npos)
				{
					std::cerr << "setOutputFile(): Failed to deduce the type of the output file."
							  << " Either specify an extension in the file name,"
							  << " or override the file type.\n";

					exit(EXIT_FAILURE);
				}

				std::string ext = file_name.substr(dot_pos + 1);

				// test if pdf
				if (ext.compare("pdf") == 0)
					output_file_type = PDF;
				else if (ext.compare("png") == 0)
					output_file_type = PNG;
				else
				{
					std::cerr << "setOutputFile(): File extension '" << ext
							  << "' isn't recognized."
							  << " Either specify a known extension,"
							  << " or overrid ethe file type.\n";

					exit(EXIT_FAILURE);
				}
			}

			_output_file_name = file_name;
			_output_file_type = output_file_type;
		
			return *this;
		}

		/** This function creates the output data file
		 *  that the gnuplot script will read from.
		 *  It places this file in a directory titled: "data-dir"
		 *  (in anticipation of additional data files for more plots)
		 *  It will also store the X and y values internally for later use
		 *
		 *  @param X: the X values
		 *  @param y: the y values at each corresponding x value in @a X
		 *  @param plot_title: [OPTIONAL] title for plot (for multiple plots). default is nothing
		 *  @return reference to self
		 */
		Plotter& plot(std::vector<data_type> const& X, std::vector<data_type> const& y, std::string plot_title="")
		{
			if (X.size() != y.size())
			{
				std::cerr << "plot(): X size(" << X.size() << ") and y size(" << y.size() << ") differ.\n";
				exit(EXIT_FAILURE);
			}

		    _data_file_path = DATA_DIR;
			if (!fs::exists(_data_file_path))
			{
				if(!fs::create_directory(_data_file_path))
				{
					std::cerr << "plot(): Failed to create data output directory '" << _data_file_path.string() << "'.\n";
					exit(EXIT_FAILURE);
				}
			}

			std::ostringstream data_file_name{};
			data_file_name << "data" << ++_plot_num << ".dat";

			_data_file_path /= data_file_name.str();
			_data_file.open(_data_file_path.make_preferred(), std::ios_base::out);
			if (!_data_file)
			{
				std::cerr << "plot(): failed to open file '" << _data_file_path.string() << "'.\n";
				exit(EXIT_FAILURE);
			}

			_data_file << std::setprecision(std::numeric_limits<data_type>::max_digits10);
			for (std::tuple<data_type,data_type> x : std::ranges::zip_view{X, y})
			{
				_data_file  << std::get<0>(x) << '\t'
						  << std::get<1>(x) << '\n';
			}
		    
			_X = X;
			_y = y;
			_title = plot_title;

			_data_file.close();

			return *this;
		}


		/** This function will assemble the gnuplot script and place it in "script.gplt"
		 *  It will then call the `system()` C stdlib function to invoke gnuplot
		 *  @NOTE Ensure gnuplot is present on your path, else this will fail.
		 */
		inline Plotter& save()
		{
			_script_file_path = SCRIPT_DIR;
			if (!fs::exists(_script_file_path))
			{
				if (!fs::create_directory(_script_file_path))
				{
					std::cerr << "plot(): Failed to create script output directory\n";
					exit(EXIT_FAILURE);
				}
			}

			_script_file_name = "script.gplt";
			_script_file_path /= _script_file_name;
			_script_file.open(_script_file_path);
			if (!_script_file)
			{
				std::cerr << "Plotter::save(): failed to open script file '" << _script_file_path.string() << "'.\n";
				exit(EXIT_FAILURE);
			}

			std::string terminal_type{};
			switch (_output_file_type)
			{
				case PNG: terminal_type = "pngcairo"; break;
				case PDF: terminal_type = "pdfcairo"; break;
				case AUTO:
				default:
				{
					std::cerr << "Plotter::save(): invalid output file/terminal type\n";
					exit(EXIT_FAILURE);
				}
			}
			_script_file << "set terminal " << terminal_type << " enhanced notransparent\n";
			_script_file << "set output '" << _output_file_name << "'\n";
			
			// I could throw an error if the user doesn't specify a valid range,
			// but instead I'll just iterate through the provided lists
			// and pick out the actual min and max +- 3%
			if ((_xmin == 0 and _xmax == 0) || _xmin == _xmax)
			{
				range_type xmin = *std::min_element(_X.begin(), _X.end());
				range_type xmax = *std::max_element(_X.begin(), _X.end());

				range_type three_percent = (xmax-xmin)*static_cast<range_type>(0.03);
				// three_percent = 0.0;
				
				_xmin = xmin - three_percent;
				_xmax = xmax + three_percent;
			}
			if ((_ymin == 0 and _ymax == 0) || _ymin == _ymax)
			{
				range_type ymin = *std::min_element(_y.begin(), _y.end());
				range_type ymax = *std::max_element(_y.begin(), _y.end());

				range_type three_percent = (ymax-ymin)*static_cast<range_type>(0.03);
				// three_percent = 0.0;
				
				_ymin = ymin - three_percent;
				_ymax = ymax + three_percent;
			}			
			_script_file << _genLine_xrange() << '\n';
			_script_file << _genLine_yrange() << '\n';
			_script_file << "set xlabel '" << _xlabel << "'\n";
			_script_file << "set ylabel '" << _ylabel << "'\n";
			_script_file << "set title '" << _title << "'\n";
	    			
			_script_file << "plot '" << _data_file_path.string() << "' with lines title '" << _title << "'\n";
			_script_file.close();

			std::ostringstream command{};
			command << "gnuplot " << _script_file_path.string();
			std::cout << "Plotter::save(): the command is: " << command.str() << " \n";

			if (system(command.str().c_str()) != 0)
			{
				std::cerr << "save(): Failed to create child shell to call gnuplot, or gnuplot call failed. Is it on your PATH?\n";
				exit(EXIT_FAILURE);
			}

			return *this;
		}


	private:
		/** @name Gnuplot script line generators
		 *  These are helper functions that take some plot information
		 *  and produce the corresponding line that would be placed
		 *  inside a gnuplot script
		 *  These are separated since there is bounds-checking and other things
		 *  that can be cumbersome in other functions.
		 */
		///@{
		/** @brief generates a yrange line */
		inline std::string _genLine_yrange()
		{
			if (_ymin >= _ymax)
			{
				std::cerr << "_genLine_yrange(): minimum y value ("
						  << _ymin
						  << ") is equal to or larger than maximum y value ("
						  << _ymax
						  << "\n";
				exit(EXIT_FAILURE);
			}
		
			std::ostringstream ss{};
			ss << "set yrange [" << _ymin << ":" << _ymax << "]";
			return ss.str();
		}
		
		/** @brief generates an xrange line */
		inline std::string _genLine_xrange()
		{
			if (_xmin >= _xmax)
			{
				std::cerr << "_genLine_xrange(): minimum x value ("
						  << _xmin
						  << ") is equal to or larger than maximum x value ("
						  << _xmax
						  << "\n";
				exit(EXIT_FAILURE);
			}
		
			std::ostringstream ss{};
			ss << "set xrange [" << _xmin << ":" << _xmax << "]";
			return ss.str();
		}

		///@}
	};

	template <Arithmetic T>
	const std::string Plotter<T>::DATA_DIR{"data"};

	template <Arithmetic T>
	const std::string Plotter<T>::SCRIPT_DIR{"scripts"};
	
} // namespace chmath

#endif // __CHMATH__HPP
