import unittest
from Half_Edge import *
from container_assert_helpers import  *




class Half_Edge_With_One_Triangle_Test(unittest.TestCase):
    def setUp(self) -> None :
        self.half_edge=Half_Edge()
        self.half_edge.construct_from_triangles([[0,1,2]])

    def test_edge(self):
        act=self.half_edge.get_edges()
        exp=[[1,0],[2,1],[0,2]]
        assertArrayEquals(self,act,exp)


    def test_insert_vertex_in_triangle(self):
        self.half_edge.add_vertex_in_triangle(0)
        assertArrayEqualsUnordedly(self, self.half_edge.get_one_ring(0), [1,2,3])
        assertArrayEqualsUnordedly(self, self.half_edge.get_one_ring(1), [0,2,3])
        assertArrayEqualsUnordedly(self, self.half_edge.get_one_ring(2), [0,1,3])


class Half_Edge_With_2by2_Square_Test(unittest.TestCase):
    def setUp(self) -> None :
        self.half_edge=Half_Edge()
        self.half_edge.construct_from_triangles([[0,1,2],[0,2,3]])


    def test_edge_with_2by2_squre(self):
        assertArrayEquals(self,self.half_edge.get_edges(),[[1,0],[2,1],[0,2],[3,2],[0,3]])


    def test_one_ring(self):
        assertArrayEqualsUnordedly(self, self.half_edge.get_one_ring(0), [1,2,3])
        assertArrayEqualsUnordedly(self, self.half_edge.get_one_ring(1), [0,2])


    def test_flip_edge(self):
        self.half_edge.flip_edge(0,2,1,3)


class Half_Edge_With_2by2_Square_Flip_Test(unittest.TestCase):
    def setUp(self) -> None :
        self.half_edge=Half_Edge()
        self.half_edge.construct_from_triangles([[0,1,2],[0,2,3]])
        self.half_edge.flip_edge(0, 2, 1, 3)

    def test_edge(self):
        assertArrayEquals(self,self.half_edge.get_edges(),[[1,0],[2,1],[1,3],[3,2],[0,3]])

    def test_trianlge(self):
        assertArrayEquals(self,self.half_edge.get_triangles(),[[0,1,3],[2,3,1]])

    def test_neighbor_triangle(self):
        self.assertEqual(self.half_edge.get_neighbor_triangle(2,1,3),3)
        self.assertEqual(self.half_edge.get_neighbor_triangle(3,1,3),2)


    def test_one_ring(self):
        assertArrayEqualsUnordedly(self, self.half_edge.get_one_ring(0), [1,3])
        assertArrayEqualsUnordedly(self, self.half_edge.get_one_ring(1), [0,2,3])
        assertArrayEqualsUnordedly(self, self.half_edge.get_one_ring(2), [1,3])
        assertArrayEqualsUnordedly(self, self.half_edge.get_one_ring(3), [0,1,2])

    def test_neighbor_triangle(self):
        self.assertEqual(self.half_edge.get_neighbor_triangle(2,1,3),3)
        self.assertEqual(self.half_edge.get_neighbor_triangle(3,1,3),2)

class Half_Edge_Add_Vertex_To_Triangle_Test(unittest.TestCase):
    def setUp(self) -> None :
        self.half_edge=Half_Edge()
        self.half_edge.construct_from_triangles([[0,1,2]])
        self.half_edge.add_vertex_in_triangle(0)

    def test_edge(self):
        assertArrayEquals(self,self.half_edge.get_edges(),[[1,0],[2,1],[0,2],[3,1],[3,2],[3,0]])

    def test_trianlge(self):
        assertArrayEquals(self,self.half_edge.get_triangles(),[[0,1,3],[1,2,3],[2,0,3]])


    def test_one_ring(self):
        assertArrayEqualsUnordedly(self, self.half_edge.get_one_ring(0), [1,2,3])
        assertArrayEqualsUnordedly(self, self.half_edge.get_one_ring(1), [0,2,3])
        assertArrayEqualsUnordedly(self, self.half_edge.get_one_ring(2), [0,1,3])
        assertArrayEqualsUnordedly(self, self.half_edge.get_one_ring(3), [0,1,2])

    def test_neighbor_triangle(self):
        self.assertEqual(self.half_edge.get_neighbor_triangle(1,1,3),2)
        self.assertEqual(self.half_edge.get_neighbor_triangle(2,2,3),3)
        self.assertEqual(self.half_edge.get_neighbor_triangle(3,3,0),1)



if __name__ == '__main__':
    unittest.main()