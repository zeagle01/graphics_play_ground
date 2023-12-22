
#include "gmock/gmock.h"

#include <vector> 
#include <string>

import clumsy_lib;


using namespace testing;
using namespace clumsy_lib;

namespace dispatchor_test
{


	struct base
	{
		virtual ~base() {};
	};

	struct A :public base
	{

	};

	struct B :public base
	{

	};



	class dispatchor_test :public Test
	{
	protected:
		void SetUp()
		{
			m_program_stack.clear();

			my_dispatchor.connect<A>(
				[&](auto& data)
				{
					m_program_stack.push_back("A");
					return true;
				}
			);
		}
		using program_stack = std::vector<std::string>;
		program_stack m_program_stack;

		dispatchor<base, bool> my_dispatchor;
	};

	TEST_F(dispatchor_test, registered_func_will_be_call)
	{
		A a;
		base& data = a;

		my_dispatchor(data);

		EXPECT_THAT(m_program_stack, Eq(program_stack{ {"A"} }));
	}

	TEST_F(dispatchor_test, not_registered_func_will_not_be_call)
	{
		B b;
		base& data = b;

		my_dispatchor(data);

		EXPECT_THAT(m_program_stack, Eq(program_stack{  }));
	}
}




