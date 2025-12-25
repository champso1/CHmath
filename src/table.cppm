module;
#include <cstdlib>
export module chmath:table;
import std;
using uint = unsigned;

export namespace chmath
{
	template <typename T>
	concept CFormattable = std::formattable<T, char>;

	class FormattableBase
	{
	protected:
	    FormattableBase() = default;
	public:
		virtual ~FormattableBase() = default;
		virtual std::string format(uint w) const = 0;
	};

	template <CFormattable T>
	class Formattable final : public FormattableBase
	{
	public:
		using value_type = T;
		
	private:
		value_type _val;

	public:
		Formattable() = delete;
		explicit Formattable(value_type v) : _val(v) {}
		~Formattable() = default;
		
		value_type& get() { return _val; }
		value_type const& get() const { return _val; }

		std::string format(uint w) const override
		{
			return std::vformat("{0:^{1}}", std::make_format_args(_val, w));
		}
	};

	// we need to specialize for floating point types
	// in order to set a precision
	template <CFormattable TFloatingPoint>
	requires (std::is_floating_point_v<TFloatingPoint>)
	class Formattable<TFloatingPoint> : public FormattableBase
	{
	public:
		using value_type = TFloatingPoint;

	private:
		value_type _val;

	public:
		Formattable() = delete;
		explicit Formattable(value_type v) : _val(v) {}
		~Formattable() = default;
		
		value_type& get() { return _val; }
		value_type const& get() const { return _val; }

		std::string format(uint w) const override
		{
			uint prec = w/2;
			return std::vformat("{0:^{1}.{2}}", std::make_format_args(_val, w, prec));
		}
	};

	using TableItem = std::unique_ptr<FormattableBase>;
	
	template <CFormattable T, typename... TFormattableArgs>
	TableItem make_table_item(T v, TFormattableArgs&& ...args)
	{
		return std::make_unique<Formattable<T>>(v, std::forward<TFormattableArgs>(args)...);
	}

	class Table final
	{
	public:
		using ref_type = std::reference_wrapper<std::ostream>;
		using col_type = std::vector<std::vector<TableItem>>;

	private:
		static constexpr uint _DEFAULT_WIDTH = 5;
		
	    std::vector<std::string_view> _column_names;
		ref_type _stream;
		col_type _cols;

		bool _w_spec{false};
		uint _w{_DEFAULT_WIDTH};

	public:
		Table(std::vector<std::string_view> const& column_names, std::ostream& stream=std::cout)
			: _column_names{column_names}, _stream(stream) {}

		void set_width(uint w)
		{
			_w = w;
			_w_spec = true;
		}
		
		template <CFormattable T>
		void add_column(std::vector<T> const& col)
		{
			std::vector<TableItem> items{};
			for (T const t : col)
			{
				items.emplace_back(make_table_item(t));
			}
			_cols.emplace_back(std::move(items));
		}

		void write()
		{
			if (_cols.size() != _column_names.size())
			{
				std::println("[ERROR] write_table(): Number of added columns {} doesn't match size of provided column names {}.",
					_cols.size(), _column_names.size());
				exit(EXIT_FAILURE);
			}
			
			uint w = 0;
			if (_w_spec)
				w = _w;
			else
			{
				for (std::string_view name : _column_names)
				{
					if (name.size() > w)
						w = name.size();
				}
			}

			std::string equals = std::format("|{}|\n", std::string((w+3)*_cols.size()-1, '='));
			std::string dashes = std::format("|{}|\n", std::string((w+3)*_cols.size()-1, '-'));

			// header
			_stream.get() << equals;
			_stream.get() << '|';
			for (std::string_view name : _column_names)
				_stream.get() << ' ' << std::vformat("{0:^{1}}", std::make_format_args(name, w)) << " |";
			_stream.get() << '\n';
			_stream.get() << equals;
			// end header
			
			auto write_datapoint = [&](TableItem& item) -> void {
				_stream.get() << ' ' << item->format(w) << " |" ;
			};

			// all data points
			for (uint i=0; i<_cols.at(0).size()-1; ++i)
			{
				_stream.get() << '|';
				for (uint j=0; j<_cols.size(); ++j)
					write_datapoint(_cols[j][i]);
				_stream.get() << '\n';
				_stream.get() << dashes;
			}
			_stream.get() << '|';
			for (uint j=0; j<_cols.size(); ++j)
				write_datapoint(_cols[j][_cols.at(0).size()-1]);
			_stream.get() << '\n';
			_stream.get() << equals;
			// end all data points
		}
    };
}


