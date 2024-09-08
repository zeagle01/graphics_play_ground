
#include "gmock/gmock.h"
#include <vector>
#include <span>
#include <ranges>
#include <algorithm>

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

	TEST_F(bvh_tester, actual_comparison_is_less)
	{
		float thickness = 1.f;
		int n = 1000;
		std::vector<float> points(n);
		build_with_pointes(points);

		std::vector<geometry::AABB1f> aabbs(n);

		for (int i = 0; i < n; i++)
		{
			float p = i * 1.f;
			points[i] = p;

			aabbs[i] = { p - 0.5f * thickness, p + 0.5f * thickness };
		}

		geometry::AABB1f target_AABB{ 0.f,1.0f };

		std::vector<int> intersected_nodes;

		int compare_count = 0;
		auto internal_fn = [&](const auto& box)
			{
				compare_count++;
				return target_AABB.intersect(box);
			};

		auto leaf_fn = [&](int i)
			{
				intersected_nodes.push_back(i);
			};
		m_traverser.traverse_top_down(m_bvh, aabbs, internal_fn, leaf_fn);

		EXPECT_THAT(compare_count, Lt(n));
	}

// Expands a 10-bit integer into 30 bits
// by inserting 2 zeros after each bit.
	unsigned int expandBits(unsigned int v)
	{
		v = (v * 0x00010001u) & 0xFF0000FFu;
		v = (v * 0x00000101u) & 0x0F00F00Fu;
		v = (v * 0x00000011u) & 0xC30C30C3u;
		v = (v * 0x00000005u) & 0x49249249u;
		return v;
	}

	// Calculates a 30-bit Morton code for the
	// given 3D point located within the unit cube [0,1].
	unsigned int morton3D(float x, float y, float z)
	{
		x = std::min(std::max(x * 1024.0f, 0.0f), 1023.0f);
		y = std::min(std::max(y * 1024.0f, 0.0f), 1023.0f);
		z = std::min(std::max(z * 1024.0f, 0.0f), 1023.0f);
		unsigned int xx = expandBits((unsigned int)x);
		unsigned int yy = expandBits((unsigned int)y);
		unsigned int zz = expandBits((unsigned int)z);
		return xx * 4 + yy * 2 + zz;
	}

	class bvh_tester_2 :public Test
	{
	protected:
		geometry::linear_binary_radix_tree<float> m_tree;
		geometry::LBVH<float,geometry::AABB1f> m_bvh;
	};

	TEST_F(bvh_tester_2, one_dimension_tree)
	{

		std::vector<float> points = { 0.f,1.f };

		m_tree.set_order_key_function(
			[](float x)
			{
				return morton3D(x, x, x);
			}
		);

		m_tree.build(points);

	}

	TEST_F(bvh_tester_2, LBVH_traverse_from_root)
	{
		std::vector<float> points = { 0.f,1.f };
		std::vector<geometry::AABB1f> aabbs{ {0.f,0.5f},{0.5f,1.f} };

		geometry::AABB1f target_AABB{ 0.f,1.0f };
		std::vector<int> insetected_aabbs;
		std::vector<int> exp_insetected_aabbs{ 0,1 };

		m_bvh.build(points, [](float x) { return morton3D(x, x, x); });
		m_bvh.assign_leaf_aabb(aabbs);


		geometry::loop_traverser<float, geometry::AABB1f> trverser;

		trverser.traverse_from_root
		(
			m_bvh,
			[&](const auto& aabb ) { return target_AABB.intersect(aabb); },
			[&](int leafI) { insetected_aabbs.push_back(leafI); }
		);


		std::sort(insetected_aabbs.begin(), insetected_aabbs.end());
		EXPECT_THAT(insetected_aabbs, Eq(exp_insetected_aabbs));
	}

	TEST_F(bvh_tester_2, LBVH_loop_traverse)
	{
		std::vector<float> points = { 0.1f,0.5,0.9f,										0.7f };
		std::vector<geometry::AABB1f> aabbs{ {0.05f,0.15f},{0.45f,0.55f},{0.85f,0.95f} ,	{0.5f,0.9f} };

		std::vector<std::pair<int, int>> insetected_aabbs;
		std::vector<std::pair<int, int>> exp_insetected_aabbs{ { {3, 1}, {3, 2} } };

		m_bvh.build(points, [](float x) { return morton3D(x, x, x); });
		m_bvh.assign_leaf_aabb(aabbs);


		geometry::loop_traverser<float, geometry::AABB1f> trverser;

		trverser.loop_traverse
		(
			points.size(), m_bvh,
			[&](const auto& aabb, int i) { 
				return aabbs[i].intersect(aabb); 
			},
			[&](auto p) {
				if (p.loopI < p.leafI && aabbs[p.loopI].intersect(aabbs[p.leafI]))
				{
					insetected_aabbs.push_back({ p.leafI, p.loopI });
				}
			}
		);

		EXPECT_THAT(insetected_aabbs, Eq(exp_insetected_aabbs));
	}
}

