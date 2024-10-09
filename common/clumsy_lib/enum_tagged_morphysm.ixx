
module;

#include "magic_enum/magic_enum.hpp"
#include "clumsy_lib/static_loop.h"
#include <memory>
#include <map>

export module clumsy_lib: enum_tagged_morphysm;

namespace clumsy_lib
{

	export 
	template<typename base_type, typename enum_type>
	class enum_tagged_morphysm
	{
	public:
		enum_tagged_morphysm()
		{
			constexpr auto enums = magic_enum::enum_values<enum_type>();
			For_Each_In_Range<int(enums.size())>::apply<add_imp>(m_implementations);

		}
		void set(enum_type v)
		{
			m_current_type = v;
		}

		enum_type get() const
		{
			return m_current_type;
		}

		std::shared_ptr<base_type> operator->()
		{
			return m_implementations[m_current_type];
		}

	private:

		struct add_imp
		{
			template<int I>
			static void apply(std::map<enum_type, std::shared_ptr<base_type>>& implementations)
			{
				constexpr auto v = static_cast<enum_type>(I);
				implementations[v] = base_type::template create<v>();
			}

		};

		enum_type m_current_type{};
		std::map<enum_type, std::shared_ptr<base_type>> m_implementations;

	};

}
