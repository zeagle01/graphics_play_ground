
import numpy as np



a=np.arange(24)
a=a.reshape((2,3,4))

b=np.arange(24)
b=b.reshape((4,3,2))

c0=np.einsum('ijk,lmn',a,b)
print(c0.shape)
#c1=np.einsum('...jk,...mn',a,b)
#print(c1.shape)

