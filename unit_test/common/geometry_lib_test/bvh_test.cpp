
#include "gmock/gmock.h"
#include <vector>
#include <span>


import geometry_lib;

using namespace testing;

namespace bvh_test
{
	TEST(bvh_test, one_dimension_tree)
	{
		geometry::bvh<float> bvh;

		std::vector<float> points{ 0.f,1.f };

		bvh.build(points);

		EXPECT_THAT(bvh.parent(0), Eq(bvh.parent(1)));

	}

	TEST(bvh_test, traverse_with_one_aabb_but_not_intersect)
	{
		geometry::bvh<float> bvh;

		std::vector<float> points{ 0.f,1.f };

		bvh.build(points);

		std::vector<geometry::AABB1f> aabbs{ {0.f,1.f},{1.f,2.f} };
		geometry::AABB1f my_box{ -1.0f,-0.5f };
		std::vector<int> intersected_nodes;

		auto is_intersect = [&](int i)
			{
				bool intersected = my_box.intersect(aabbs[i]);
				if (intersected)
				{
					intersected_nodes.push_back(i);
				}
				return  intersected;
			};


		geometry::bvh_traverser{}(bvh, is_intersect);

		EXPECT_THAT(intersected_nodes, Eq(std::vector<int>{}));
	}
}

