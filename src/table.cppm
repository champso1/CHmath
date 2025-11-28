module;
#include <print>
#include <string>
#include <unordered_map>
#include <typeindex>
#include <functional>
#include <any>

#include <cxxabi.h>

export module chmath:table;

std::string __demangleTypeName(char const* name)
{
	int status = -4;

    std::unique_ptr<char, void(*)(void*)> res {
        abi::__cxa_demangle(name, NULL, NULL, &status),
        std::free
    };

    return (status==0) ? res.get() : name ;
}

export namespace chmath
{	
	class Table final
	{
	private:
		using function_type = std::function<void(std::any const&)>;

		template <typename T, typename F>
		inline static
		std::pair<
			std::type_index,
			function_type>
		__toVisitor(F const& f)
		{
			return
			{
				std::type_index(typeid(T)),
				[g = f](std::any const& a)
				{
					if constexpr (std::is_void_v<T>)
						g();
					else
						g(std::any_cast<T const&>(a));
				}
			};
		}
		
		static const
		std::unordered_map<
			std::type_index,
		    function_type>
		_registered_types;

	public:
		template <typename T, typename F>
		static void registerType(F const& f);
		
	public:
		Table() = default;

		void process(std::any const& a);
	};

	const
	std::unordered_map<std::type_index, Table::function_type>
	Table::_registered_types
    {
		__toVisitor<void>([] -> void { std::print("{{}}"); }),
	    __toVisitor<int>([](int x) -> void { std::print("{}", x); }),
		__toVisitor<unsigned>([](unsigned x) -> void { std::print("{}", x); }),
		__toVisitor<long>([](long x) -> void { std::print("{}", x); }),
		__toVisitor<long long>([](long long x) -> void { std::print("{}", x); }),
		__toVisitor<short>([](short x) -> void { std::print("{}", x); }),
		__toVisitor<unsigned char>([](unsigned char x) -> void { std::print("{}", x); }),
		__toVisitor<char>([](char x) -> void { std::print("{}", x); }),
		__toVisitor<float>([](float x) -> void { std::print("{}", x); }),
		__toVisitor<double>([](double x) -> void { std::print("{}", x); }),
		__toVisitor<std::string>([](std::string s) -> void { std::print("{}", s); }),
		__toVisitor<char const*>([](char const* s) -> void { std::print("{}", s); })
	};


	void Table::process(std::any const& a)
	{
		if (const auto it = _registered_types.find(std::type_index(a.type()));
			it != _registered_types.cend())
		{
			it->second(a);
		}
		else
		{
			std::println("[ERROR] Table::process(): Unregistered type '{}'", __demangleTypeName(a.type().name()));
		}
	}
	
}



