#include "chmath/chmath.hpp"

#include <cstdlib>
#include <sstream>

namespace chmath
{

    Plotter& Plotter::setOutputFile(
		std::string const& file_name,
		uint output_file_type)
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

			// TODO: make this customizable?
			// allow to have more than one?
			_file_name = "script.gplt";
			_script.open(_file_name);
			if (!_script)
			{
				std::cerr << "setOutputFile(): Failed to open file '"
						  << _file_name << "'.\n";

				exit(EXIT_FAILURE);
			}

			std::ostringstream output_line_ss{};
			output_line_ss << "set terminal ";
			switch (output_file_type)
			{
				case PNG: output_line_ss << "pngcairo "; break;
				case PDF: output_line_ss << "pdfcairo "; break;
				default: exit(EXIT_FAILURE);
			}
			output_line_ss << "enhanced notransparent";

			std::string output_line{output_line_ss.str()};

			_script << output_line << '\n';
			_script << "set output '" << file_name << "'" << '\n';
		}
		
		return *this;
	}

	std::string Plotter::_genLine_yrange()
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

	std::string Plotter::_genLine_xrange()
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
	
} // namespace chmath
