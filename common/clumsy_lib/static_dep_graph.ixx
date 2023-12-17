
module;


#include "clumsy_lib/static_loop.h"
#include "clumsy_lib/type_list.h"

#include <unordered_map>
#include <typeindex>
#include <set>
#include <functional> 

export module clumsy_lib : static_dep_graph;


namespace clumsy_lib
{

	template<typename T>
	struct default_get_deps_new
	{
		using type = typename T::deps;
	};

	export
	class static_dep_graph
	{
	public:

		template<typename list, template<typename> typename get_deps = default_get_deps_new>
		void build();

		template<typename from, typename to>
		bool has_edge();

		const std::set<std::type_index>& get_outlets(std::type_index t) const;
		bool contains(std::type_index t) const;

		static static_dep_graph merge(const static_dep_graph& dep0, const static_dep_graph& dep1);

	private:

		template< template<typename> typename get_deps >
		struct collect_deps;

		using adj_list_t = std::unordered_map<std::type_index, std::set<std::type_index>>;
		adj_list_t m_data;

	};


	export
	class dynamic_walker
	{
	public:
		dynamic_walker(const static_dep_graph& _graph) :graph(_graph) {}

		template<typename T> 
		void walk(std::function<void(std::type_index)> fn);

		void walk(std::type_index type, std::function<void(std::type_index)> fn);

	private:
		const static_dep_graph& graph;
		std::set<std::type_index> visited;
	};

	export
	class static_walker
	{
	public:
		template<template<typename> typename get_fn, typename list, template<typename> typename get_deps, typename ...P>
		void walk(P&& ...p);

	private:
		std::set<std::type_index> visited;
	};



	template<typename list, template<typename> typename get_deps >
	void static_dep_graph::build()
	{
		For_Each_Type<list>::apply<collect_deps<get_deps>>(m_data);
	}

	static_dep_graph static_dep_graph::merge(const static_dep_graph& dep0, const static_dep_graph& dep1)
	{

		static_dep_graph ret = dep0;
		for (const auto& var : dep1.m_data)
		{
			auto up = var.first;
			for (const auto& down : dep1.m_data.at(up))
			{
				if (ret.m_data.contains(up))
				{
					ret.m_data[up].insert(down);
				}
				else
				{
					ret.m_data[up] = std::set<std::type_index>{ down };
				}
			}

			if (!ret.m_data.contains(up))
			{
				ret.m_data[up] = std::set<std::type_index>{};
			}

		}
		return ret;
	}

	template< template<typename> typename get_deps >
	struct static_dep_graph::collect_deps
	{
		template<typename curr_t>
		struct add_down_stream
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

			For_Each_Type<deps>::apply<add_down_stream<T>>(m_down_streams);
			if (!m_down_streams.contains(std::type_index(typeid(T))))
			{
				m_down_streams[std::type_index(typeid(T))] = std::set<std::type_index>{};
			}
		}

	};

	template<typename from, typename to>
	bool static_dep_graph::has_edge()
	{
		auto k_from = std::type_index(typeid(from));

		auto k_to = std::type_index(typeid(to));

		return m_data[k_from].contains(k_to);
	}

	const std::set<std::type_index>& static_dep_graph::get_outlets(std::type_index t) const
	{
		return m_data.at(t);
	}

	bool static_dep_graph::contains(std::type_index t) const
	{
		return m_data.contains(t);

	}



	void dynamic_walker::walk(std::type_index type, std::function<void(std::type_index)> fn)
	{

		if (graph.contains(type))
		{
			visited.insert(type);

			fn(type);

			for (const auto& d : graph.get_outlets(type))
			{
				walk(d,fn);
			}
		}
	}

}

