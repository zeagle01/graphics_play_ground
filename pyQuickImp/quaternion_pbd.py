
import matplotlib.pyplot as plt
import numpy as np
from scipy.spatial.transform import Rotation as rot


fig,ax=plt.subplots(1,2)

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

    def apply(self,X,v,W):
#        r=np.array([-0.5,0,0])
#        x=np.array([X[self.stencil[0]],X[self.stencil[1]]])
#
#        q, rep = EM_To_Q(v[self.stencil[1]], 1)
#        R = Q_to_Matrix(q)
#
#        p_ridgid=X[self.stencil[1]]+np.dot(R,r);
#        p=np.array([X[self.stencil[0]],p_ridgid])
#        C=p[1]-p[0]
#        J_x=np.eye(3)
#        X[self.stencil[0]]+=np.dot(J_x.transpose(),C)
#        X[self.stencil[1]]-=np.dot(J_x.transpose(),C)
#        dRdv=np.array([partial_R_partial_EM(v[self.stencil[1]],0)[0],
#                       partial_R_partial_EM(v[self.stencil[1]],1)[0],
#                       partial_R_partial_EM(v[self.stencil[1]],2)[0]])
#        J_R=np.array([np.dot(dRdv[0],r),np.dot(dRdv[1],r),np.dot(dRdv[2],r)])
#        v[self.stencil[1]]-=np.dot(J_R.transpose(),C)
        pass


def crossM(v):
    return np.array([
        [0,v[2],-v[1]],
        [-v[2], 0, v[0]],
        [v[1], -v[0], 0]
        ]
    )

def LeftQ_sl(q):

    ret=np.zeros((4,4))
    ret[0,3]=q[3]
    ret[1:4,3]=q[0:3]
    ret[0,0:3]=-q[0:3]
    ret[1:4,0:3]=q[3]*np.identity(3)+crossM(q[0:3])
    return ret
#
#def RightQ(q):
#    ret=np.zeros((4,4))
#    ret[0,3]=q[3]
#    ret[1:4,3]=q[0:3]
#    ret[0,0:3]=-q[0:3]
#    ret[1:4,0:3]=q[3]*np.identity(3)-crossM(q[0:3])
#    return ret


#scalar first
def LeftQ_sf(q):
    ret=np.zeros((4,4))
    ret[0,0]=q[0]
    ret[1:4,0]=q[1:4]
    ret[0,1:4]=-q[1:4]
    ret[1:4,1:4]=q[0]*np.identity(3)+crossM(q[1:4])
    return ret

##scalar last
#def LeftQ_sl(q):
#    permutation=np.array([
#        [0,0,0,1],
#        [1, 0, 0, 0],
#        [0, 1, 0, 0],
#        [0, 0, 1, 0]
#         ]
#    )
#    q_sf=np.dot(permutation,q)
#    ret=LeftQ_sf(q_sf)
#    ret=np.dot(permutation.transpose(),ret)
#    return ret


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
        #self.tau[:,2]=1

        self.inertial=np.zeros((len(self.X),3,3))
        self.inv_inertial=np.zeros_like(self.inertial)
        for i in range(len(self.X)):
            self.inertial[i]=np.diag((1,2,3))
            self.inv_inertial[i]=np.linalg.inv(self.inertial[i])




        self.build_rigid_constraints()




        self.V=np.zeros_like(self.X)
        self.dt=1/10
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

    def pbd(self):

        X0=np.copy(self.X)
        dV=np.einsum('i,j',self.W,self.g)
        q0=np.copy(self.q)

        self.V+=dV*self.dt
        tt=np.einsum('...k,...k',self.inertial ,self.omega)
        tt=self.tau-np.cross(self.omega,tt)
        tt=np.einsum('...k,...k',self.inv_inertial,tt)
        self.omega+=tt*self.dt

        self.X=self.X+self.dt*self.V
        for i in range(len(self.q)):
            LQ=LeftQ_sl(self.q[i])
            omega_tilde=np.array([*self.omega[i],0])
            dd=np.dot(LQ,omega_tilde)
            self.q[i]+=dd*self.dt*0.5
            self.q[i]/=np.linalg.norm(self.q[i])

        for it in range(10):
            for c in self.constraints:
                c.apply(self.X,self.q,self.W)


        self.V=(self.X-X0)/self.dt
        for i in range(len(self.q)):
            LQ=LeftQ_sl(self.q[i])
            tt=np.dot(LQ.transpose(),q0[i])
            tt=2*tt/self.dt
            self.omega[i]=tt[0:3]

        ## update omega


        ax[0].clear()
        ax[0].set_xlim([-2.5, 2.5])
        ax[0].set_ylim([-4, 1])
        ax[0].plot(self.X.transpose()[0],self.X.transpose()[1],'-x')

        for vi,v in enumerate(self.q):
            R=rot.from_quat(self.q).as_matrix()
            frame=np.array([self.X[vi],self.X[vi]+R[vi][0],
                            self.X[vi],self.X[vi]+R[vi][1],
                            self.X[vi],self.X[vi]+R[vi][2]
                            ])
            xp=frame[:,0:2]
            ax[0].plot(*(xp[0:2].transpose()), 'red')
            ax[0].plot(*(xp[2:4].transpose()), 'green')
            ax[0].plot(*(xp[4:6].transpose()), 'blue')
        plt.pause(0.01)



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
