import unittest
import numpy as np


class Half_Edge:
    def  __init__(self):
        self.hv = []
        self.hn = []
        self.ht = []
        self.vh = []
        self.th = []
        self.TV=[]
        self.t_deleted=[]
        self.v_deleted=[]

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


    def construct_from_triangles(self,TV):
        self.v_num=np.max(TV)+1
        self.TV=TV
        self.t_deleted=[False]*len(self.TV)
        self.vh=[-1]*self.v_num
        vv = [[] for i in range(self.v_num)]
        vhi = [[] for i in range(self.v_num)]
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

    def add_vertex_in_triangle(self,ti):

        v_new=self.v_num
        self.v_num+=1
        h=self.th[ti]
        h_num0=len(self.hv)
        t_num0=len(self.TV)
        h_next_i=[[] for i in range(3)]

        self.t_deleted[t_num0-1]=True
        for hi in range(3):

            hi_pre=(hi+2)%3
            hi_next=(hi+1)%3
            v0=self.hv[h]
            v1=self.hv[self.get_opposite_h(h)]
            self.TV.append([v1,v0,v_new])
            self.t_deleted.append(False)
            self.hv.append(v_new)
            self.hv.append(v0)
            self.hn.append(-1)
            self.hn.append(-1)
            self.ht.append(t_num0+hi)
            self.ht.append(t_num0+hi_next)
            self.ht[h]=t_num0+hi;
            h_next_i[hi]=[h,h_num0+hi*2,h_num0+hi_pre*2+1]
            self.th.append(h_num0+hi*2)

            h=self.hn[h]
        for hni in h_next_i:
            for i in range(3):
                i_next=(i+1)%3
                self.hn[hni[i]]=hni[i_next]
        self.vh.append(h_next_i[0][2])
        return v_new


    def flip_edge_adjust_hv(self,t0_h0,t1_h0,v_from0,v_from1,v_to0,v_to1):
        self.hv[t0_h0]=v_to1
        self.hv[t1_h0]=v_to0

    def flip_edge_adjust_trianlge(self,t0_h0,t1_h0,v_from0,v_from1,v_to0,v_to1):
        t0=self.ht[t0_h0]
        t1=self.ht[t1_h0]
        self.t_deleted[t0]=True
        self.t_deleted[t1]=True
        self.TV.append([v_from0,v_to0,v_to1])
        self.TV.append([v_from1,v_to1,v_to0])
        self.t_deleted.append(False)
        self.t_deleted.append(False)


    def flip_edge_adjust_ht(self,t0_h0,t1_h0,t0,t1):
        self.ht[t0_h0]=t0
        self.ht[t1_h0]=t1

    def flip_edge(self,v_from0,v_from1,v_to0,v_to1):
        t0_h=[0]*3;
        t1_h=[0]*3;

        t0_h0=self.get_h(v_from0,v_from1)
        t1_h0=self.get_h(v_from1,v_from0)

        t_num0=len(self.TV)

        self.flip_edge_adjust_hv(t0_h0,t1_h0,v_from0,v_from1,v_to0,v_to1)

        self.flip_edge_adjust_trianlge(t0_h0,t1_h0,v_from0,v_from1,v_to0,v_to1)

        self.flip_edge_adjust_ht(t0_h0,t1_h0,t_num0,t_num0+1)



        t0_h[0]=t0_h0;
        t0_h[1]=self.hn[t0_h[0]]
        t0_h[2]=self.hn[t0_h[1]]

        t1_h[0]=t1_h0;
        t1_h[1]=self.hn[t1_h[0]]
        t1_h[2]=self.hn[t1_h[1]]

        new_hn0=[t1_h[0],t1_h[2],t0_h[1]]
        new_hn1=[t0_h[0],t0_h[2],t1_h[1]]
        for i in range(3):
            i_next=(i+1)%3
            self.hn[new_hn0[i]]=new_hn0[i_next]
            self.hn[new_hn1[i]]=new_hn1[i_next]



    def get_triangles(self):
        ret=[]
        for ti,t in enumerate(self.TV):
            if not self.t_deleted[ti]:
                ret.append(t)
        return ret

    def get_all_triangles(self):
        return self.TV

    def get_neighbor_triangle(self,t,v0,v1):
        h=self.get_h(v0,v1)
        if self.ht[h]==t:
            return self.ht[self.get_opposite_h(h)]
        else:
            return self.ht[h]

    def get_triangle_vetex(self,ti):
        if ti<0:
            return None
        else:
            return self.TV[ti]

    def get_edges(self):
        ret=[]
        for v in self.hv:
            ret.append(v)
        e_num=len(ret)//2
        return np.reshape(ret,(e_num,2)).tolist()

    def get_h(self,v_from,v_to):
        h=self.vh[v_from]
        h0=h
        while True:
            if self.hv[h]==v_to:
                return h
            if self.ht[h]==-1:
                break
            h=self.hn[h]
            h=self.hn[h]
            h=self.get_opposite_h(h)
            if h==h0:
                break
        return None


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





