
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
        self.q=R.from_matrix(np.eye(3)).as_quat()
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

        ret=np.array([self.X,self.X+R.from_quat(self.q).as_matrix()[0],
                      self.X,self.X+R.from_quat(self.q).as_matrix()[1],
                      self.X,self.X+R.from_quat(self.q).as_matrix()[2]
                      ])
        return ret


def State_to_Array(rb,y):
    y[0:3]=rb.X[:]
    y[3:7]=rb.q
    y[7:10]=rb.P
    y[10:13]=rb.L


def Array_to_State(rb,y):
    rb.X=y[0:3]
    rb.q=y[3:7]
    rb.P=y[7:10]
    rb.L=y[10:13]

    rb.v=rb.P/rb.mass
    rb_R=R.from_quat(rb.q).as_matrix()
    rb.I_Inv=np.dot(rb_R,np.dot(rb.I_BodyInv,rb_R.transpose()))
    rb.omega=np.dot(rb.I_Inv,rb.L)








def ode(x0,n,t,t_step,DxDt):
    delta_x=DxDt(x0,t,n)*t_step
    x_final=x0+delta_x
    return x_final




def quaternion_multiply(quaternion1, quaternion0):
    w0, x0, y0, z0 = quaternion0
    w1, x1, y1, z1 = quaternion1
    return np.array([-x1 * x0 - y1 * y0 - z1 * z0 + w1 * w0,
                     x1 * w0 + y1 * z0 - z1 * y0 + w1 * x0,
                     -x1 * z0 + y1 * w0 + z1 * x0 + w1 * y0,
                     x1 * y0 - y1 * x0 + z1 * w0 + w1 * z0] )



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
        b.force=np.array([1,0,0])
        b.torque=np.array([0,0,1])
        #pass


    def DDt_state_to_array(self,b,x_dot):
        x_dot[0:3]=b.v
        omega_hat=np.array([0,*(b.omega)])
        x_dot[3:7]=0.5*quaternion_multiply(omega_hat,b.q)
        x_dot[7:10]=b.force
        x_dot[10:13]=b.torque


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
