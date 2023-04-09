
#include "gmock/gmock.h"
#include <string>
#include "clumsy_lib/morphysm.h"

using namespace testing;
using namespace clumsy_lib;

namespace morphysm_test
{
	struct Base
	{
		virtual std::string func() { return "base"; };
	};

	struct A : Base
	{
		std::string func()
		{
			return "A";
		}
	};

	struct B :Base
	{
		std::string func()
		{
			return "B";
		}
	};



	TEST(Morphysm_Test, register_and_use)
	{
		Morphysm<Base> obj;
		obj.register_sub_type<A>();
		obj.register_sub_type<B>();

		{
			obj.shift<A>();
			std::string act = obj->func();
			std::string exp = "A";
			EXPECT_THAT(act, Eq(exp));
		}

		{
			obj.shift<B>();
			std::string act = obj->func();
			std::string exp = "B";
			EXPECT_THAT(act, Eq(exp));
		}
	}

}

