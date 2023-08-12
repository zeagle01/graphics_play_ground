
import matplotlib.pyplot as plt
import numpy as np



fig,ax=plt.subplots(1)

nx=100
ny=100
gx=np.linspace(-10,10,nx)
gy=np.linspace(-10,10,ny)
M=np.meshgrid(gx,gy)
#z=np.power((np.sqrt(x*x+y*y)-1),2)

#e=|x-x0|-1
#E'=E0+dE dx
#dE=(f + H dx)dx

l0=1
k=1

x0=np.array(
    [0,0]
)

x=np.array(
    [0.5,0]
)

length=np.linalg.norm(x-x0)
n=(x-x0)/length

f=k*(np.linalg.norm(x-x0)-l0)*n

P=np.einsum('i,j',n,n)

H=k*P+k*(1-l0/length)*(np.eye(2)-P)

z=np.einsum('imn,ij,jmn->mn',M,H,M)+np.einsum('imn,i',M,f)


im=ax.imshow(z,origin='lower',extent=[gx.min(),gx.max(),gy.min(),gy.max()],cmap='coolwarm')

cont=ax.contour(gx,gy,z)

ax.clabel(cont) #with value line

fig.colorbar(im,ax=ax)

plt.show()

