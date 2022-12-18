
#pragma once

#include "type_map.h"
#include "member_extractor.h"
#include <iostream>
#include <string>

namespace soft_render
{

	template<typename T>
	concept configarable = requires
	{
		typename T::config;
	};


	struct serilizer
	{

		static std::string extract_name(std::string s)
		{
			//return s;
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

		struct write
		{
			template <typename field_name, typename obj_t>
			static void apply(obj_t& obj, std::ostream& os, int indent_depth)
			{
				auto& tm = obj.m_configs;
				using field_type = field_name::type;
				if constexpr (configarable<field_type>)
				{
					auto& sub_obj = tm.get_ref<field_name>();

					for (int i = 0; i < indent_depth; i++)
					{
						os << "\t";
					}
					os << extract_name(typeid(obj_t).name()) << ":\n";
					os << "{\n";

					for_each_type< extract_member_type_list_t<field_type::config> >:: template apply<write>(sub_obj, os, indent_depth + 1);

					for (int i = 0; i < indent_depth; i++)
					{
						os << "\t";
					}
					os << "}\n";
				}
				else 
				{
					const auto& v = tm.get_ref<field_name>();

					for (int i = 0; i < indent_depth; i++)
					{
						os << "\t";
					}

					os << extract_name(typeid(field_name).name()) << ": ";
					os << v;
					os << "\n";
				}
			}
		};

		template<typename obj_t>
		static void write_type_map(obj_t& obj, std::ostream& os)
		{
			for_each_type< extract_member_type_list_t<obj_t::config> >:: template apply<write>(obj, os, 0);
		}
	};

}

