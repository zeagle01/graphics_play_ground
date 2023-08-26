
#include "classes_for_get_instance.h"

#include "clumsy_lib/get_instance.h"

namespace get_instance_test
{

	class imp_A : public my_interface
	{
		virtual std::string bark()
		{
			return "A";
		}
	};

	class imp_B : public my_interface
	{
		virtual std::string bark()
		{
			return "B";
		}

	};

}
using namespace get_instance_test;

REGISTER_INSTANCE_FACTORY(my_interface,imp_A,data_type::A)
REGISTER_INSTANCE_FACTORY(my_interface,imp_B,data_type::B)
