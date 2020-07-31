
import matplotlib.pyplot as plt
import numpy as np
from constraint import *
from convert_obj_to_XT import *




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





class Spectrum_Experiment:
    def __init__(self,sim_data,b,e):
        self.begin=b
        self.end=e
        self.sim_data=sim_data
        self.x=[]
        self.y=[]



    def find_ks_vs_kb(self):
        self.x=[]
        self.y=[]
        for eks in range(self.begin, self.end):
            self.sim_data.c_stretch = 2 ** eks
            rho = 0
            self.sim_data.c_bending = 1e-3
            self.x.append(self.sim_data.c_stretch)
            while True:
                rho = self.get_spectrum_radiu()
                if rho > 1:
                    break
                self.sim_data.c_bending *= 2.0

            # binary solve
            c_bending_critic_interval = [self.sim_data.c_bending / 2, self.sim_data.c_bending]
            while True:
                c_bending_critic = 0.5 * (c_bending_critic_interval[0] + c_bending_critic_interval[1])
                rho = self.get_spectrum_radiu_with_bending(c_bending_critic)
                if rho > 1:
                    c_bending_critic_interval[1] = c_bending_critic
                else:
                    c_bending_critic_interval[0] = c_bending_critic
                err = (c_bending_critic_interval[1] - c_bending_critic_interval[0]) / c_bending_critic_interval[0]
                if err < 1e-3:
                    break

            self.y.append(self.sim_data.c_bending)
        return self.x,self.y


    def get_spectrum_radiu(self):
        solver = Spetrum_Checker(self.sim_data)
        w, v = solver.update()
        rho = np.max(np.abs(w))
        return rho

    def get_spectrum_radiu_with_bending(self,c_bending):
        self.sim_data.c_bending=c_bending
        return self.get_spectrum_radiu()




##################################
fig,ax=plt.subplots(1,2)

files=['mesh/triangle_stride2.obj','mesh/triangle_stride4.obj','mesh/triangle_stride8.obj','mesh/triangle_stride16.obj','mesh/triangle_stride32.obj']

####### data input
for file in files:
    X,T=read_obj(file)
    X=np.array(X)
    T=np.array(T)

    sim_data=Sim_Data(X,T)
    se=Spectrum_Experiment(sim_data,-12,22)
    x,y=se.find_ks_vs_kb()

    ax[0].set_xscale('log')
    ax[0].set_yscale('log')
    ax[0].plot(x,y,'-o',label=file)

    ax[1].set_xscale('log')
    ax[1].set_yscale('log')
    ax[1].plot(x,np.array(y)/np.array(x),'-o',label=file)

ax[0].legend()
ax[1].legend()
plt.show()





