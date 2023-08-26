

#pragma once

#include "static_loop.h"
#include "type_list.h"

#include <typeindex>
#include <unordered_map>

namespace clumsy_lib
{

	template<typename T>
	struct default_get_deps
	{
		template<typename U>
		static auto  choosor()
		{
			if constexpr (requires {typename U::deps; })
			{
				return typename U::deps {};
			}
			else
			{
				return type_list<>{};
			}
		}

		using type = decltype(choosor<T>());
	};

	template<typename list, template<typename> typename get_deps = default_get_deps>
	class Dependent_Propagator
	{
	public:
		Dependent_Propagator()
		{
			build_dep_graph();
			For_Each_Type<list>::apply<Init_Change>(m_is_changed);
		}

		template<typename name> void touch()
		{
			touch_imp(std::type_index(typeid(name)));
		}

		template<typename name> bool is_changed() const
		{
			return m_is_changed.at(std::type_index(typeid(name)));
		}

	private:

		std::unordered_map<std::type_index, bool> m_is_changed;
		using adj_list_t = std::unordered_map<std::type_index, std::vector<std::type_index>>;
		adj_list_t m_down_streams;

	private:

		void touch_imp(const std::type_index& t)
		{
			m_is_changed[t] = true;
			if (m_down_streams.count(t))
			{
				for (const auto& d : m_down_streams.at(t))
				{
					touch_imp(d);
				}
			}
		}

	private:
		void build_dep_graph()
		{
			For_Each_Type<list>::apply<Collect_Deps>(m_down_streams);
		}

		struct Init_Change
		{
			template<typename T>
			static void apply(std::unordered_map<std::type_index, bool>& m_is_changed)
			{
				m_is_changed[std::type_index(typeid(T))] = false;
			}

		};

		struct Collect_Deps
		{
			template<typename curr_t> 
			struct Add_Down_Stream
			{
				template<typename upstream_t>
				static void apply(adj_list_t& m_down_streams)
				{
					if (m_down_streams.count(std::type_index(typeid(upstream_t))))
					{
						m_down_streams[std::type_index(typeid(upstream_t))].push_back(std::type_index(typeid(curr_t)));
					}
					else
					{
						m_down_streams[std::type_index(typeid(upstream_t))] = std::vector{ std::type_index(typeid(curr_t)) };
					}

				}
			};

			template<typename T>
			static void apply(adj_list_t& m_down_streams)
			{
				using deps = get_deps<T>::type;

				For_Each_Type<deps>::apply<Add_Down_Stream<T>>(m_down_streams);
			}

		};

	};


}