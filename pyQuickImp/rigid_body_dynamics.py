
import matplotlib.pyplot as plt
import numpy as np
import unittest
from scipy.spatial.transform import Rotation as R



class RigidBody:
    def __init__(self):
        self.mass=1
        self.I_Body=np.eye(3)
        self.I_BodyInv=np.eye(3)

        self.X=np.zeros(3)
        self.R=np.eye(3)
        self.P=np.zeros(3)
        self.L=np.zeros(3)

        self.I_Inv=np.eye(3)
        self.V=np.zeros(3)
        self.omega=np.zeros(3)

        self.force=np.zeros(3)
        self.torque=np.zeros(3)

    def to_frame(self):
        #frame=R.from_rotvec(self.R)
        #ret=frame.as_matrix()

        ret=np.array([self.X,self.X+self.R[0],
                      self.X,self.X+self.R[1],
                      self.X,self.X+self.R[2]
                      ])
        return ret


def State_to_Array(rb,y):
    y[0:3]=rb.X[:]
    y[3:12]=rb.R.flatten()
    y[12:15]=rb.P
    y[15:18]=rb.L


def Array_to_State(rb,y):
    rb.X=y[0:3]
    rb.R=y[3:12].reshape((3,3))
    rb.P=y[12:15]
    rb.L=y[15:18]

    rb.v=rb.P/rb.mass
    rb.I_Inv=np.dot(rb.R,np.dot(rb.I_BodyInv,rb.R.transpose()))
    rb.omega=np.dot(rb.I_Inv,rb.L)








def ode(x0,n,t,t_step,DxDt):
    delta_x=DxDt(x0,t,n)*t_step
    x_final=x0+delta_x
    return x_final







class Displayer:
    def __init__(self):
        self.fig, self.ax = plt.subplots(1, 2)

    def display(self,bodies):
        self.ax[0].clear()
        self.ax[1].clear()

        for b in bodies:
            body_frame=b.to_frame()
            xp=self.project(body_frame)
            self.ax[0].plot(*(xp[0:2].transpose()),'red')
            self.ax[0].plot(*(xp[2:4].transpose()),'green')
            self.ax[0].plot(*(xp[4:6].transpose()),'blue')
        plt.pause(0.1)

    def project(self,x):
        MVP=R.from_rotvec([1,-1,0.5])
        x=MVP.apply(x)
        return x[:,0:2]



class Simulator:
    def __init__(self):

        self.STATE_SIZE=18
        self.NBODIES=2
        self.N_UNKNOWN_NUM=self.STATE_SIZE*self.NBODIES
        self.bodies=[RigidBody() for i in range(self.NBODIES)]
        self.displayer=Displayer()


    def init_state(self):
        for i in range(self.NBODIES):
            self.bodies[i].mass=1

    def Bodies_to_Array(self,y):
        for i in range(self.NBODIES):
            State_to_Array(self.bodies[i],y[i*self.STATE_SIZE:])

    def Array_to_Bodies(self,y):
        for i in range(self.NBODIES):
            Array_to_State(self.bodies[i],y[i*self.STATE_SIZE:])

    def DxDt(self,x0, t, n):
        ret=np.zeros_like(x0)
        self.Array_to_Bodies(x0)
        for bi,b in enumerate(self.bodies):
            if bi==0:
                continue
            self.compute_force_and_torque(t,b)
            self.DDt_state_to_array(b,ret[bi*self.STATE_SIZE:])

        return ret

    def compute_force_and_torque(self,t,b):
        b.force=np.array([0,0,0])
        b.torque=np.array([0,0,1])
        #pass

    def DDt_state_to_array(self,b,x_dot):
        x_dot[0:3]=b.v
        Rdot=np.dot(self.vector_up_to_matrix(b.omega),b.R)
        x_dot[3:12]=Rdot.flatten()
        x_dot[12:15]=b.force
        x_dot[15:18]=b.torque

    def vector_up_to_matrix(self,v):
        return np.array([
           [0,-v[2],v[1]] ,
            [v[2], 0, -v[0]],
            [-v[1], v[0], 0]
        ])

    def run_simulation(self):
        x0=np.zeros(self.N_UNKNOWN_NUM)
        x_final=np.zeros_like(x0)

        self.init_state()
        self.Bodies_to_Array(x_final)


        t=0
        t_step=1./100
        while t<10.0:
            x0=np.copy(x_final)
            x_final=ode(x0,self.N_UNKNOWN_NUM,t,t_step,self.DxDt)
            self.Array_to_Bodies(x_final)
            self.displayer.display(self.bodies)
            t=t+t_step

        plt.show()

class RigidBodyTest(unittest.TestCase):

    def setUp(self) -> None :
        self.sim=Simulator()

    def test_RigidBody(self):
        rb = RigidBody()
        print(rb.mass)
        print(rb.X)
        print(rb.I_Body)

    def test_run_simulation(self):
        self.sim.run_simulation()




if __name__ == '__main__':
    unittest.main()
