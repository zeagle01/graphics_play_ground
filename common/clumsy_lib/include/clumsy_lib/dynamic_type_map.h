
#pragma once

#include <map>
#include <vector>
#include <string>
#include <memory>

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
			std::string key = typeid(Var_Name).name();
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

}