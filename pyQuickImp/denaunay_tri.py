
import numpy as np
import matplotlib.pyplot as plt
from scipy.linalg import solve
from  Half_Edge import *



display_delay=0.005
np.random.seed(0)
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


fig1,ax=plt.subplots(1,1)
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

def dummy_split():
    for i,p in enumerate(points):
        #plt.clf()
        ax[0].clear()
        ax[1].clear()
        ax[0].scatter(p[0],p[1])
        ti,t=find_triangle_point_reside(p,triangles)
        draw_triangle(ax[0],t)
        draw_triangles(ax[1],triangles)
        draw_triangle(ax[1],t)
        ax[1].scatter(points[0:i+1,0], points[0:i+1,1])

        if(t is not None):
            nt0=np.array([[p, t[0], t[1]]])
            draw_triangle(ax[0], nt0[0])
            nt1=np.array([[p, t[1], t[2]]])
            draw_triangle(ax[0], nt1[0])
            nt2=np.array([[p, t[2], t[0]]])
            draw_triangle(ax[0], nt2[0])
            triangles=np.concatenate((triangles,nt0))
            triangles=np.concatenate((triangles,nt1))
            triangles=np.concatenate((triangles,nt2))
            triangles[ti]=None


def need_flip(X,v_new,vi,vj,opposite_v):
    A=np.array([
        [X[v_new][0],X[v_new][1],X[v_new][0]**2+X[v_new][1]**2,1],
        [X[vi][0], X[vi][1], X[vi][0] ** 2 + X[vi][1] ** 2, 1],
        [X[vj][0], X[vj][1], X[vj][0] ** 2 + X[vj][1] ** 2, 1],
        [X[opposite_v][0], X[opposite_v][1], X[opposite_v][0] ** 2 + X[opposite_v][1] ** 2, 1],
    ]
    )

    det_A=np.linalg.det(A)
    if det_A>0:
        return False
    else :
        return True


def  legalize(ax,X,topo,v_new,vi,vj):

    TV=topo.get_all_triangles()
    ti=[v_new,vi,vj]
    draw_triangle(ax,X[ti],[0,0,1,1])

    vij=(vi,vj)
    ax.plot(X[vij,0],X[vij,1],color=[1,0,0,1])
    plt.pause(display_delay)
    h=topo.get_h(vj,vi)
    if h is None:
        return
    ho=topo.get_opposite_h(h)
    t_neighbor=topo.ht[ho]
    neighbor_tv=topo.get_triangle_vetex(t_neighbor)
    if neighbor_tv is None:
        return
    TV=topo.get_all_triangles()
    draw_triangle(ax,X[neighbor_tv],[0,1,0,1])
    opposite_v=neighbor_tv[0]+neighbor_tv[1]+neighbor_tv[2]-vi-vj
    if need_flip(X,v_new,vi,vj,opposite_v):
        #topo.flip_edge(vi,vj,v_new,opposite_v)
        legalize(ax,X,topo, v_new, vi, opposite_v)
        legalize(ax,X,topo, v_new, vj, opposite_v)








def denauley_triangulate(X):
    topo = Half_Edge()
    TV0 = np.array([ [ 0, 1, 2 ] ])
    topo.construct_from_triangles( TV0.tolist())

    X = np.concatenate((bounding_triangles, X))

    #draw_triangles(ax[2], X[TV0])
    #ax[2].scatter(X[:,0],X[:,1])
    #ax[2].text(-0.1,-0.1,'$P$')

    for xi in range(3,len(X)):
        x=X[xi]
        ax[0].clear()
        #ax[1].clear()

        ax[0].scatter(X[:, 0], X[:, 1],color='grey')
        ax[0].text(-0.1, -0.1, '$P$')
        ax[0].scatter(x[0],x[1],color='red')
        ax[0].text(x[0],x[1],'$p_r$')
        TV=np.array(topo.get_all_triangles())
        draw_triangles(ax[0], X[TV],'grey')
        ti,t=find_triangle_point_reside(x,X[TV],topo.t_deleted)
        draw_triangle(ax[0],t,'black')
        #draw_triangles(ax[1],X[TV])
        #draw_triangle(ax[1],t)
        #ax[1].scatter(X[3:xi+1,0], X[3:xi+1,1])
        if(t is not None):
            #nt0=np.array([[x, t[0], t[1]]])
            #draw_triangle(ax[0], nt0[0])
            #nt1=np.array([[x, t[1], t[2]]])
            #draw_triangle(ax[0], nt1[0])
            #nt2=np.array([[x, t[2], t[0]]])
            #draw_triangle(ax[0], nt2[0])
            v_new=topo.add_vertex_in_triangle(ti)
            h=topo.vh[v_new]
            for tvi in range(3):
                hi=h
                vi=topo.hv[hi]
                hj=topo.get_opposite_h(h)
                hj=topo.hn[hj]
                vj=topo.hv[hj]
                legalize(ax[0],X,topo,v_new,vi,vj)
                h = topo.hn[h]
                h = topo.hn[h]
                h= topo.get_opposite_h(h)


denauley_triangulate(points)



plt.show()
