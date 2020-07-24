
import matplotlib.pyplot as plt
import numpy as np


class Projector:
    def __init__(self):
        self.M = np.identity(4)
        self.V = np.identity(4)
        self.P = np.identity(4)
        self.t=real(0)
    
    def project(self,X):
        out=np.empty((4,X.shape[0]))
        out[0:3,:]=X.transpose()
        out[3,:]=np.ones(X.shape[0])


        self.t+=real(0.05)
        self.V[0:3:2,0:3:2]=np.array([
            [np.cos(self.t), -np.sin(self.t)],
            [np.sin(self.t), np.cos(self.t)]
        ])


        mvp=np.einsum('ij,jk,kl',self.P,self.V,self.M)
        out=np.einsum('ij,jk',mvp,out)
        return out[0:2,:]


####### data input
real=np.float32
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
class Spring_Constraint:
    def __init__(self,x,w,stiff,l0):
        self.x=x
        self.w=w
        self.stiff=stiff
        self.l0=l0

    def compute_A(self):
        a=self.stiff*np.einsum('i,j',self.w,self.w)
        return a

    def compute_p(self):
        p=np.einsum('i,ij',self.w,self.x)
        lp=np.linalg.norm(p)
        p*=self.l0/lp
        return p

    def compute_b(self):
        p=self.compute_p()
        b=self.stiff*np.einsum('i,j',self.w,p)
        return b

class Spring_Solver:
    def __init__(self,sim_data):
        self.sim_data=sim_data
        self.w=np.array([1,-1])
        self.stiff=sim_data.c_stretch

    def pre_compute(self):
        ##to generalize
        self.stencils= [
            [0,1],
            [1, 2],
            [2, 3],
            [3, 0],
            [0, 2],
        ]
        self.l0=np.zeros(len(self.stencils),dtype=real)
        for i,stencil in enumerate(self.stencils):
            d=np.einsum('i,ij',self.w,self.sim_data.X_rest[stencil])
            self.l0[i]=np.linalg.norm(d)

    def generate_constraints(self):
        self.constraints=[]
        for i,stencil in enumerate(self.stencils):
            constraint=Spring_Constraint(self.sim_data.X[stencil],self.w,self.stiff,self.l0[i])
            self.constraints.append(constraint)

    def get_stencils(self):
        return self.stencils;

    def get_constraints(self):
        return self.constraints



class Bending_Constraint:
    def __init__(self,x,stiff):
        self.x=x
        self.stiff=stiff

    def cot(self,x0,x1,x2):
        d=np.array([x1-x0,x2-x0])
#        l0=np.dot(d[0],d[0])
#        l1=np.dot(d[0],d[1]/np.linalg.norm(d[1]))
#        l1=l1*l1
#        ret=np.sqrt((l0-l1)/l1)

        cro=np.cross(d[0],d[1])
        cro=np.linalg.norm(cro)
        dot=np.dot(d[0],d[1])
        return cro/dot

    def area(self,x0,x1,x2):
        cro=np.cross(x1-x0,x2-x0)
        return np.linalg.norm(0.5*cro)

    def compute_A(self):
        E=np.array([
            [1,1,0,0],
            [1, 0, 1, 0],
            [1, 0, 0, 1],
            [0, 1, 1, 0],
            [0, 1, 0, 1]
        ],dtype=real)
        L=np.array([
            0,
            -self.cot(self.x[0],self.x[1],self.x[2]),
            -self.cot(self.x[0], self.x[1], self.x[3]),
            -self.cot(self.x[1], self.x[0], self.x[2]),
            -self.cot(self.x[1], self.x[0], self.x[3])
        ])
        L[0]=np.sum(-L[1:])
        K=np.einsum('i,ij',L,E)
        M=self.area(self.x[0],self.x[1],self.x[2])+self.area(self.x[0],self.x[1],self.x[3])
        Q=self.stiff/M*np.einsum('i,j',K,K)
        return Q

    def compute_b(self):
        b=np.zeros((4,3))
        return b

class Bending_Solver:
    def __init__(self,sim_data):
        self.sim_data=sim_data
        self.stiff=sim_data.c_bending

    def pre_compute(self):
        ##to generalize
        self.stencils= [
            [0,2,1,3]
        ]

    def generate_constraints(self):
        self.constraints=[]
        for i,stencil in enumerate(self.stencils):
            constraint=Bending_Constraint(self.sim_data.X[stencil],self.stiff)
            self.constraints.append(constraint)

    def get_stencils(self):
        return self.stencils;

    def get_constraints(self):
        return self.constraints



class Inertial_Constraint:
    def __init__(self,x,v,m,h,g):
        self.x=x
        self.v=v
        self.m=m
        self.h=h
        self.g=g

    def compute_A(self):
        return np.array([[self.m/self.h/self.h]])

    def compute_b(self):
        #return self.m/self.h/self.h*(self.x+np.array([0,-0.1,0]))
        return self.m/self.h/self.h*(self.x+self.v*self.h)+self.h*self.h*np.array([0,self.g,0],dtype=real)

