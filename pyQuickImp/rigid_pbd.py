
import matplotlib.pyplot as plt
import numpy as np
from scipy.spatial.transform import Rotation as rot


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
        q[0:3]=v*(0.5-theta*theta/48.0)
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

############## dRdv ##########
def partial_q_partial_v(v,i):
    theta=np.linalg.norm(v)
    cosp=np.cos(0.5*theta)
    sinp=np.sin(0.5*theta)

    dqdx=np.empty(4)
    i1 = (i + 1) % 3
    i2 = (i + 2) % 3
    if theta<MIN_ANGLE:
        Tsinc=0.5-theta*theta/48.0
        vTerm=v[i]*(theta*theta/40.0-1.0)/24.0
        dqdx[3]=-0.5*v[i]*Tsinc
        dqdx[i]=v[i]*vTerm+Tsinc
        dqdx[i1]=v[i1]*vTerm
        dqdx[i2]=v[i2]*vTerm
    else:
        ang=1.0/theta
        ang2=ang*ang*v[i]
        sang=sinp*ang
        cterm=ang2*(0.5*cosp-sang)

        dqdx[i]=cterm*v[i]+sang
        dqdx[i1]=cterm*v[i1]
        dqdx[i2]=cterm*v[i2]
        dqdx[3]=-0.5*v[i]*sang

    return dqdx


def partial_R_partial_v(q,dqdvi):
    prod=np.empty(9)
    dRdvi=np.eye(3)
    prod[0]=-4*q[0]*dqdvi[0]
    prod[1]=-4*q[1]*dqdvi[1]
    prod[2]=-4*q[2]*dqdvi[2]
    prod[3]=2*(q[1]*dqdvi[0]+q[0]*dqdvi[1])
    prod[4]=2*(q[3]*dqdvi[2]+q[2]*dqdvi[3])
    prod[5]=2*(q[2]*dqdvi[0]+q[0]*dqdvi[2])
    prod[6]=2*(q[3]*dqdvi[1]+q[1]*dqdvi[3])
    prod[7]=2*(q[2]*dqdvi[1]+q[1]*dqdvi[2])
    prod[8]=2*(q[3]*dqdvi[0]+q[0]*dqdvi[3])

    dRdvi[0,0]=prod[1]+prod[2]
    dRdvi[0,1]=prod[3]-prod[4]
    dRdvi[0,2]=prod[5]+prod[6]

    dRdvi[1,0]=prod[3]+prod[4]
    dRdvi[1,1]=prod[0]+prod[2]
    dRdvi[1,2]=prod[7]-prod[8]

    dRdvi[2,0]=prod[5]-prod[6]
    dRdvi[2,1]=prod[7]+prod[8]
    dRdvi[2,2]=prod[0]+prod[1]

    return dRdvi



def partial_R_partial_EM(v,i):
    q,rep=EM_To_Q(v,1)
    dqdvi=partial_q_partial_v(v,i)
    dRdvi=partial_R_partial_v(q,dqdvi)
    return dRdvi,rep


###########################################################
def quaternion_multiply(quaternion1, quaternion0):
    w0, x0, y0, z0 = quaternion0
    w1, x1, y1, z1 = quaternion1
    return np.array([-x1 * x0 - y1 * y0 - z1 * z0 + w1 * w0,
                     x1 * w0 + y1 * z0 - z1 * y0 + w1 * x0,
                     -x1 * z0 + y1 * w0 + z1 * x0 + w1 * y0,
                     x1 * y0 - y1 * x0 + z1 * w0 + w1 * z0] )







class Pin_Constraints:
    def __init__(self, stencil):
        self.stencil = stencil
    def precompute(self,X,W):
        self.X0=X[self.stencil[0]]
    def apply(self,X,v,W):
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

    def apply(self,X,v,W):
        r=np.array([-0.5,0,0])
        x=np.array([X[self.stencil[0]],X[self.stencil[1]]])

        q, rep = EM_To_Q(v[self.stencil[1]], 1)
        R = Q_to_Matrix(q)

        p_ridgid=X[self.stencil[1]]+np.dot(R,r);
        p=np.array([X[self.stencil[0]],p_ridgid])
        C=p[1]-p[0]
        J_x=np.eye(3)
        X[self.stencil[0]]+=np.dot(J_x.transpose(),C)
        X[self.stencil[1]]-=np.dot(J_x.transpose(),C)
        dRdv=np.array([partial_R_partial_EM(v[self.stencil[1]],0)[0],
                       partial_R_partial_EM(v[self.stencil[1]],1)[0],
                       partial_R_partial_EM(v[self.stencil[1]],2)[0]])
        J_R=np.array([np.dot(dRdv[0],r),np.dot(dRdv[1],r),np.dot(dRdv[2],r)])
        v[self.stencil[1]]-=np.dot(J_R.transpose(),C)
        pass


