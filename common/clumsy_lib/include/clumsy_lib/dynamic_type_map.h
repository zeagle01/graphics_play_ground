
#pragma once

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <typeinfo>

#include <typeindex>
#include <unordered_map>
#include <map>
#include <string>

#include "clumsy_lib/type_getter.h"

namespace clumsy_lib
{
	template<template<typename> typename Get_Type = Get_Its_Own_Type>
	class Dynamic_Type_Map
	{
	public:

		template<typename Var_Name>
		void add_or_replace()
		{
			std::string key = typeid(Var_Name).name();
			m_datas[key] = std::make_shared<typename Get_Type<Var_Name>::type>();
		}

		template<typename Var_Name>
		void erase()
		{
			std::string key = typeid(Var_Name).name();
			m_datas.erase(key);
		}


		template<typename Var_Name>
		auto& get_ref()
		{
			using tt = decltype(typeid(Var_Name));
			tt v_info = typeid(Var_Name);
			std::string key = v_info.name();
			return *std::static_pointer_cast<typename Get_Type<Var_Name>::type>(m_datas.at(key));

		}

		template<typename Var_Name>
		const auto& get_ref() const
		{
			std::string key = typeid(Var_Name).name();
			return *std::static_pointer_cast<typename Get_Type<Var_Name>::type>(m_datas.at(key));
		}


	private:
		std::map<std::string, std::shared_ptr<void>> m_datas;

	};

	// previous one not works in module (don't know why,maybe it's compiler issue)
	// have to define a new one here
	class dynamic_type_map
	{
	public:

		template<typename Var_Name>
		void add_or_replace()
		{
			auto key = get_key<Var_Name>();
			m_datas[key] = std::make_shared<typename Var_Name::type>();
		}

		template<typename Var_Name>
		void erase()
		{
			auto key = get_key<Var_Name>();
			m_datas.erase(key);
		}


		template<typename Var_Name>
		auto& get_ref()
		{
			auto key = get_key<Var_Name>();
			return *std::static_pointer_cast<typename Var_Name::type>(m_datas.at(key));

		}

		template<typename Var_Name>
		const auto& get_ref() const
		{
			auto key = get_key<Var_Name>();
			return *std::static_pointer_cast<typename Var_Name::type>(m_datas.at(key));
		}
	private:
		using key_t = std::type_index;

		template<typename Var_Name> key_t get_key()
		{
			return std::type_index(typeid(Var_Name));
		}


	private:
		std::unordered_map<key_t, std::shared_ptr<void>> m_datas;

	};

}