
#pragma once

#include <map>
#include <string>
#include <memory> 
#include <vector>
#include <type_traits>

#define ADD_MEMBER_POINTER(name,type,...) struct name: type_map::variable<type,__VA_ARGS__> {}; name* name##_var;
#define ADD_RELATION_PAIR_RECORD(name,...) struct name: std::type_identity<soft_render::type_list<__VA_ARGS__>> {}; name* name##_var;
#define ADD_RELATION_RECORD_KEY_FIRST( key0, ...) struct key0##key1: std::type_identity<soft_render::type_list<key0,__VA_ARGS__>> {}; key0* key0##_var;
#define ADD_RELATION_RECORD_KEY_FIRST_TWO( key0, key1,...) struct key0##key1: std::type_identity<soft_render::type_list<key0,key1,__VA_ARGS__>> {}; key0##key1* key0##key1##_var;

namespace soft_render
{

	class type_map
	{
	public:

		template<typename variable_type, variable_type ... values>
		struct variable
		{
			using type = variable_type;

			constexpr  static variable_type get_default_value()
			{
				if constexpr (sizeof...(values) == 0)
				{
					return {};
				}
				else if constexpr (sizeof...(values) == 1)
				{
					return (values, ...);
				}
				else
				{
					static_assert(false, "shouldn't be more default values size greater than one !");
					return {};
				}
			}
		};


		template<typename Var>
		auto& get_ref()
		{
			auto key = typeid(Var).name();
			return *(std::static_pointer_cast<Var::type>(m_datas[key]));
		}

		template<typename Var,typename ...P>
		void add_type(P&&... p)
		{
			auto key = typeid(Var).name();

			std::shared_ptr<Var::type> obj;
			if constexpr (std::is_aggregate_v<Var::type>)
			{
				obj = std::make_shared<Var::type>();
				typename Var::type& value = *obj;
				value = typename Var::type { std::forward<P>(p)... };
			}
			else
			{
				obj = std::make_shared<Var::type>(std::forward<P>(p)...);
			}

			m_datas[key] = obj;
		}

	private:
		std::map<std::string, std::shared_ptr<void>> m_datas;

	};

	class type_map_view
	{
	public:
		template<typename Var>
		auto& get_ref()
		{
			auto key = typeid(Var).name();
			return *(static_cast<Var*>(m_datas[key]));
		}

		template<typename Var >
		void add_type(Var* obj)
		{
			auto key = typeid(Var).name();

			m_datas[key] = obj;
		}

	private:
		std::map<std::string, void*> m_datas;
	};
}

