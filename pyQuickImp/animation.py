
import matplotlib.pyplot as plt
import numpy as np



plt.ion()


fig,ax=plt.subplots(1,1)

X=[[],[]]

for i in range(100):
    plt.cla() #clear
    X[0].append(i)
    X[1].append(i*i)
    ax.plot(X[0],X[1])
    plt.pause(0.1) #pause for a while


plt.ioff()
plt.show()