class Inertial_Solver:
    def __init__(self,sim_data):
        self.sim_data=sim_data

    def pre_compute(self):
        v_num=self.sim_data.X.shape[0]
        self.stencils=np.arange(v_num)
        self.stencils=self.stencils.reshape((v_num,1))

    def generate_constraints(self):
        self.constraints=[]
        for stencil in self.stencils:
            self.constraints.append(Inertial_Constraint(self.sim_data.X[stencil],self.sim_data.V[stencil],self.sim_data.m,self.sim_data.h,self.sim_data.c_gravity))

    def get_stencils(self):
        return self.stencils;
    def get_constraints(self):
        return self.constraints


class Sim_Data:
    def __init__(self,X,T):
        self.X=X
        self.X0=X.copy()
        self.X_rest=X.copy()
        self.V=np.zeros_like(X)
        self.T=T

#independent
        self.h=real(1.)
        self.m=real(1.)
        self.l=real(1.)

#dependent
        self.c_stretch=real(1e5)
        self.c_bending=real(1e1)
        self.c_gravity=real(-1)

class PD_Solver:
    def __init__(self,sim_data):
        self.sim_data=sim_data

        self.constraint_solvers=[]
        self.constraint_solvers.append(Inertial_Solver(self.sim_data))
        self.constraint_solvers.append(Spring_Solver(self.sim_data))
        self.constraint_solvers.append(Bending_Solver(self.sim_data))

        for constraint_solver in self.constraint_solvers:
            constraint_solver.pre_compute()

    def find_matrix_eigen(self):
        v_num=self.sim_data.X.shape[0]
        A=np.zeros((v_num,v_num),dtype=float)
        for i in range(v_num):
            for j in range(len(self.J[i])):
                jj=self.J[i][j]
                A[i,jj]=self.A[i][j]

        M=np.diag(np.diag(A))
        N=M-A
        B=np.dot(np.linalg.inv(M),N)
        w,v=np.linalg.eig(B)
        return w,v

    def update(self):
        self.generate_constraints()
        self.sim_data.X0=self.sim_data.X.copy()
        self.assemble_matrix()
        return self.find_matrix_eigen()

    def generate_constraints(self):
        for constraint_solver in self.constraint_solvers:
            constraint_solver.generate_constraints()

    def put(self,vi,vj,v,J,A):
        if vj not in J[vi]:
            J[vi].append(vj)
            A[vi].append(v)
        else:
            ind=J[vi].index(vj)
            A[vi][ind]+=v

    def assemble_matrix(self):
        v_num=self.sim_data.X.shape[0]
        self.J=[[] for i in range(v_num)]
        self.A=[[] for i in range(v_num)]
        self.b=np.zeros((v_num,3))
        for constraint_solver in self.constraint_solvers:
            stencils=constraint_solver.get_stencils()
            constraints=constraint_solver.get_constraints()
            for stencil,constraint in zip(stencils,constraints):
                local_A = constraint.compute_A()
                local_b = constraint.compute_b()
                for vi_loc,vi in enumerate(stencil):
                    self.b[vi]+=local_b[vi_loc]
                    for vj_loc,vj in enumerate(stencil):
                        self.put(vi,vj,local_A[vi_loc][vj_loc],self.J,self.A)

    def solve(self):
        v_num=self.sim_data.X.shape[0]
        for row in range(v_num):
            if row==0:
                continue
            off=np.zeros(3)
            diag_loc=None
            for col_loc,col in enumerate(self.J[row]):
                if col==row:
                    diag_loc=col_loc
                else:
                    off+=self.A[row][col_loc]*self.sim_data.X[col]
            self.sim_data.X[row]=(self.b[row]-off)/self.A[row][diag_loc]


    def get_position(self):
        return self.sim_data.X


fig,ax=plt.subplots(1,1)

sim_data=Sim_Data(X,T)
#b=-2
#e=4
#delta=1.0
x=[]
y=[]
y1=[]
y2=[]
for eks in range(-12,22):
    sim_data.c_stretch=2**eks
    rho=0
    sim_data.c_bending=1e-3
    x.append(sim_data.c_stretch)
    y1.append(sim_data.c_stretch)
    y2.append(sim_data.c_stretch*0.07)
    while rho<1:
        solver=PD_Solver(sim_data)
        w,v=solver.update()
        rho=np.max(np.abs(w))
        sim_data.c_bending *=2.0
    y.append(sim_data.c_bending)

ax.plot(x,y,'-o',label='$k_b$')
ax.plot(x,y1,'-o',label='$k_s$')
ax.plot(x,y2,'-o',label='$0.07k_s$')

ax.legend()
plt.show()





