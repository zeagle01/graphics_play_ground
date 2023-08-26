

#pragma once

#include "static_loop.h"
#include "type_list.h"

#include <typeindex>
#include <unordered_map>
#include <set>

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


	using adj_list_t = std::unordered_map<std::type_index, std::set<std::type_index>>;
	using change_status_t = std::unordered_map<std::type_index, bool>;

	class Dependent_Graph
	{
	public:

		template<typename list, template<typename> typename get_deps = default_get_deps>
		static adj_list_t build()
		{
			adj_list_t ret;

			For_Each_Type<list>::apply<Collect_Deps<get_deps>>(ret);
			return ret;
		}

		static adj_list_t merge(adj_list_t dep0, adj_list_t dep1)
		{
			adj_list_t ret = dep0;
			for (const auto& var : dep1)
			{
				auto up = var.first;
				for (const auto& down : dep1[up])
				{
					if (ret.contains(up))
					{
						ret[up].insert(down);
					}
					else
					{
						ret[up] = std::set<std::type_index>{ down };
					}
				}

				if (!ret.contains(up))
				{
					ret[up] = std::set<std::type_index>{};
				}

			}
			return ret;
		}

	private:

		template< template<typename> typename get_deps >
		struct Collect_Deps
		{
			template<typename curr_t> 
			struct Add_Down_Stream
			{
				template<typename upstream_t>
				static void apply(adj_list_t& m_down_streams)
				{
					if (m_down_streams.contains(std::type_index(typeid(upstream_t))))
					{
						m_down_streams[std::type_index(typeid(upstream_t))].insert(std::type_index(typeid(curr_t)));
					}
					else
					{
						m_down_streams[std::type_index(typeid(upstream_t))] = std::set{ std::type_index(typeid(curr_t)) };
					}

				}
			};

			template<typename T>
			static void apply(adj_list_t& m_down_streams)
			{
				using deps = get_deps<T>::type;

				For_Each_Type<deps>::apply<Add_Down_Stream<T>>(m_down_streams);
				if (!m_down_streams.contains(std::type_index(typeid(T))))
				{
					m_down_streams[std::type_index(typeid(T))] = std::set<std::type_index>{};
				}
			}

		};

	};


	template<typename list >
	class Down_Stream_Datas
	{
	public:
		Down_Stream_Datas(const change_status_t& is_changed) :m_is_changed(is_changed) {}

		template<typename name> bool is_changed() const
		{
			static_assert(Type_In_List<name, list>, "type should be in list!");
			return m_is_changed.at(std::type_index(typeid(name)));
		}
	private:
		const change_status_t& m_is_changed;

	};

	template<typename list >
	class Dependent_Propagator
	{
	public:
		void set_dependent_graph(const adj_list_t& deps)
		{
			m_down_streams = deps;
			for (const auto& it : m_down_streams)
			{
				const auto& up = it.first;
				m_is_changed[up] = false;
			}
		}

		template<typename name> void touch()
		{
			static_assert(Type_In_List<name, list>, "type should be in list!");
			touch_imp(std::type_index(typeid(name)));
		}

		template<typename name> bool is_changed() const
		{
			static_assert(Type_In_List<name, list>, "type should be in list!");
			return m_is_changed.at(std::type_index(typeid(name)));
		}

		const change_status_t& get_all_change_status() const
		{
			return m_is_changed;
		}

	private:
		change_status_t m_is_changed;
		adj_list_t m_down_streams;
	private:

		void touch_imp(const std::type_index& t)
		{
			if (m_down_streams.contains(t))
			{
				m_is_changed[t] = true;
				for (const auto& d : m_down_streams.at(t))
				{
					touch_imp(d);
				}
			}
		}

	};



}