
#include "gmock/gmock.h"

#include "clumsy_lib/enum_tagged_morphysm.h"


import clumsy_lib;



using namespace testing;
using namespace clumsy_lib;


namespace enum_tagged_morphysm_test
{

	enum class implementation
	{
		A,
		B,
	};

	class my_base
	{
	public:
		virtual std::string bark() = 0;

		CLUMSY_ENUM_TAGGED_MORPHYSM(my_base, implementation);
	};


	CLUMSY_ENUM_TAGGED_MORPHYSM_IMP(my_base, implementation::A,
		public:
			std::string bark() override { return "A"; }
	)

	CLUMSY_ENUM_TAGGED_MORPHYSM_IMP(my_base, implementation::B,
		public:
			std::string bark() override { return "B"; }
	)


	using morphysm = clumsy_lib::enum_tagged_morphysm<my_base, implementation>;

	TEST(enum_tagged_morphysm_test, default_implementation)
	{
		morphysm m;
		auto act = m->bark();
		EXPECT_THAT(act, Eq("A"));
	}

	TEST(enum_tagged_morphysm_test, switch_implementation)
	{
		morphysm m;
		m.set(implementation::B);
		auto act = m->bark();
		EXPECT_THAT(act, Eq("B"));
	}

}
