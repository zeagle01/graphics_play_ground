
import matplotlib.pyplot as plt
import numpy as np
import unittest



class RigidBody:
    def __init__(self):
        self.mass=0
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



def State_to_Array(rb,y):
    pass


def Array_to_State(rb,y):
    pass


def Array_to_Bodies(y):
    pass

def Bodies_to_Array(y):
    pass


def Compute_Force_and_Torque(t,rb):
    pass


def dydt(t,y,ydot):
    pass



class RigidBodyTest(unittest.TestCase):
    def test_RigidBody(self):
        rb = RigidBody()
        print(rb.mass)
        print(rb.X)
        print(rb.I_Body)




if __name__ == '__main__':
    unittest.main()
