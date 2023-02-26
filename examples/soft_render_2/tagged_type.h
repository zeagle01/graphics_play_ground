
#pragma once

namespace soft_render
{

	template<int N>
	struct Literal_String
	{
		constexpr Literal_String(const char(&str)[N])
		{
			std::copy_n(str, N, data);
		};

		char data[N];
	};


	template<Literal_String s>
	struct Tagged_Type
	{ }; 

}