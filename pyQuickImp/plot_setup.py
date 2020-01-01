import matplotlib.pyplot as plt
import numpy as np


data=np.array([[0,0],[0,1],[1,1]])
fig,ax=plt.subplots(1,2)
ax[0].fill(data[:,0],data[:,1],fill=False)
data=np.array([[0,0],[1,0],[1,1],[0,1]])
t=[[0,1,2],[0,2,3]]
ax[1].fill(data[t[0],0],data[t[0],1],fill=False)
ax[1].fill(data[t[1],0],data[t[1],1],fill=False)

plt.show()
