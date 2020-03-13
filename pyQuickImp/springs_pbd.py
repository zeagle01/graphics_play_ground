
import matplotlib.pyplot as plt
import numpy as np
from scipy.spatial.transform import Rotation as rot


fig,ax=plt.subplots(1,2)







class Pin_Constraints:
    def __init__(self, stencil):
        self.stencil = stencil
    def precompute(self,X,W):
        self.X0=X[self.stencil[0]]
    def apply(self,X,W):
        p=self.X0-X[self.stencil[0]]
        c=1.0
        X[self.stencil[0]]+=c*p
        pass





class Spring_Constraints:
    def __init__(self, stencil):
        self.stencil = stencil
    def precompute(self,X,W):
        self.L=np.linalg.norm(X[self.stencil[1]]-X[self.stencil[0]])
    def apply(self,X,W):
        p = X[self.stencil[1]] - X[self.stencil[0]]
        l = np.linalg.norm(p)
        l0 = self.L
        p = p / l * (l - l0)
        c = 0.05

        X[self.stencil[0]] = X[self.stencil[0]] + c* W[self.stencil[0]]/(W[self.stencil[0]]+W[self.stencil[1]])* p
        X[self.stencil[1]] = X[self.stencil[1]] - c *W[self.stencil[1]]/(W[self.stencil[0]]+W[self.stencil[1]])* p
        pass


class Dynamic:
    def __init__(self):
        #self.X= np.array([[0,0,0],[1,0,0],[2,0,0],[2,0,0]])
        #self.stencils=np.array([[0,1],[1,2],[2,3]])
        self.X= np.array([[0,0,0],[1,0,0],[1.5,0,0]])
        self.stencils=np.array([[0,1],[1,2]])

        self.build_spring_constraints()




        self.V=np.zeros_like(self.X)
        self.dt=1/10
        self.W=np.ones(len(self.X))
        self.g=np.array([0,-10,0])

    def build_spring_constraints(self):
        self.constraints=[]
        self.constraints.append(Pin_Constraints(np.array([0])))
        for i in range(2):
            self.constraints.append(Spring_Constraints(self.stencils[i]))

    def build_rigid_constraints(self):
        self.constraints=[]
        self.constraints.append(Pin_Constraints(np.array([0])))
        for i in range(1):
            self.constraints.append(Spring_Constraints(self.stencils[i]))
        for i in range(1,2):
            self.constraints.append(Vertex_Ridgid_Body_Quaternion_Constraints(self.stencils[i]))

    def precompute(self):
        self.L=np.linalg.norm(self.X[self.stencils[:,1]]-self.X[self.stencils[:,0]],axis=1)
        for c in self.constraints:
            c.precompute(self.X,self.W)

    def pbd(self):

        X0=np.copy(self.X)
        dV=np.einsum('i,j',self.W,self.g)

        self.V+=dV*self.dt

        self.X=self.X+self.dt*self.V
        for it in range(10):
            for c in self.constraints:
                c.apply(self.X,self.W)


        self.V=(self.X-X0)/self.dt


        ax[0].clear()
        ax[0].set_xlim([-2.5, 2.5])
        ax[0].set_ylim([-4, 1])
        ax[0].plot(self.X.transpose()[0],self.X.transpose()[1],'-x')

        plt.pause(0.01)



        ax[0].clear()
        ax[0].set_xlim([-2.5, 2.5])
        ax[0].set_ylim([-4, 1])
        ax[0].plot(self.X.transpose()[0],self.X.transpose()[1],'-x')



def onpress(event):
    print('%s click: button=%d, x=%d, y=%d, xdata=%f, ydata=%f' %
          ('double' if event.dblclick else 'single', event.button,
           event.x, event.y, event.xdata, event.ydata))

def onmove(event):
    print('move', event.x, event.y, event.xdata, event.ydata)


def onrelease(event):
    printf("release")

cid = fig.canvas.mpl_connect('button_press_event', onpress)
cid = fig.canvas.mpl_connect('motion_notify_event', onmove)
cid = fig.canvas.mpl_connect('button_release', onrelease)

dy=Dynamic()
dy.precompute()
for it in range(1000):
    dy.pbd()


plt.show()
