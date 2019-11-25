
import numpy as np
import matplotlib.pyplot as plt
from scipy.linalg import solve



display_delay=0.005
points=np.random.rand(5,2)

#n=[2,2]
#x=np.linspace(0,1,n[0])
#y=np.linspace(0,1,n[1])
#x,y=np.meshgrid(x,y)
#x=x.reshape(n[0]*n[1])
#y=y.reshape(n[0]*n[1])
#points=np.array([x,y])
#points=points.transpose()

M=np.max(points)

triangles=np.array([
    [
        [3*M,0],
        [0, 3*M],
        [-3 * M, -3 * M],
    ]
])


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

fig=plt.figure()
ax0=fig.add_subplot(221)
#ax0.scatter(points[0],points[1])

ax1=fig.add_subplot(222)
#for t in triangles:
#    t_points=np.array([t[0],t[1],t[2],t[0]])
#    ax0.plot(t_points[:,0],t_points[:,1])
#    ax1.plot(t_points[:,0],t_points[:,1])


def draw_triangle(ax,t):
    t_points=np.array([t[0],t[1],t[2],t[0]])
    ax.plot(t_points[:,0],t_points[:,1])
    plt.pause(display_delay)

def draw_triangles(ax,triangles):
    for t in triangles:
        t_points=np.array([t[0],t[1],t[2],t[0]])
        ax.plot(t_points[:,0],t_points[:,1])
    plt.pause(display_delay)

for i,p in enumerate(points):

    #plt.clf()
    ax0.clear()
    ax1.clear()
    ax0.scatter(p[0],p[1])
    ti,t=find_triangle_point_reside(p,triangles)
    draw_triangle(ax0,t)
    draw_triangles(ax1,triangles)

    if(t is not None):
        nt0=np.array([[p, t[0], t[1]]])
        draw_triangle(ax0, nt0[0])
        nt1=np.array([[p, t[1], t[2]]])
        draw_triangle(ax0, nt1[0])
        nt2=np.array([[p, t[2], t[0]]])
        draw_triangle(ax0, nt2[0])
        triangles=np.concatenate((triangles,nt0))
        triangles=np.concatenate((triangles,nt1))
        triangles=np.concatenate((triangles,nt2))
        triangles[ti]=None







plt.show()
