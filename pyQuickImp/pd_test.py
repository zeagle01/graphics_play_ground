
import matplotlib.pyplot as plt
import numpy as np


class Projector:
    def __init__(self):
        self.M = np.identity(4)
        self.V = np.identity(4)
        self.P = np.identity(4)
    
    def project(self,X):
        out=np.empty((4,X.shape[0]))
        out[0:3,:]=X.transpose()
        out[3,:]=np.ones(X.shape[0])

        mvp=np.einsum('ij,jk,kl',self.P,self.V,self.M)
        out=np.einsum('ij,jk',mvp,out)
        return out[0:2,:]


####### data input
real=np.float32
t=real(0.)
X=np.array([
    [0,0,0],
    [0.5,0,0],
    [0.5, 0.5, 0],
    [0.0, 0.5, 0]
], dtype=real)

T=np.array(
    [[0,1,2],
     [0, 2, 3]]
)
############
projector=Projector()



################ constraints





fig,ax=plt.subplots(1,1)
plt.ion()
while True:
    ax.cla()
    ax.set_xlim([-1,1])
    ax.set_ylim([-1,1])
    plotX=projector.project(X)
    for xi,x in enumerate(plotX.transpose()):
        ax.text(x[0],x[1],str(xi))
    for tri in T:
        ax.fill(plotX[0,tri],plotX[1,tri],'-o',fill=False)
    t+=0.1
    plt.pause(0.1)

plt.ioff()
plt.show()