class Vertex_Ridgid_Body_Quaternion_Constraints:
    def __init__(self, stencil):
        self.stencil = stencil

    def precompute(self,X,W):
        pass

    def apply(self,X,q,W):
        r=np.array([-0.5,0,0])
        x=np.array([X[self.stencil[0]],X[self.stencil[1]]])

        q, rep = EM_To_Q(v[self.stencil[1]], 1)
        R = Q_to_Matrix(q)

        p_ridgid=X[self.stencil[1]]+np.dot(R,r);
        p=np.array([X[self.stencil[0]],p_ridgid])
        C=p[1]-p[0]
        J_x=np.eye(3)
        X[self.stencil[0]]+=np.dot(J_x.transpose(),C)
        X[self.stencil[1]]-=np.dot(J_x.transpose(),C)
        dRdv=np.array([partial_R_partial_EM(v[self.stencil[1]],0)[0],
                       partial_R_partial_EM(v[self.stencil[1]],1)[0],
                       partial_R_partial_EM(v[self.stencil[1]],2)[0]])
        J_R=np.array([np.dot(dRdv[0],r),np.dot(dRdv[1],r),np.dot(dRdv[2],r)])
        v[self.stencil[1]]-=np.dot(J_R.transpose(),C)
        pass

class Spring_Constraints:
    def __init__(self, stencil):
        self.stencil = stencil
    def precompute(self,X,W):
        self.L=np.linalg.norm(X[self.stencil[1]]-X[self.stencil[0]])
    def apply(self,X,v,W):
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
        self.R_vec=np.zeros_like(self.X)
        self.R_quaternion=np.zeros((len(self.X),4))
        self.omega=np.zeros_like(self.X)

        self.build_spring_constraints()
        #self.build_rigid_constraints()




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
        q0=np.copy((rot.from_rotvec(self.R_vec)).as_quat())

        self.V+=dV*self.dt
        self.R_vec+=self.omega*self.dt

        self.X=self.X+self.dt*self.V
        for it in range(10):
            for c in self.constraints:
                c.apply(self.X,self.R_vec,self.W)


        self.V=(self.X-X0)/self.dt
        q=(rot.from_rotvec(self.R_vec)).as_quat()
        for i in range(len(q)):
            q_conjugate=np.array([q0[i][3],*(-q0[i][0:3])])
            q_i=np.array([q[i][3],*(q[i][0:3])])
            self.omega[i]=2./self.dt*quaternion_multiply(q_i,q_conjugate)[1:4]


        ax[0].clear()
        ax[0].set_xlim([-2.5, 2.5])
        ax[0].set_ylim([-4, 1])
        ax[0].plot(self.X.transpose()[0],self.X.transpose()[1],'-x')

        for vi,v in enumerate(self.R_vec):
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

    def quaternion(self):
        X0=np.copy(self.X)
        dV=np.einsum('i,j',self.W,self.g)
        #q0=np.copy((rot.from_rotvec(self.R_vec)).as_quat())

        self.V+=dV*self.dt
        self.R_quaternion+=self.omega*self.dt

        self.X=self.X+self.dt*self.V
        for it in range(10):
            for c in self.constraints:
                c.apply(self.X,self.R_vec,self.W)


        self.V=(self.X-X0)/self.dt
        q=(rot.from_rotvec(self.R_vec)).as_quat()
        for i in range(len(q)):
            q_conjugate=np.array([q0[i][3],*(-q0[i][0:3])])
            q_i=np.array([q[i][3],*(q[i][0:3])])
            self.omega[i]=2./self.dt*quaternion_multiply(q_i,q_conjugate)[1:4]


        ax[0].clear()
        ax[0].set_xlim([-2.5, 2.5])
        ax[0].set_ylim([-4, 1])
        ax[0].plot(self.X.transpose()[0],self.X.transpose()[1],'-x')

        for vi,v in enumerate(self.R_vec):
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
