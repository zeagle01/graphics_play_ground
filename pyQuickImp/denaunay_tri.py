
import numpy as np
import matplotlib.pyplot as plt
import matplotlib
from scipy.linalg import solve
from  Half_Edge import *



display_delay=0.05
np.random.seed(10)
points=np.random.rand(16,2)

#n=[2,2]
#x=np.linspace(0,1,n[0])
#y=np.linspace(0,1,n[1])
#x,y=np.meshgrid(x,y)
#x=x.reshape(n[0]*n[1])
#y=y.reshape(n[0]*n[1])
#points=np.array([x,y])
#points=points.transpose()

M=np.max(points)

bounding_triangles=np.array(
    [
        [3*M,0],
        [0, 3*M],
        [-3 * M, -3 * M],
    ]
)



def point_in_triange(p,t):
    d=np.array([
        t[1]-t[0],
        t[2] - t[0]
    ]
    )
    dp=np.array(
        p-t[0]
    )
    w=solve(d.transpose(),dp.transpose())
    #p1=d.transpose().dot(w)+t[0]
    w=np.array([w[0],w[1],1-w[0]-w[1]])
    compare=np.logical_and(w < 1 ,w > 0)
    if(compare.all()):
        return True
    else:
        return False



def find_triangle_point_reside(p,triangles,is_deleted):
    for ti,t in enumerate(triangles):
        if is_deleted[ti]:
            continue
        isNun=np.any(np.isnan(t))
        if not isNun and point_in_triange(p,t):
            return ti,t
    return (-1,None)


fig1,ax=plt.subplots(1,3)
ax=np.array(ax)
ax=ax.reshape(-1)


def draw_triangle(ax,t,color):
    t_points=np.array([t[0],t[1],t[2],t[0]])
    ax.plot(t_points[:,0],t_points[:,1],color=color)
    plt.pause(display_delay)

def draw_triangles(ax,triangles,color):
    for t in triangles:
        t_points=np.array([t[0],t[1],t[2],t[0]])
        ax.plot(t_points[:,0],t_points[:,1],color=color)
    plt.pause(display_delay)



def segment_intersect(s0,s1):
    d0=np.array([s1[0]-s0[0],s1[1]-s0[0]])
    d1=np.array([s1[0]-s0[1],s1[1]-s0[1]])
    c0=np.linalg.det(d0)
    c1=np.linalg.det(d1)
    if c0*c1>0:
        return False
    else:
        return True

def need_flip(X,v_new,vi,vj,opposite_v):
    A=np.array([
        [1,X[vi][0], X[vi][1], X[vi][0] ** 2 + X[vi][1] ** 2],
        [1,X[vj][0], X[vj][1], X[vj][0] ** 2 + X[vj][1] ** 2],
        [1,X[opposite_v][0], X[opposite_v][1], X[opposite_v][0] ** 2 + X[opposite_v][1] ** 2],
        [1, X[v_new][0], X[v_new][1], X[v_new][0] ** 2 + X[v_new][1] ** 2],
    ]
    )
    Gamma=np.array([
        [1,X[vi][0],X[vi][1]],
        [1, X[vj][0], X[vj][1]],
        [1, X[opposite_v][0], X[opposite_v][1]]
    ])

    min_v=min(vi,vj)
    if vi<=2 and vj<=2:
        return False
    #elif vi>2 and vj>2:
    #    pass
    elif min_v<=2 and opposite_v<min_v:
        return False

#    if vi<=2 and  vj >2:
#        return True
#    elif vj<=2 and vi>2:
#        return True
#    elif opposite_v<=2 and vi>2 and vj>2:
#        return False
#    elif opposite_v<2:
#        return False
#    if vi <opposite_v and vj< opposite_v:
#        return False

    if not segment_intersect([X[vi],X[vj]],[X[v_new],X[opposite_v]]):
        return False

    det_A=np.linalg.det(A)
    det_Gamma=np.linalg.det(Gamma)
    if det_A*det_Gamma<0:#inside circle
        return True
    else :
        return False


def  legalize(ax,cg,X,topo,v_new,vi,vj):

    TV=topo.get_all_triangles()
    ti=[v_new,vi,vj]
    draw_triangle(ax[0],X[ti],cg.get_next_color())

    vij=(vi,vj)
    ax[0].plot(X[vij,0],X[vij,1],color=cg.get_next_color())
    plt.pause(display_delay)

    t=topo.get_triangle_by_vertex(vi,vj,v_new)
    if t ==-1:
        return

    t_neighbor=topo.get_neighbor_triangle( t, vi, vj)

