
import unittest
import numpy as np

from Delaunay_Triangulator import *
from container_assert_helpers import *




class Delaunay_Triangulator_Add_One_Test(unittest.TestCase):

    def setUp(self):
        self.trianglutor=Delaunay_Triangulator()
        np.random.seed(0)
        #points = np.random.rand(3, 2)
        points=np.array([[0,0],[0,1],[1,1]])
        self.trianglutor.init(points)
        self.trianglutor.add_one()



    def test_exsit_triangle(self):
        assertArrayEqualsUnordedly(self,self.trianglutor.get_exist_triangles(),[[0,1,3],[1,2,3],[0,2,3]])


class Delaunay_Triangulator_Add_Two_Test(unittest.TestCase):

    def setUp(self):
        self.trianglutor=Delaunay_Triangulator()
        np.random.seed(0)
        #points = np.random.rand(3, 2)
        points=np.array([[0,0],[0,1],[1,1]])
        self.trianglutor.init(points)
        self.trianglutor.add_one()
        self.trianglutor.add_one()



    def test_exsit_triangle(self):
        #assertArrayEqualsUnordedly(self,self.trianglutor.get_exist_triangles(),[[0,1,3],[1,2,3],[0,2,3]])
        pass


if __name__ == '__main__':
    unittest.main()
