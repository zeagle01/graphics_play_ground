
#pragma once

#include <memory>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <set>
#include "type_list.h"
#include "static_loop.h"
#include "class_reflection.h"

namespace clumsy_lib
{

	template<typename K,typename V> 
	struct Order_Helper
	{
		struct K_V 
		{
			std::string type_name;
			V obj;
		};

		using list_t = std::list<K_V>;

		std::map<K, typename list_t::iterator> iterators;
		list_t linked_list;

		void push_back(const K& key, V value)
		{
			linked_list.push_back({ std::string(typeid(key).name()),value });
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

	struct New_Std_Shared_Ptr
	{
		template<typename Sub, typename Base>
		static std::shared_ptr<Sub> apply()
		{
			return std::make_shared<Sub>();
		}
	};

	struct New_Type_To_Variant
	{
		template<typename Sub, typename Base>
		static std::shared_ptr<Base> apply()
		{
			return std::make_shared<Base>(Sub{});
		}

	};

	//////////// type_map
	template<typename Base_Type = void, typename New_Type_Fn = New_Std_Shared_Ptr>
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
			static void apply(auto& type_map, auto& order_list, std::shared_ptr<Base_Type> obj)
			{
				auto key = typeid(T).name();
				if (!type_map.count(key))
				{
					type_map[key] = obj;
					order_list.push_back(key, obj);
				}
			}

			template <typename T>
			static void apply(auto& type_map,auto& order_list)
			{
				//apply<T>(type_map, order_list, std::make_shared<T>());
				apply<T>(type_map, order_list, New_Type_Fn::template apply<T, Base_Type>());
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

		template<typename Sub_Type>
		std::shared_ptr<Base_Type> get_as_interface_type()
		{
			auto key = typeid(Sub_Type).name();
			if (!type_map.count(key))
			{
				add_type<Sub_Type>();
			}
			return type_map[key];
		}


		template<typename Type_Group>
		void add_types()
		{
			using Types = clumsy_lib::extract_member_type_list_t<Type_Group>;
			For_Each_Type<Types>::template apply< Add_Type>(type_map, order_list);
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


	template<typename Base_Type = void >
	struct Raw_Ptr_Type_Map 
	{
		using type_map_t = std::map<std::string, Base_Type*>;
		type_map_t type_map;

		template<typename Sub_Type>
		void add_type(Sub_Type* obj)
		{
			auto key = typeid(Sub_Type).name();
			if (!type_map.count(key))
			{
				type_map[key] = static_cast<Base_Type*>(obj);
			}
		}

		template<typename Sub_Type>
		Sub_Type* get_type()
		{
			//TODO check
			auto key = typeid(Sub_Type).name();
			return static_cast<Sub_Type*>(type_map[key]);
		}

//		template<typename Sub_Type>
//		const Sub_Type* get_type() const 
//		{
//			//TODO check
//			auto key = typeid(Sub_Type).name();
//			return static_cast<const Sub_Type*>(type_map[key]);
//		}

	};

}