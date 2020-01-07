
import matplotlib.pyplot as plt
import numpy as np


fig,ax=plt.subplots(1,2)


class Dynamic:
    def __init__(self):
        self.X= np.array([[0,0,0],[1,0,0],[2,0,0]])
        self.stencils=np.array([[0,1],[1,2]])
        #self.X= np.array([[0,0,0],[1,0,0]])
        #self.stencils=np.array([[0,1]])

        self.V=np.zeros_like(self.X)
        self.dt=1/10
        self.W=np.ones(len(self.X))
        self.g=np.array([0,-10,0])

    def precompute(self):
        self.L=np.linalg.norm(self.X[self.stencils[:,1]]-self.X[self.stencils[:,0]])
        #if len(self.L)==1:
        #    self.L.reshape(1,-1)

    def pbd(self):

        X0=np.copy(self.X)
        dV=np.einsum('i,j',self.W,self.g)
        self.V=self.V+dV*self.dt
        self.V[0]=np.zeros(3)
        self.X=self.X+self.dt*self.V
        #self.X[0]=np.zeros(3)
        for it in range(10):
            for stencil_i,stencil in enumerate(self.stencils):
                p=self.X[stencil[1]]-self.X[stencil[0]]
                l=np.linalg.norm(p)
                l0=1
                p=p/l*(l-l0)
                if stencil[0]!=0:
                    self.X[stencil[0]] = self.X[stencil[0]] + 0.1 * p
                if stencil[1]!=0:
                    self.X[stencil[1]] = self.X[stencil[1]] - 0.1 * p

        self.V=(self.X-X0)/self.dt

        ax[0].clear()
        ax[0].set_xlim([-2.5, 2.5])
        ax[0].set_ylim([-4, 1])
        ax[0].plot(self.X.transpose()[0],self.X.transpose()[1],'-x')
        plt.pause(0.01)



dy=Dynamic()
dy.precompute()
for it in range(1000):
    dy.pbd()

plt.show()
