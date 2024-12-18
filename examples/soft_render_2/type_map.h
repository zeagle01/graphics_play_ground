
#pragma once

#include <map>
#include <string>
#include <memory> 
#include <vector>
#include <type_traits>

#include "type_list.h"
#include "member_extractor.h"
#include "Tagged_Type.h"

#define ADD_MEMBER_POINTER(name,type,...) struct name: type_map::variable<type,__VA_ARGS__> {}; name* name##_var;
#define ADD_MEMBER_STR_POINTER(name,str) struct name: type_map::str_variable<str> {}; name* name##_var;
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
					static_assert(std::_Always_false, "shouldn't be more default values size greater than one !");
					return {};
				}
			}
		};

		template<Literal_String s>
		struct str_variable
		{
			using type = std::string;
			constexpr  static std::string get_default_value()
			{
				return std::string(std::begin(s.data), std::end(s.data));
			}
		};


		template<typename Var>
		auto& get_ref()
		{
			auto key = typeid(Var).name();
			return *(std::static_pointer_cast<typename Var::type>(m_datas[key]));
		}

		template<typename Var,typename ...P>
		void add_type(P&&... p)
		{
			auto key = typeid(Var).name();

			std::shared_ptr<typename Var::type> obj;
			if constexpr (std::is_aggregate_v<typename Var::type>)
			{
				obj = std::make_shared<typename Var::type>();
				typename Var::type& value = *obj;
				value = typename Var::type { std::forward<P>(p)... };
			}
			else
			{
				obj = std::make_shared<typename Var::type>(std::forward<P>(p)...);
			}

			m_datas[key] = obj;//override
		}


	template<typename config>
	static void fill_types(type_map& tm)
	{
		for_each_type< extract_member_type_list_t<config>>::template apply<add_type_by_default_value>(tm);
	}

	private:
		struct add_type_by_default_value
		{
			template<typename T> 
			static void apply(type_map& tm )
			{
				tm.add_type<T>();
				tm.get_ref<T>() = T::get_default_value();
			}

		};

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


	/////////////////////For_Each_Type///////////////////////
	namespace details
	{
		template<typename tl>
		struct for_each_obj_with_type;

		//public:
		template< template <typename ...> typename tl, typename ...TP>
		struct for_each_obj_with_type<tl<TP...>>
		{
			// template<typename T> struct F{ apply (p) {} }
			template< typename F, typename ...P>
			static void apply(type_map& tm, P&&...p)
			{
				(F::template apply<TP>(tm.get_ref<TP>(), std::forward<P>(p)...), ...);
			}
		};

	}

	template<typename tl>
	using for_each_obj_with_type = details::for_each_obj_with_type<tl>;


	static std::string extract_name(std::string s)
	{
		auto it = s.find_last_of(':');
		if (it != std::string::npos)
		{
			return s.substr(it + 1, it + s.size());
		}
		else
		{
			return s;
		}
	}
}

