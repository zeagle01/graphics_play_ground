
#include "gmock/gmock.h"
#include <vector>
#include <span>
#include <ranges>


import geometry_lib;

using namespace testing;

namespace bvh_test
{


	class bvh_tester :public Test
	{
	protected:

		void build_with_pointes(const std::vector<float>& points)
		{
			m_bvh.build<float>(points);
		}

	protected:

		static void expect_nodes_equal(const std::vector<geometry::node>& act, const std::vector<geometry::node>& exp)
		{
			auto copy_exp = exp;
			auto copy_act = act;
			std::ranges::sort(copy_exp);
			std::ranges::sort(copy_act);
			EXPECT_THAT(copy_act, Eq(copy_exp));
		}


	protected:
		geometry::bvh m_bvh;
		geometry::bvh_traverser<geometry::AABB1f> m_traverser;
	};




	TEST_F(bvh_tester, one_dimension_tree)
	{

		build_with_pointes({ 0.f,1.f });

		std::vector<geometry::node> traversed_nodes;

		auto get_node = [&](const auto& curr, const auto& l, const auto& r)
			{
				traversed_nodes.push_back(curr);
				return true;

			};

		m_bvh.traverse_bottom_up({ 1,geometry::node_type::leaf }, get_node);

		expect_nodes_equal(traversed_nodes, { {1,geometry::node_type::leaf},{0,geometry::node_type::inner} });

	}

	TEST_F(bvh_tester, traverse_top_down)
	{
		build_with_pointes({ 0.f,1.f });

		std::vector<geometry::node> traversed_nodes;

		auto get_node = [&](const auto& curr, const auto& l, const auto& r)
			{
				traversed_nodes.push_back(curr);
				return true;

			};

		m_bvh.traverse_top_down({ 0,geometry::node_type::inner }, get_node);

		expect_nodes_equal(traversed_nodes, { {0,geometry::node_type::inner},{0,geometry::node_type::leaf},{1,geometry::node_type::leaf} });
	}


	TEST_F(bvh_tester, bvh_traverser_go_down)
	{

		build_with_pointes({ 0.f,1.f });

		std::vector<geometry::AABB1f> aabbs{ {0.f,1.f},{1.f,2.f} };
		geometry::AABB1f a_AABB{ 0.2,0.8f };

		std::vector<int> intersected_nodes;

		auto internal_fn = [&](const auto& box)
			{
				return a_AABB.intersect(box);
			};

		auto leaf_fn = [&](int i)
			{
				intersected_nodes.push_back(i);
			};
		m_traverser.traverse_top_down(m_bvh, aabbs, internal_fn, leaf_fn);

		std::vector<int> exp{ 0 };
		EXPECT_THAT(intersected_nodes, Eq(exp));
	}

}

