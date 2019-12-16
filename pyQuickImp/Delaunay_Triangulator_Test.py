
import unittest
import numpy as np

from Delaunay_Triangulator import *
from container_assert_helpers import *




class Delaunay_Triangulator_Test(unittest.TestCase):

    def setUp(self):
        self.trianglutor=Delaunay_Triangulator()
        np.random.seed(0)
        points = np.random.rand(3, 2)
        self.trianglutor.triangulate(points)


    def test_triangulate_three(self):
        assertArrayEquals(self, self.trianglutor.get_triangles(), [[0,1,2]])


if __name__ == '__main__':
    unittest.main()
