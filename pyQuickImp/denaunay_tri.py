
import numpy as np
import matplotlib.pyplot as plt
from scipy.linalg import solve
from  Half_Edge import *



display_delay=0.005
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



def find_triangle_point_reside(p,triangles):
    for ti,t in enumerate(triangles):
        isNun=np.any(np.isnan(t))
        if not isNun and point_in_triange(p,t):
            return ti,t
    return (-1,None)


fig1,ax=plt.subplots(2,2)
ax=np.array(ax)
ax=ax.reshape(-1)


def draw_triangle(ax,t):
    t_points=np.array([t[0],t[1],t[2],t[0]])
    ax.plot(t_points[:,0],t_points[:,1],'black')
    plt.pause(display_delay)

def draw_triangles(ax,triangles):
    for t in triangles:
        t_points=np.array([t[0],t[1],t[2],t[0]])
        ax.plot(t_points[:,0],t_points[:,1],'grey')
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


def  legalize(topo,v_new,vi,vj):
    h=topo.vh[vi]
    ho=topo.get_opposite_h(h)
    t_neighbor=topo.ht[ho]


def denauley_triangulate(X):
    topo = Half_Edge()
    TV0 = np.array([ [ 0, 1, 2 ] ])
    topo.construct_from_triangles( TV0.tolist())

    X = np.concatenate((bounding_triangles, X))


    for xi in range(3,len(X)):
        x=X[xi]
        ax[0].clear()
        ax[1].clear()
        ax[0].scatter(x[0],x[1])
        TV=np.array(topo.get_triangles())
        ti,t=find_triangle_point_reside(x,X[TV])
        draw_triangle(ax[0],t)
        draw_triangles(ax[1],X[TV])
        draw_triangle(ax[1],t)
        ax[1].scatter(X[3:xi+1,0], X[3:xi+1,1])
        if(t is not None):
            #nt0=np.array([[x, t[0], t[1]]])
            #draw_triangle(ax[0], nt0[0])
            #nt1=np.array([[x, t[1], t[2]]])
            #draw_triangle(ax[0], nt1[0])
            #nt2=np.array([[x, t[2], t[0]]])
            #draw_triangle(ax[0], nt2[0])
            v_new=topo.add_vertex_in_triangle(ti)
            TV = np.array(topo.get_triangles())
            for tvi in range(3):
                tvi_next=(tvi+1)%3
                tv=TV[ti]
                legalize(topo,v_new,tv[tvi],tv[tvi_next])
            #TV[ti]
            #triangles=np.concatenate((triangles,nt0))
            #triangles=np.concatenate((triangles,nt1))
            #triangles=np.concatenate((triangles,nt2))
            #triangles[ti]=None


denauley_triangulate(points)



plt.show()
