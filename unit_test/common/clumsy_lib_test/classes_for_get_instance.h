
#pragma once

#include <string>

namespace get_instance_test
{
	enum class data_type
	{
		A,
		B,
		Count
	};

	class my_interface
	{
	public:
		virtual std::string bark() = 0;
	};

}

