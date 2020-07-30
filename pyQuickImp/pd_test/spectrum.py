
import matplotlib.pyplot as plt
import numpy as np
from constraint import *
from convert_obj_to_XT import *

#file='mesh/2by2_square.obj'
#file='mesh/triangle_stride1.obj'
file='mesh/triangle_stride.obj'
####### data input
X,T=read_obj(file)
X=np.array(X)
T=np.array(T)



class Spetrum_Checker(PD_Solver):

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





fig,ax=plt.subplots(1,1)

sim_data=Sim_Data(X,T)
#b=-2
#e=4
#delta=1.0
x=[]
y=[]
y1=[]
y2=[]

def get_spectrum_radiu():
    solver = Spetrum_Checker(sim_data)
    w, v = solver.update()
    rho = np.max(np.abs(w))
    return rho

def get_spectrum_radiu_with_bending(c_bending):
    sim_data.c_bending=c_bending
    return get_spectrum_radiu()


for eks in range(-12,22):
    sim_data.c_stretch=2**eks
    rho=0
    sim_data.c_bending=1e-3
    x.append(sim_data.c_stretch)
    y1.append(sim_data.c_stretch)
    y2.append(sim_data.c_stretch*0.03)
    while True:
        rho=get_spectrum_radiu()
        if rho >1:
            break
        sim_data.c_bending *=2.0

#binary solve
    c_bending_critic_interval=[sim_data.c_bending/2,sim_data.c_bending]
    while True:
        c_bending_critic = 0.5 * (c_bending_critic_interval[0] + c_bending_critic_interval[1])
        rho=get_spectrum_radiu_with_bending(c_bending_critic)
        if rho>1:
            c_bending_critic_interval[1]=c_bending_critic
        else:
            c_bending_critic_interval[0]=c_bending_critic
        err=(c_bending_critic_interval[1]-c_bending_critic_interval[0])/c_bending_critic_interval[0]
        if err<1e-3:
            break

    y.append(sim_data.c_bending)

ax.plot(x,y,'-o',label='$k_b$')
ax.plot(x,y1,'-o',label='$k_s$')
ax.plot(x,y2,'-o',label='$0.03k_s$')

ax.legend()
plt.show()





