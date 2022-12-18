
#pragma once

#include "type_map.h"
#include "member_extractor.h"
#include <iostream>
#include <string>

namespace soft_render
{
	namespace details
	{
		template<typename T>
		concept configarable = requires
		{
			typename T::config;
		};

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
					os << extract_name(typeid(obj_t).name()) << " :\n";
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

					os << extract_name(typeid(field_name).name()) << " : ";
					os << v;
					os << "\n";
				}
			}
		};

		struct read
		{
			template <typename field_name, typename obj_t>
			static void apply(obj_t& obj, std::istream& is, int indent_depth)
			{
				auto& tm = obj.m_configs;
				using field_type = field_name::type;
				if constexpr (configarable<field_type>)
				{
					tm.add_type<field_name>();
					auto& sub_obj = tm.get_ref<field_name>();

					for (int i = 0; i < indent_depth; i++)
					{
						std::string s_tab;
						is >> s_tab;
					}
					std::string node_name;
					is >> node_name;
					auto in_node_name = extract_name(typeid(obj_t).name());
					if (node_name != in_node_name)
					{
						printf(" wrong node %s vs %s\n", node_name.c_str(), in_node_name.c_str());
						return;
					}

					{
						std::string semicolon,begin_brace;
						is >> semicolon >> begin_brace;
					}

					for_each_type< extract_member_type_list_t<field_type::config> >:: template apply<read>(sub_obj, is, indent_depth + 1);

					std::string end_brace;
					is >> end_brace;
				}
				else
				{
					tm.add_type<field_name>();
					auto& v = tm.get_ref<field_name>();

					std::string node_name;
					is >> node_name;

					auto in_node_name = extract_name(typeid(field_name).name());
					if (node_name != in_node_name)
					{
						printf(" wrong node %s vs %s\n", node_name.c_str(), in_node_name.c_str());
						return;
					}

					{
						std::string semicolon;
						is >> semicolon;
					}

					is >> v;
				}
			}
		};

	}



	struct serilizer
	{

		template<typename obj_t>
		static void write(obj_t& obj, std::ostream& os)
		{
			for_each_type< extract_member_type_list_t<obj_t::config> >:: template apply<details::write>(obj, os, 0);
		}


		template<typename obj_t>
		static void  read(obj_t& obj, std::istream& is)
		{
			for_each_type< extract_member_type_list_t<obj_t::config> >:: template apply<details::read>(obj, is, 0);
		}
	};

}

