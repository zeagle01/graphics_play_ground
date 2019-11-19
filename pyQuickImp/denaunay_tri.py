
import numpy as np
import matplotlib.pyplot as plt




points=np.random.rand(2,100)

M=np.max(points)

triangles=np.array([
    [
        [3*M,0],
        [0, 3*M],
        [-3 * M, -3 * M],
    ]
])


#for p in points:




fig=plt.figure()
ax0=fig.add_subplot(221)
ax0.scatter(points[0],points[1])

ax1=fig.add_subplot(222)
for t in triangles:
    t_points=np.array([t[0],t[1],t[2],t[0]])
    ax0.plot(t_points[:,0],t_points[:,1])
    ax1.plot(t_points[:,0],t_points[:,1])

plt.show()

