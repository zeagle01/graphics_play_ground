import numpy as np
import matplotlib.pyplot as plt



x=np.linspace(0,1,100)
y=np.power(10,6*(x-1))
y1=np.power(x,2)
y2=np.power(x,10)



fig,ax=plt.subplots(1,1)
ax=np.array(ax)
ax=ax.flat
ax[0].plot(x,y,label='$e^x$')
ax[0].plot(x,y1,label='$x^2$')
ax[0].plot(x,y2,label='$x^{10}$')

ax[0].legend()
plt.show()