
#pragma once

#include <memory>
#include <vector>
#include <string>
#include <map>
#include <set>
#include "type_list.h"
#include "class_reflection.h"

namespace clumsy_lib
{

	//////////// type_map
	template<typename Base_Type = void>
	class Type_Map 
	{
	private:
		using type_map_t = std::map<std::string, std::shared_ptr<Base_Type>>;
		type_map_t type_map;

		struct Add_Type
		{
			template <typename T>
			static void apply(auto& type_map)
			{
				auto key = typeid(T).name();
				if (!type_map.count(key))
				{
					auto obj = std::make_shared<T>();
					type_map[key] = std::static_pointer_cast<Base_Type>(obj);
				}
			}
		};



	public:
		template<typename Sub_Type>
		void add_type(std::shared_ptr<Sub_Type> a)
		{
			//TODO: sub class check;
			{
				auto key = typeid(Sub_Type).name();
				if (!type_map.count(key))
				{
					type_map[key] = std::static_pointer_cast<Base_Type>(a);
				}
			}
		}

		template<typename Sub_Type>
		void add_type()
		{
			Add_Type::template apply<Sub_Type>(type_map);
		}

		template<typename Sub_Type>
		void remove_type()
		{
			auto key = typeid(Sub_Type).name();
			if (type_map.count(key))
			{
				type_map.erase(key);
			}
		}

		template<typename Sub_Type>
		std::shared_ptr<Sub_Type> get_type()
		{
			auto key = typeid(Sub_Type).name();
			if (!type_map.count(key))
			{
				add_type<Sub_Type>();
			}
			return std::static_pointer_cast<Sub_Type>(type_map[key]);
		}


		template<typename Type_Group>
		void add_types()
		{
			using Types = clumsy_lib::extract_member_type_list_t<Type_Group>;
			for_each_type<Types, Add_Type>(type_map);
		}

		/////////////////// for loop
		auto begin() { return type_map.begin(); } 
		auto end() { return type_map.end(); }
	};
}