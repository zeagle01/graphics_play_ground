
module;


#include "clumsy_lib/static_loop.h"
#include "clumsy_lib/type_list.h"

#include <unordered_map>
#include <typeindex>
#include <set>

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

		static static_dep_graph merge(const static_dep_graph& dep0, const static_dep_graph& dep1);

	private:

		template< template<typename> typename get_deps >
		struct collect_deps;

		using adj_list_t = std::unordered_map<std::type_index, std::set<std::type_index>>;
		adj_list_t m_data;

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

}

