
#pragma once

#include "clumsy_lib/static_loop.h"

#include <memory>

#define REGISTER_INSTANCE_FACTORY( base_t,derived_t,enum_v)						\
namespace clumsy_lib															\
{																				\
	template<>																	\
	template<> std::unique_ptr<base_t> creator<base_t>::apply<enum_v>()			\
	{																			\
		return std::make_unique<derived_t>();									\
	}																			\
																				\
	template<>																	\
	template<> constexpr bool creator<base_t>::check<enum_v>() { return true; }					\
}																				\

namespace clumsy_lib
{


	template<typename base_t> struct creator
	{
		template<auto enum_v>
		static std::unique_ptr<base_t> apply();

		template<auto enum_v>
		static constexpr bool check() { return false; }
	};

	struct get_instance
	{

		template<typename base_t, typename enum_t>
		static std::unique_ptr<base_t> apply(enum_t v)
		{
			std::unique_ptr<base_t> ret;
			For_Each_In_Range<static_cast<int>(enum_t::Count)>::apply<careate_if_match<base_t>>(ret, v);
			return ret;
		}
	private:
		template<typename base_t>
		struct careate_if_match
		{
			template<int I, typename enum_t>
			static void apply(std::unique_ptr<base_t>& ret, enum_t v)
			{
				if (static_cast<int>(v) == I)
				{
					constexpr enum_t enum_v = static_cast<enum_t>(I);
					if constexpr (creator<base_t>::template check<enum_v>())
					{
						ret = creator<base_t>::template apply<enum_v>();
					}
				}
			}
		};

	};



}