
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


		struct write
		{
			static void indent(std::ostream& os, int indent_depth)
			{
				for (int i = 0; i < indent_depth; i++)
				{
					os << "  ";
				}
			}

			template<typename obj_name>
			static void apply( typename obj_name::type& obj, std::ostream& os, int indent_depth)
			{
				using obj_t = obj_name::type;
				if constexpr (configarable<obj_t>)
				{
					indent(os, indent_depth);

					os << extract_name(typeid(obj_t).name()) << " :\n";

					indent(os, indent_depth);

					os << "{\n";

					for_each_obj_with_type< extract_member_type_list_t<obj_t::config> >:: template apply<write>(obj.m_configs, os, indent_depth + 1);

					indent(os, indent_depth);

					os << "}\n";
				}
				else
				{
					indent(os, indent_depth);

					os << extract_name(typeid(obj_name).name()) << " : ";

					os << obj;

					os << "\n";
				}

			}
		};


		struct read
		{
			template<typename obj_name>
			static bool apply(typename obj_name::type& obj, std::istream& is, int indent_depth)
			{

				using obj_t = obj_name::type;
				if constexpr (configarable<obj_t>)
				{

					std::string node_name,semi_colon;
					is >> node_name>>semi_colon;

					std::string obj_type_name_str = extract_name(typeid(obj_t).name());
					if (obj_type_name_str != node_name)
					{
						printf(" wrong node %s vs %s\n", node_name.c_str(), obj_type_name_str.c_str());
						return false;;
					}

					std::string open_curl;
					is >> open_curl;

					for_each_obj_with_type< extract_member_type_list_t<obj_t::config> >:: template apply<read>(obj.m_configs, is, indent_depth + 1);

					std::string close_curl;
					is >> close_curl;
				}
				else
				{
					std::string node_name,semi_colon;
					is >> node_name>>semi_colon;
					is >> obj;
				}

				return true;
			}

		};

	}



	struct serilizer
	{

		template<typename obj_t>
		static void write(obj_t& obj, std::ostream& os)
		{
			struct root_name { using type = obj_t; };
			details::write::apply<root_name>(obj, os, 0);
		}


		template<typename obj_t>
		static bool  read(obj_t& obj, std::istream& is)
		{
			struct root_name { using type = obj_t; };
			return details::read::apply<root_name>(obj, is, 0);
		}
	};

}

