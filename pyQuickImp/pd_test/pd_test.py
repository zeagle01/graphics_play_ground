
import matplotlib.pyplot as plt
import numpy as np
from constraint import *
from convert_obj_to_XT import *






#file='mesh/2by2_square.obj'
#file='mesh/triangle.obj'
file='mesh/triangle_stride1.obj'
#file='mesh/triangle_stride.obj'

####### data input
X,T=read_obj(file)
X=np.array(X)
T=np.array(T)

sim_data=Sim_Data(X,T)

sim_data.c_stretch=1e2
sim_data.c_bending=1e-2
sim_data.c_gravity=-1

solver=PD_Solver(sim_data)

fig,ax=plt.subplots(1,1)
plt.ion()

t=real(0.)
while True:
    ax.cla()
    ax.set_xlim([-1,1])
    ax.set_ylim([-1,1])
    #update X here
    X[:,0]+=0.01*np.sin(0.1*t)
    X=solver.update()

    plotX=projector.project(X)
    for xi,x in enumerate(plotX.transpose()):
        ax.text(x[0],x[1],str(xi))
    for tri in T:
        ax.fill(plotX[0,tri],plotX[1,tri],'-o',fill=False)
    t+=sim_data.h
    plt.pause(0.01)

plt.ioff()
plt.show()





