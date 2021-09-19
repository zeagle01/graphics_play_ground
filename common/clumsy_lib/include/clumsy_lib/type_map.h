
#pragma once

#include <memory>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <set>
#include "type_list.h"
#include "class_reflection.h"

namespace clumsy_lib
{

	template<typename K,typename V> 
	struct Order_Helper
	{
		std::map<K, typename std::list<V>::iterator> iterators;
		std::list<V> linked_list;

		void push_back(const K& key, V value)
		{
			linked_list.push_back(value);
			auto back_it = linked_list.end();
			iterators[key] = --back_it;
		}

		void erase(const K&  key)
		{
			auto it = iterators[key];
			linked_list.erase(it);
			iterators.erase(key);
		}

	};

	//////////// type_map
	template<typename Base_Type = void>
	class Type_Map 
	{
	private:
		using type_map_t = std::map<std::string, std::shared_ptr<Base_Type>>;
		type_map_t type_map;

		Order_Helper<std::string, std::shared_ptr<Base_Type>> order_list;

		struct Add_Type
		{

			template <typename T>
			static void apply(auto& type_map, auto& order_list, std::shared_ptr<T> obj)
			{
				auto key = typeid(T).name();
				if (!type_map.count(key))
				{
					type_map[key] = std::static_pointer_cast<Base_Type>(obj);
					order_list.push_back(key, std::static_pointer_cast<Base_Type>(obj));
				}
			}

			template <typename T>
			static void apply(auto& type_map,auto& order_list)
			{
				apply<T>(type_map, order_list, std::make_shared<T>());
			}

		};



	public:
		template<typename Sub_Type>
		void add_type(std::shared_ptr<Sub_Type> obj)
		{
			Add_Type::template apply<Sub_Type>(type_map, order_list, obj);
		}

		template<typename Sub_Type>
		void add_type()
		{
			Add_Type::template apply<Sub_Type>(type_map,order_list);
		}

		template<typename Sub_Type>
		void remove_type()
		{
			auto key = typeid(Sub_Type).name();
			if (type_map.count(key))
			{
				type_map.erase(key);
				order_list.erase(key);
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
			for_each_type<Types, Add_Type>(type_map,order_list);
		}


		//empty
		bool empty()
		{
			return type_map.empty();
		}

		/////////////////// for loop
		auto begin() { return order_list.linked_list.begin(); } 
		auto end() { return order_list.linked_list.end(); }
	};
}