import numpy as np
import matplotlib.pyplot as plt

fig,ax=plt.subplots(1,1)

T=20
h=2.1
k=1
y0=1


x=[0]
explicit_y=[y0]
explicit_y_stable=[y0]
implicit_y=[y0]

explicit_y_stable_delta=0
i=0
while i*h<T:
    x.append((i+1)*h)
    explicit_y_new=(1-h*k)*explicit_y[i]
    explicit_y_stable_new=(1-h*k)*explicit_y_stable[i]
    explicit_y_stable_delta_new=explicit_y_stable_new-explicit_y_stable[i]
    if np.abs(explicit_y_stable_delta_new)>np.abs(explicit_y_stable_delta) and np.abs(explicit_y_stable_delta)>0:
        sign=1 if explicit_y_stable_delta_new>0 else -1
        explicit_y_stable_new=explicit_y_stable[i]+sign*np.abs(explicit_y_stable_delta)*0.6

    explicit_y_stable.append(explicit_y_stable_new)
    explicit_y_stable_delta=explicit_y_stable_delta_new


    implicit_y_new=implicit_y[i]/(1+h*k)

    explicit_y.append(explicit_y_new)
    implicit_y.append(implicit_y_new)
    i=i+1

x=np.array(x)
explicit_y=np.array(explicit_y)
explicit_y_stable=np.array(explicit_y_stable)
implicit_y=np.array(implicit_y)
ax.plot(x,explicit_y,label='explicit')
ax.plot(x,implicit_y,label='implicit')
ax.plot(x,explicit_y_stable,label='control')

plt.legend()
plt.show()






