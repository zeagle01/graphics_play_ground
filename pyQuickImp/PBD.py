
import matplotlib.pyplot as plt
import numpy as np


fig,ax=plt.subplots(1,2)

MIN_ANGLE=1e-7
CUTOFF_ANGLE=np.pi



def check_parameterization(v,theta):
    rep=0
    if theta>CUTOFF_ANGLE:
        scl=theta
        if theta>2*np.pi:
            theta=np.mod(theta,2*np.pi)
            scl=theta/scl
            v=v*scl
            rep=1
        if theta>CUTOFF_ANGLE:
            scl=theta
            theta=2*np.pi-theta
            scl=1-2*np.pi/scl
            v=scl*v
            rep=1
    return theta,rep

def EM_To_Q(v,reparam):
    rep=0
    theta=0
    q=np.empty(4)
    if reparam:
        theta,rep=check_parameterization(v,theta)
    else:
        theta=np.linalg.norm(v)

    theta=np.linalg.norm(v)
    cosp=np.cos(0.5*theta)
    sinp=np.sin(0.5*theta)
    q[3]=cosp
    if theta<MIN_ANGLE:
        q[0:3]=v*0.5-theta*theta/48.0
    else:
        q[0:3]=v*sinp/theta
    return q,rep


def Q_to_Matrix(q):
    xy=2*q[0]*q[1]
    xz=2*q[0]*q[2]
    yz=2*q[1]*q[2]
    wx=2*q[3]*q[0]
    wy=2*q[3]*q[1]
    wz=2*q[3]*q[2]
    xx=2*q[0]*q[0]
    yy=2*q[1]*q[1]
    zz=2*q[2]*q[2]
    R=np.array([
        [1-(yy+zz),xy-wz,xz+wy],
        [xy+wz,1-(xx+zz),yz-wx],
        [xz-wy,yz+wx,1-(xx+yy)]
    ])
    return R



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


#todo
class Vertex_Ridgid_Body_Constraints:
    def __init__(self, stencil):
        self.stencil = stencil
    def precompute(self,X,W):
        pass
    def apply(self,X,W):
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
        self.R=np.zeros_like(self.X)

        self.constraints=[]
        self.constraints.append(Pin_Constraints(np.array([0])))
        for i in range(1):
            self.constraints.append(Spring_Constraints(self.stencils[i]))

        for i in range(1,2):
            self.constraints.append(Vertex_Ridgid_Body_Constraints(self.stencils[i]))




        self.V=np.zeros_like(self.X)
        self.dt=1/10
        self.W=np.ones(len(self.X))
        self.g=np.array([0,-10,0])

    def precompute(self):
        self.L=np.linalg.norm(self.X[self.stencils[:,1]]-self.X[self.stencils[:,0]],axis=1)
        for c in self.constraints:
            c.precompute(self.X,self.W)

    def pbd(self):

        X0=np.copy(self.X)
        dV=np.einsum('i,j',self.W,self.g)
        self.V=self.V+dV*self.dt
        self.X=self.X+self.dt*self.V
        for it in range(10):
            for c in self.constraints:
                c.apply(self.X,self.W)


        self.V=(self.X-X0)/self.dt

        ax[0].clear()
        ax[0].set_xlim([-2.5, 2.5])
        ax[0].set_ylim([-4, 1])
        ax[0].plot(self.X.transpose()[0],self.X.transpose()[1],'-x')

        for vi,v in enumerate(self.R):
            q,rep=EM_To_Q(v,1)
            R=Q_to_Matrix(q)
            frame=np.array([self.X[vi],self.X[vi]+R[0],
                          self.X[vi],self.X[vi]+R[1],
                          self.X[vi],self.X[vi]+R[2]
                          ])
            xp=frame[:,0:2]
            ax[0].plot(*(xp[0:2].transpose()), 'red')
            ax[0].plot(*(xp[2:4].transpose()), 'green')
            ax[0].plot(*(xp[4:6].transpose()), 'blue')
        plt.pause(0.01)



dy=Dynamic()
dy.precompute()
for it in range(1000):
    dy.pbd()

plt.show()
