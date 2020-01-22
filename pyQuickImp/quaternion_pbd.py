
import matplotlib.pyplot as plt
import numpy as np
from scipy.spatial.transform import Rotation as rot


fig,ax=plt.subplots(1,2)

def crossM(v):
    return np.array(
        [
            [0,v[2],-v[1]],
            [-v[2],0,v[0]],
            [v[1],-v[0],0]
        ]
    )

def quaternion_multiply(p,q):
    ret=np.zeros(4)
    ps=p[3]
    pv=p[0:3]
    qs=q[3]
    qv=q[0:3]
    ret[0:3]=ps*qv+qs*pv+np.cross(pv,qv)
    ret[3]=ps*qs-np.dot(pv,qv)
    return ret

def quaternion_conjugate(q):
    return np.array([*-q[0:3], q[3]])


def derivativeRp(q,p):
    ret=np.zeros((3,4))
    qs=q[3]
    qv=q[0:3]
    ret[:,3]=2*qs*p-2*np.cross(p,qv)
    ret[:,0:3]=2*np.dot(p,qv)*np.identity(3)\
               +2*np.einsum('i,j',qv,p)  -2*np.einsum('i,j',p,qv)\
               -2*qs*crossM(p)
    return ret



class Pin_Constraints:
    def __init__(self, stencil):
        self.stencil = stencil
    def precompute(self,X,W):
        self.X0=X[self.stencil[0]]
    def apply(self,X,q,W):
        p=self.X0-X[self.stencil[0]]
        c=1.0
        X[self.stencil[0]]+=c*p
        pass




class Spring_Constraints:
    def __init__(self, stencil):
        self.stencil = stencil
    def precompute(self,X,W):
        self.L=np.linalg.norm(X[self.stencil[1]]-X[self.stencil[0]])
    def apply(self,X,q,W):
        p = X[self.stencil[1]] - X[self.stencil[0]]
        l = np.linalg.norm(p)
        l0 = self.L
        p = p / l * (l - l0)
        c = 0.05

        X[self.stencil[0]] = X[self.stencil[0]] + c* W[self.stencil[0]]/(W[self.stencil[0]]+W[self.stencil[1]])* p
        X[self.stencil[1]] = X[self.stencil[1]] - c *W[self.stencil[1]]/(W[self.stencil[0]]+W[self.stencil[1]])* p
        pass



class Vertex_Ridgid_Body_Constraints:
    def __init__(self, stencil):
        self.stencil = stencil

    def precompute(self,X,W):
        pass

    def apply(self,X,q,W):
        r=np.array([-0.5,0,0])
        x=np.array([X[self.stencil[0]],X[self.stencil[1]]])

#
        R=rot.from_quat(q[self.stencil[1]]).as_matrix()
        p_ridgid=X[self.stencil[1]]+np.dot(R,r);
        p=np.array([X[self.stencil[0]],p_ridgid])
        C=p[1]-p[0]
        J_x=np.eye(3)
        c=1e-3
        X[self.stencil[0]]+=np.dot(J_x.transpose(),C)*c
        X[self.stencil[1]]-=np.dot(J_x.transpose(),C)*c
        J_q=derivativeRp(q[self.stencil[1]],r)
        q[self.stencil[1]]-=np.dot(J_q.transpose(),C)*c
        q[self.stencil[1]]/=np.linalg.norm(q[self.stencil[1]])
        pass








class Dynamic:
    def __init__(self):
        #self.X= np.array([[0,0,0],[1,0,0],[2,0,0],[2,0,0]])
        #self.stencils=np.array([[0,1],[1,2],[2,3]])
        self.X= np.array([[0,0,0],[1,0,0],[1.5,0,0]])
        self.stencils=np.array([[0,1],[1,2]])

        self.q=np.zeros((len(self.X),4))
        self.q[:,3]=1;
        self.omega=np.zeros_like(self.X)
        #self.omega[:,2]=1e0
        self.tau=np.zeros_like(self.X)
        #self.tau[:,2]=1e1

        self.inertial=np.zeros((len(self.X),3,3))
        self.inv_inertial=np.zeros_like(self.inertial)
        for i in range(len(self.X)):
            self.inertial[i]=np.diag((1,1,1))
            self.inv_inertial[i]=np.linalg.inv(self.inertial[i])




        self.build_rigid_constraints()




        self.V=np.zeros_like(self.X)
        self.dt=1/100
        self.W=np.ones(len(self.X))
        self.g=np.array([0,-10,0])


    def build_rigid_constraints(self):
        self.constraints=[]
        self.constraints.append(Pin_Constraints(np.array([0])))
        for i in range(1):
            self.constraints.append(Spring_Constraints(self.stencils[i]))
        for i in range(1,2):
            self.constraints.append(Vertex_Ridgid_Body_Constraints(self.stencils[i]))

    def precompute(self):
        self.L=np.linalg.norm(self.X[self.stencils[:,1]]-self.X[self.stencils[:,0]],axis=1)
        for c in self.constraints:
            c.precompute(self.X,self.W)

    def apply_torque(self):
        #tt=np.einsum('...k,...k',self.inertial ,self.omega)
        #tt=self.tau-np.cross(self.omega,tt)
        #tt=np.einsum('...k,...k',self.inv_inertial,tt)
        #self.omega=self.omega+tt*self.dt

        ## world
        R=rot.from_quat(self.q).as_matrix()
        invI=np.einsum('...i,...ij',R,self.inv_inertial)
        invI=np.einsum('...i,...i',invI,R)
        self.omega=self.omega+np.einsum('...k,...k',invI,self.tau)*self.dt

    def orientation_integrate(self):
        for i in range(len(self.q)):
            omega_tilde=np.array([*self.omega[i],0])
            #self.q[i]+=quaternion_multiply(self.q[i],omega_tilde)*self.dt*0.5
            self.q[i]+=quaternion_multiply(omega_tilde,self.q[i])*self.dt*0.5
            self.q[i]/=np.linalg.norm(self.q[i])


    def update_angular_velocity(self,q0):
        for i in range(len(self.q)):
            q_conjugate=quaternion_conjugate(self.q[i])
            tt=quaternion_multiply(q_conjugate,q0[i])
            tt=2*tt/self.dt
            self.omega[i]=tt[0:3]


    def draw(self):
        ax[0].clear()
        ax[0].set_xlim([-2.5, 2.5])
        ax[0].set_ylim([-4, 1])
        ax[0].plot(self.X.transpose()[0],self.X.transpose()[1],'-x')

        R = rot.from_quat(self.q).as_matrix()
        for vi,v in enumerate(self.q):
            frame=np.array([self.X[vi],self.X[vi]+R[vi][0],
                            self.X[vi],self.X[vi]+R[vi][1],
                            self.X[vi],self.X[vi]+R[vi][2]
                            ])
            xp=frame[:,0:2]
            ax[0].plot(*(xp[0:2].transpose()), 'red')
            ax[0].plot(*(xp[2:4].transpose()), 'green')
            ax[0].plot(*(xp[4:6].transpose()), 'blue')
        plt.pause(0.01)


    def pbd(self):

        X0=np.copy(self.X)
        q0=np.copy(self.q)

        dV=np.einsum('i,j',self.W,self.g)##apply force
        self.V=self.V+dV*self.dt
        self.apply_torque()

        self.X=self.X+self.dt*self.V ##position integrate
        self.orientation_integrate()

        for it in range(10):
            for c in self.constraints:
                c.apply(self.X,self.q,self.W)


        self.V=(self.X-X0)/self.dt ##update velocity
        self.update_angular_velocity(q0)

        self.draw()





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
