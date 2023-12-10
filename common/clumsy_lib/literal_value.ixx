module;

#include <array>
#include <string_view>


export module clumsy_lib : literal_value;


namespace clumsy_lib
{
	
	export 
	template<auto v>
	struct literal_value
	{
		static constexpr decltype(v) value = v;

	};

	template<int N>
	struct literal_string_storage
	{
		constexpr literal_string_storage(const char (&data_ref)[N]) { std::copy_n(data_ref, N, data.data()); }
		std::array<char,N> data;
	};

	export 
	template<literal_string_storage str>
	struct literal_string 
	{
		static constexpr std::string_view value = str.data.data();
	};


	export 
	template<typename T, T v >
	struct literal_custom
	{
		static constexpr decltype(v) value = v;
	};


}


