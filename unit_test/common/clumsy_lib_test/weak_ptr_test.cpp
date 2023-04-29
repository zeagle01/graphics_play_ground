

#include "clumsy_lib/weak_ptr.h"
#include "gmock/gmock.h"
#include <string>
#include <memory>


using namespace testing;

namespace weak_ptr_test
{
	struct A
	{
		std::string f()
		{
			return "f()";
		}

		std::string g(int a)
		{
			return "g(" + std::to_string(a) + ")";
		}

		std::string g2(int a,int b)
		{
			return "g(" + std::to_string(a) + "," + std::to_string(b) + ")";
		}

		void h(std::string& out)
		{
			out = "h";
		}
	};


	TEST(Weak_Ptr_Test, empty_ptr_method)
	{
		std::shared_ptr<A> data ;
		clumsy_lib::Weak_Ptr wptr(data);

		std::string act = wptr.SAFE_INVOKE(f);

		EXPECT_THAT(act, Eq(""));
	}

	TEST(Weak_Ptr_Test, non_empty_ptr_method)
	{
		auto data = std::make_shared<A>();
		clumsy_lib::Weak_Ptr wptr(data);

		std::string act = wptr.SAFE_INVOKE(f);

		EXPECT_THAT(act, Eq("f()"));
	}

	TEST(Weak_Ptr_Test, invoke_method_with_int_parameter)
	{
		auto data = std::make_shared<A>();
		clumsy_lib::Weak_Ptr wptr(data);

		int interger_parameter = 1;

		std::string act = wptr.SAFE_INVOKE(g,interger_parameter);
		EXPECT_THAT(act, Eq("g(1)"));

		data.reset();
		act = wptr.SAFE_INVOKE(g, interger_parameter);
		EXPECT_THAT(act, Eq(""));
	}

	TEST(Weak_Ptr_Test, invoke_method_with_two_parameter)
	{
		auto data = std::make_shared<A>();
		clumsy_lib::Weak_Ptr wptr(data);

		int interger_parameter = 1;

		std::string act = wptr.SAFE_INVOKE(g2,interger_parameter,2);
		EXPECT_THAT(act, Eq("g(1,2)"));

		data.reset();
		act = wptr.SAFE_INVOKE(g2, interger_parameter, 2);
		EXPECT_THAT(act, Eq(""));
	}

	TEST(Weak_Ptr_Test, invoke_method_with_void_return)
	{
		auto data = std::make_shared<A>();
		clumsy_lib::Weak_Ptr wptr(data);

		int interger_parameter = 1;
		std::string act; 
		wptr.SAFE_INVOKE(h, act);

		EXPECT_THAT(act, Eq("h"));

		data.reset();
		wptr.SAFE_INVOKE(h, act); 
		EXPECT_THAT(act, Eq("h")); //won't change value because obj is empty
	}

}