#    h=topo.get_h(vj,vi)
#    if h is None:
#        return
#    ho=topo.get_opposite_h(h)
#    t_neighbor=topo.ht[ho]
    neighbor_tv=topo.get_triangle_vetex(t_neighbor)
    if neighbor_tv is None:
        return
    TV=topo.get_all_triangles()
    draw_triangle(ax[0],X[neighbor_tv],cg.get_next_color())
    opposite_v=neighbor_tv[0]+neighbor_tv[1]+neighbor_tv[2]-vi-vj
    if need_flip(X,v_new,vi,vj,opposite_v):
        topo.flip_edge(vi,vj,v_new,opposite_v)
        draw_topo_change(ax[1], X, v_new , topo, 2)
        legalize(ax,cg,X,topo, v_new, vi, opposite_v)
        legalize(ax,cg,X,topo, v_new, vj, opposite_v)





class color_getter():
    def __init__(self):
        self.color_count=0
        #self.cmap = matplotlib.cm.get_cmap('tab10')
        #self.cmap = matplotlib.cm.get_cmap('Greys')
        self.cmap = matplotlib.cm.get_cmap('jet')

    def get_next_color(self):
        ret = self.cmap(self.color_count)
        self.color_count += 1/10.0
        if self.color_count>1:
            self.color_count=0
        return ret


def draw_point_index(ax,X):
    for xi,x in enumerate(X):
        ax.text(x[0],x[1],'$'+str(xi)+"$")

def get_final_triangle(topo):
    TV=topo.get_all_triangles();
    t_deleted=topo.t_deleted
    ret=[]
    for ti,t in enumerate(TV):
        if t_deleted[ti]:
            continue
        helper_v=False
        for v in t:
            if v<3:
                helper_v=True
                break;
        if helper_v:
            continue
        ret.append(t)
    return ret


def draw_topo_change(ax,X,xi,topo,n):
    ax.clear()
    #TV_final = np.array(get_final_triangle(topo))
    TV_final = np.array(topo.get_triangles())

    ax.scatter(X[3:xi, 0], X[3:xi, 1], color='black')
    if len(TV_final) != 0:
        draw_triangles(ax, X[TV_final], 'grey')
        #TV=np.array(topo.get_all_triangles())
        if n!=0:
            TV=TV_final[-n:]
            if len(TV)==n:
                draw_triangles(ax, X[TV], 'black')


def denauley_triangulate(X):
    topo = Half_Edge()
    TV0 = np.array([ [ 0, 1, 2 ] ])
    topo.construct_from_triangles( TV0.tolist())

    X = np.concatenate((bounding_triangles, X))

    #draw_triangles(ax[2], X[TV0])
    #ax[2].scatter(X[:,0],X[:,1])
    #ax[2].text(-0.1,-0.1,'$P$')

    for xi in range(3,len(X)):
        cg = color_getter()
        x=X[xi]
        ax[0].clear()
        draw_topo_change(ax[1], X,xi, topo,0)
        ax[2].clear()
        TV_final=get_final_triangle(topo)

        if len(TV_final)!=0:
            TV_final=np.array(TV_final)
            draw_triangles(ax[2], X[TV_final],'black')
            draw_point_index(ax[2], X[2:xi+1])

        ax[0].scatter(X[:, 0], X[:, 1],color='grey')
        #ax[0].text(-0.1, -0.1, '$P$')

        ax[0].scatter(X[3:xi, 0],X[3:xi,1],color='black')

        ax[0].scatter(x[0],x[1],color='red')
        #ax[0].text(x[0],x[1],'$p_r$')

        draw_point_index(ax[0], X[0:xi+1])

        TV_exist=np.array(topo.get_triangles())
        draw_triangles(ax[0], X[TV_exist],'grey')



        TV=np.array(topo.get_all_triangles())
        ti,t=find_triangle_point_reside(x,X[TV],topo.t_deleted)
        draw_triangle(ax[0],t,cg.get_next_color())
        if(t is not None):
            v_new=topo.add_vertex_in_triangle(ti)
            draw_topo_change(ax[1], X, xi+1, topo, 3)
            h=topo.vh[v_new]
            for tvi in range(3):
                hi=h
                vi=topo.hv[hi]
                hj=topo.get_opposite_h(h)
                hj=topo.hn[hj]
                vj=topo.hv[hj]
                legalize(ax,cg,X,topo,v_new,vi,vj)
                h = topo.hn[h]
                h = topo.hn[h]
                h= topo.get_opposite_h(h)


denauley_triangulate(points)



plt.show()
