import unittest
import numpy as np


class Half_Edge:
    def  __init__(self):
        self.hv = []
        self.hn = []
        self.ht = []
        self.vh = []
        self.th = []

    def get_opposite_h(self,hi):
        ei=hi//2
        pos=hi%2
        return ei*2+(pos+1)%2;


    def make_one_ring_begin(self):
        for h in self.vh:
            h0=h
            while True:
                h=self.get_opposite_h(h)
                if self.ht[h]==-1:
                    break
                h=self.hn[h]
                if h==h0:
                    break


    def construct_from_triangles(self,v_num,TV):
        self.vh=[-1]*v_num
        vv = [[] for i in range(v_num)]
        vhi = [[] for i in range(v_num)]
        for ti,tv in enumerate(TV):
            hi = [-1] * 3
            hoi=[-1]*3
            for tvi in range(3):
                tvi_next = (tvi + 1) % 3
                ev = [tv[tvi], tv[tvi_next]]

                if ev[0] in vv[ev[1]]:
                    vvi=vv[ev[1]].index(ev[0])
                    hoi[tvi] = vhi[ev[1]][vvi]
                    hi[tvi] = self.get_opposite_h(hoi[tvi])
                    self.ht[hi[tvi]]=ti
                else:
                    self.hv.append(ev[1])
                    self.hv.append(ev[0])
                    self.hn.append(-1)
                    self.hn.append(-1)
                    self.ht.append(ti)
                    self.ht.append(-1)
                    hi[tvi]=len(self.hv)-2
                    hoi[tvi]=len(self.hv)-1

                vv[ev[0]].append(ev[1])
                vhi[ev[0]].append(hi[tvi])

                if self.vh[ev[0]]==-1:
                    self.vh[ev[0]]=hi[tvi]
            for i in range(3):
                i_next=(i+1)%3;
                self.hn[hi[i]]=hi[i_next]
            self.th.append(hi[0])

        self.make_one_ring_begin()

    def get_edges(self):
        ret=[]
        for v in self.hv:
            ret.append(v)
        e_num=len(ret)//2
        return np.reshape(ret,(e_num,2)).tolist()

    def get_one_ring(self,v):
        ret=[]
        h=self.vh[v]
        h0=h
        while True:
            ret.append(self.hv[h])
            if self.ht[h]==-1:
                break
            h=self.hn[h]
            h=self.hn[h]
            h=self.get_opposite_h(h)
            if h==h0:
                break

        return ret





class Half_Edge_Test(unittest.TestCase):
    def setUp(self) -> None :
        self.half_edge=Half_Edge()

#    def test_edge_with_triangle(self):
#        self.half_edge.construct_from_triangles(3,[[0,1,2]])
#        act=self.half_edge.get_edges()
#        exp=[[1,0],[2,1],[0,2]]
#        self.assertTrue(np.array_equal(act,exp))

    def test_edge_with_2by2_squre(self):
        self.half_edge.construct_from_triangles(4,[[0,1,2],[0,2,3]])
        self.assertTrue(np.array_equal(self.half_edge.get_edges(),[[1,0],[2,1],[0,2],[3,2],[0,3]]))


    def test_one_ring(self):
        self.half_edge.construct_from_triangles(4,[[0,1,2],[0,2,3]])
        self.assertTrue(np.array_equal(self.half_edge.get_one_ring(0),[1,2,3]))
        self.assertTrue(np.array_equal(self.half_edge.get_one_ring(1),[2,0]))



if __name__ == '__main__':
    unittest.main()