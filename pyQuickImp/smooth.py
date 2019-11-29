import numpy as np
import matplotlib.pyplot as plt

fig,ax=plt.subplots(2,1)
ax=np.array(ax)
ax=ax.reshape(-1)


n=100
x=np.linspace(0,np.pi,100)
y0=np.sin(x)
ax[0].plot(x,y0,label="origin")
ax[1].plot(x,y0,label="origin")
y=y0+np.random.rand(n)-0.5
ax[0].plot(x,y,label="y_noised")
ax[1].plot(x,y,label="y_noised")


x_conv=np.linspace(0,np.pi,n)

kernel=np.array([1./3,1./3,1./3])
y1=np.convolve(y,kernel)[1:-1]
ax[0].plot(x_conv,y1,label='y1')
for i in range(9):
    y1 = np.convolve(y1, kernel)[1:-1]
ax[1].plot(x_conv,y1,label='y1_10')


kernel=np.array([1./2,0,1./2])
y2=np.convolve(y,kernel)[1:-1]
ax[0].plot(x_conv,y2,label='y2')
for i in range(9):
    y2 = np.convolve(y2, kernel)[1:-1]
ax[1].plot(x_conv,y2,label='y2_10')

ax[0].legend()
ax[1].legend()
plt.show()
