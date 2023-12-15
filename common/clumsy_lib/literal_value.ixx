module;

#include <array>
#include <string_view>


export module clumsy_lib : literal_value;


namespace clumsy_lib
{

	export 
	template<typename T, int = 0 >
	struct literal
	{
		constexpr literal(T v) : data(v) {}
		T data;
		constexpr T get() const { return data; }
	};

	export 
	template<int N>
	struct literal<char,N>
	{
		constexpr literal( const char (&v)[N] )  
		{
			std::copy_n(v, N, data.data()); 
		}
		std::array<char,N> data;

		constexpr std::string_view get() const 
		{ 
			return std::string_view(std::begin(data), std::end(data)); 
		}
	};

	export 
	template<literal v>
	struct literal_t 
	{
		static constexpr auto value = v.get();
	};

}


