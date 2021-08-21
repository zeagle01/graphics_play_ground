
#pragma once

#include <memory>
#include <vector>
#include <string>
#include <map>
#include <set>
#include "clumsy_lib/class_reflection.h"

namespace clumsy_lib
{

	//////////// type_map
	template<typename Base_Type = void>
	class Type_Map 
	{
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
			auto key = typeid(Sub_Type).name();
			if (!type_map.count(key))
			{
				auto obj = std::make_shared<Sub_Type>();
				type_map[key] = std::static_pointer_cast<Base_Type>(obj);
			}
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

		/////////////////// for loop
		auto begin() { return type_map.begin(); } 
		auto end() { return type_map.end(); }
	private:
		std::map<std::string, std::shared_ptr<Base_Type>> type_map;
	};




}