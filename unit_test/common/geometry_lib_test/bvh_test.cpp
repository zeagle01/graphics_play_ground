
#include "gmock/gmock.h"
#include <vector>
#include <span>
#include <ranges>


import geometry_lib;

using namespace testing;

namespace bvh_test
{
	TEST(bvh_test, one_dimension_tree)
	{
		geometry::bvh l_bvh;

		std::vector<float> points{ 0.f,1.f };

		l_bvh.build<float>(points);

		std::vector<geometry::node> traversed_nodes;

		auto get_node = [&](const auto& curr, const auto& l, const auto& r)
			{
				traversed_nodes.push_back(curr);
				return true;

			};

		l_bvh.traverse_bottom_up({ 1,geometry::node_type::leaf }, get_node);

		std::vector<geometry::node> exp{ {1,geometry::node_type::leaf},{0,geometry::node_type::inner} };

		std::ranges::sort(exp);
		std::ranges::sort(traversed_nodes);
		EXPECT_THAT(traversed_nodes, Eq(exp));

	}

	TEST(bvh_test, traverse_top_down)
	{
		geometry::bvh l_bvh;

		std::vector<float> points{ 0.f,1.f };

		l_bvh.build<float>(points);

		std::vector<geometry::node> traversed_nodes;

		auto get_node = [&](const auto& curr, const auto& l, const auto& r)
			{
				traversed_nodes.push_back(curr);
				return true;

			};

		l_bvh.traverse_top_down({ 0,geometry::node_type::inner }, get_node);

		std::vector<geometry::node> exp{ {0,geometry::node_type::inner},{0,geometry::node_type::leaf},{1,geometry::node_type::leaf} };
		std::ranges::sort(exp);
		std::ranges::sort(traversed_nodes);
		EXPECT_THAT(traversed_nodes, Eq(exp));

	}


	TEST(bvh_test, update_internal_AABBs)
	{

		geometry::bvh l_bvh;
		std::vector<float> points{ 0.f,1.f };
		l_bvh.build<float>(points);

		std::vector<geometry::AABB1f> aabbs{ {0.f,1.f},{1.f,2.f} };

		geometry::bvh_traverser<geometry::AABB1f> traverser;
		traverser.update_iternal_box(l_bvh, aabbs);

		std::vector<geometry::AABB1f> exp{ {0.f,2.f} };
		auto internal_aabbs = traverser.get_internal_AABB();
		EXPECT_THAT(internal_aabbs, Eq(exp));
	}

	TEST(bvh_test, bvh_traverser_go_down)
	{

		geometry::bvh l_bvh;
		std::vector<float> points{ 0.f,1.f };
		l_bvh.build<float>(points);

		std::vector<geometry::AABB1f> aabbs{ {0.f,1.f},{1.f,2.f} };
		geometry::AABB1f a_AABB{ 0.2,0.8f };

		std::vector<int> intersected_nodes;

		geometry::bvh_traverser<geometry::AABB1f> traverser;
		auto collect_intersected = [&](const auto& curr, const auto& l, const auto& r, bool intersected)
			{
				if (intersected && curr.type == geometry::node_type::leaf)
				{
					intersected_nodes.push_back(curr.index);
				}
				return true;
			};
		traverser.traverse_top_down(a_AABB, l_bvh, aabbs, collect_intersected);

		std::vector<int> exp{ 0 };
		EXPECT_THAT(intersected_nodes, Eq(exp));
	}
}

