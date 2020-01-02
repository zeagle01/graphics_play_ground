
from scipy.linalg import solve
from Half_Edge import *

class Delaunay_Triangulator:
    def __init__(self):
        self.topo=Half_Edge()

    def triangulate(self,points):
        pass

    def get_triangles(self):
        return [[0,1,2]]


    def point_in_triange(self,p, t):
        d = np.array([
            t[1] - t[0],
            t[2] - t[0]
        ]
        )
        dp = np.array(
            p - t[0]
        )
        w = solve(d.transpose(), dp.transpose())
        # p1=d.transpose().dot(w)+t[0]
        w = np.array([w[0], w[1], 1 - w[0] - w[1]])
        compare = np.logical_and(w < 1, w > 0)
        if (compare.all()):
            return True
        else:
            return False

    def find_current_vertex_reside_triangle(self):
        for ti, t in enumerate(self.topo.get_all_triangles()):
            if self.topo.t_deleted[ti]:
                continue
            isNan = np.any(np.isnan(t))
            if not isNan and self.point_in_triange(self.X[self.current_vertex], self.X[t]):
                return ti
        return  None

    def add_current_vertex_into_triangle(self,t):
        self.topo.add_vertex_in_triangle(t)


    def add_one(self):
        t=self.find_current_vertex_reside_triangle()
        if t is not None:
            self.add_current_vertex_into_triangle(t)
        self.current_vertex+=1

    def init(self,points):
        M = np.max(points)
        bounding_triangles = np.array(
            [
                [3 * M, 0],
                [0, 3 * M],
                [-3 * M, -3 * M],
            ]
        )
        self.X = np.concatenate((bounding_triangles, points))
        self.topo.construct_from_triangles([[0,1,2]])
        self.current_vertex=3

    def get_exist_triangles(self):
        return self.topo.get_triangles()
