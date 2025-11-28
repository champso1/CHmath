module;

#include <cstdlib>
#include <vector>
#include <print>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <ranges>

#define CHMATH_JOIN0(a, b) a ## b
#define CHMATH_JOIN(a, b) CHMATH_JOIN0(a, b)

export module chmath:plot;

export namespace chmath
{
	template <typename T>
	concept Arithmetic = std::is_arithmetic_v<T>;
	
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
		std::filesystem::path _script_file_path{}; //!< path to script file
		std::ofstream _script_file{}; //!< filestream for handling script file
		std::string _data_file_name{}; //!< name of data file
	    std::filesystem::path _data_file_path{}; //!< path to data file
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


		
#define GEN_PLOTTER_SET_FUNC0(name, T)									\
		inline Plotter& CHMATH_JOIN(set_, name)(T const& name)			\
		{																\
			CHMATH_JOIN(_,name) = name;									\
			return *this;												\
		}
#define GEN_PLOTTER_SET_FUNC(name, T) GEN_PLOTTER_SET_FUNC0(name, T)

		GEN_PLOTTER_SET_FUNC(title, std::string)
		GEN_PLOTTER_SET_FUNC(xlabel, std::string)
		GEN_PLOTTER_SET_FUNC(ylabel, std::string)
	    GEN_PLOTTER_SET_FUNC(xmin, range_type)
		GEN_PLOTTER_SET_FUNC(xmax, range_type)
		GEN_PLOTTER_SET_FUNC(ymin, range_type)
		GEN_PLOTTER_SET_FUNC(ymax, range_type)
		GEN_PLOTTER_SET_FUNC(line_width, enum_type)
		GEN_PLOTTER_SET_FUNC(line_style, enum_type)
	    GEN_PLOTTER_SET_FUNC(legend_loc, enum_type)
		GEN_PLOTTER_SET_FUNC(border_thickness, enum_type)
		GEN_PLOTTER_SET_FUNC(line_color, enum_type)

		
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
					std::println(stderr, "setOutputFile(): Failed to deduce the type of the output file.");
					std::println(stderr, "                 Either specify a supported extension in the file name,");
					std::println(stderr, "                 or override the filetype as a parameter.");

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
					std::println(stderr, "setOutputFile: File extension '{}' isn't recognized.", ext);
					std::println(stderr, "               Either specify a supported extension or override the filetype.");

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
				std::println(stderr, "plot(): X size ({}) and y size ({}) differ.", X.size(), y.size());
				exit(EXIT_FAILURE);
			}

		    _data_file_path = DATA_DIR;
			if (!std::filesystem::exists(_data_file_path))
			{
				if(!std::filesystem::create_directory(_data_file_path))
				{
					std::println(stderr, "plot(): Failed to create output data directory '{}'.", _data_file_path.string());
					exit(EXIT_FAILURE);
				}
			}

			std::ostringstream data_file_name{};
			data_file_name << "data" << ++_plot_num << ".dat";

			_data_file_path /= data_file_name.str();
			_data_file.open(_data_file_path.make_preferred(), std::ios_base::out);
			if (!_data_file)
			{
				std::println(stderr, "plot: Failed to open file '{}'.", _data_file_path.string());
				exit(EXIT_FAILURE);
			}

			for (std::tuple<data_type,data_type> x : std::ranges::zip_view{X, y})
				_data_file << std::get<0>(x) << '\t' << std::get<1>(x) << '\n';
		    
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
			if (!std::filesystem::exists(_script_file_path))
			{
				if (!std::filesystem::create_directory(_script_file_path))
				{
					std::println(stderr, "plot(): Failed to create script output directory.");
					exit(EXIT_FAILURE);
				}
			}

			_script_file_name = "script.gplt";
			_script_file_path /= _script_file_name;
			_script_file.open(_script_file_path);
			if (!_script_file)
			{
				std::println(stderr, "save(): failed to open the script file '{}'.", _script_file_path.string());
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
					std::println(stderr, "save(): Invalid output file/terminal type.");
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
			std::println("save(): the command is {}.", command.str());

			if (system(command.str().c_str()) != 0)
			{
				std::println(stderr, "save(): Failed to create child shell to call gnuplot, or gnuplot call failed.");
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
				std::println(stderr, "_genLine_yrange(): minimum y value ({}) is equal to or larger than maximum y value ({})", _ymin, _ymax);
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
				std::println(stderr, "_genLine_xrange(): minimum x value ({}) is equal to or larger than maximum x value ({})", _xmin, _xmax);
				exit(EXIT_FAILURE);
			}
		
			std::ostringstream ss{};
			ss << "set yrange [" << _xmin << ":" << _xmax << "]";
			return ss.str();
		}

		///@}
	};

	template <Arithmetic T>
	const std::string Plotter<T>::DATA_DIR{"data"};

	template <Arithmetic T>
	const std::string Plotter<T>::SCRIPT_DIR{"scripts"};
} // namespace chmath
