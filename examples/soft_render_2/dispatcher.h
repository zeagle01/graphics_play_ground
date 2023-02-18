#pragma once

#include <string>
#include <functional>
#include <map>


namespace soft_render
{
	template<typename base_t,typename ret_t>
	class dispatcher
	{
		using key_t = std::string;
		using base_to_sub_cast_fn_t = std::function<ret_t(const base_t*)>;
		std::map<key_t, std::vector<base_to_sub_cast_fn_t>> m_fns;
	public:

		template<typename sub_t>
		void add(std::function<ret_t(const sub_t&)> fn)
		{

			auto base_to_sub_cast_fn = [fn](const base_t* obj)
			{
				return fn(*dynamic_cast<const sub_t*>(obj));
			};

			key_t key = typeid(sub_t).name();

			if (!m_fns.count(key)) 
			{
				m_fns[key] = std::vector<base_to_sub_cast_fn_t>();
			}
			m_fns[key].push_back(base_to_sub_cast_fn);
		}

		void operator()(base_t& obj)
		{
			std::string key = typeid(obj).name();
			if (m_fns.count(key))
			{
				for (int i = 0; i < m_fns[key].size(); i++)
				{
					m_fns[key][i](&obj);
				}
			}
		}


	};

}

