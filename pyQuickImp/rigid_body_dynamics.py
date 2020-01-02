
import matplotlib.pyplot as plt
import numpy as np
import test



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




rb=RigidBody()
#print(rb.mass)
#print(rb.X)
#print(rb.I_Body)





