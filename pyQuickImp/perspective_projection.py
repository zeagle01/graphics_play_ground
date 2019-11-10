
import  numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

plane=np.mat(
[
[-1,0,1],
[-1,1,1],
[1,0,1],
[1,1,1]
]
).transpose()

plane_z1=np.mat(
    [
        [-2,-2,1],
        [-2,2,1],
        [2,-2,1],
        [2,2,1]
    ]
).transpose()

projective_transform=np.mat(
    [
        [1, 0, 0],
        [0, 1, 0.5],
        [0, 0, 1]
    ]
).transpose()

tranformed_plane=projective_transform*plane

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')


def plot_plane(ax,plane):
    ax.plot_wireframe(plane[0, :].reshape(2, 2), plane[1, :].reshape(2, 2), plane[2, :].reshape(2, 2))

def plot_ray(ax,plane):
    for i in range(plane.shape[1]):
        data=[[0,0,0]]
        data.append([plane[0,i],plane[1,i],plane[2,i]])
        data=np.array(data).transpose()
        ax.plot(data[0],data[1],data[2],'r')
        d=data[:,1]-data[:,0]
        z=np.array([0,0,1])
        dz=np.dot(z,d)
        d=d/d[2]
        ax.scatter(*d,'green')



plot_plane(ax,plane)
plot_plane(ax,tranformed_plane)
plot_plane(ax,plane_z1)
ax.scatter(0,0,0)
plot_ray(ax,plane)
plot_ray(ax,tranformed_plane)



plt.show()