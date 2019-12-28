
import matplotlib.pyplot as plt
import numpy as np



A=np.array(
   [
       [4,-1,1],
       [4, -8, 1],
       [-2, 1, 5],
   ]
)

b=np.array(
    [7,-21,15]
)

x=np.linalg.solve(A,b)

lambd,Evtor=np.linalg.eig(A)






def Solving_Loop(method,A,b,x_0,iterations,err0):

    X = np.zeros((iterations + 1, len(x_0)))
    err = np.zeros(iterations+1)
    X[0] = x_0
    method.precompute(A,b)
    for i in range(iterations):
        X[i+1,:]=method.update(A,b,X[i,:])
        err[i]=np.linalg.norm(X[i+1,:]-X[i,:])
        if(err[i]<err0 or i==iterations-1):
            Xo=X[:i+1]
            err_o=err[:i+1]
            break
    return Xo,err_o


class Jacobi:
    def precompute(self,A,b):
        self.D = np.diag(np.diag(A))

    def update(self,A,b,x):
        return np.dot(np.linalg.inv(self.D),b-np.dot(A-self.D,x))


class Gauss_Seidel:
    def precompute(self,A,b):
        self.L = np.tril(A)
        self.U = A - self.L

    def update(self,A,b,x):
        return np.dot(np.linalg.inv(self.L),b-np.dot(self.U,x))


class Gradient_Descent:
    def precompute(self,A,b):
        pass

    def update(self,A,b,x):
        return x-0.01*(b-np.dot(A,x))

x_0=np.array([1,2,2])
max_iteration=100
limit_err=1e-12
X,err=Solving_Loop(Jacobi(),A,b,x_0,max_iteration,limit_err)
X1,err1=Solving_Loop(Gauss_Seidel(),A,b,x_0,max_iteration,limit_err)
#X1,err1=Solving_Loop(Gradient_Descent(),A,b,x_0,max_iteration,limit_err)

fig,ax=plt.subplots(1,2)
ax[0].plot(X[:,0],X[:,1],'-o')
ax[0].plot(X1[:,0],X1[:,1],'-o')
#ax[0].plot(X,'-o')
#ax[0].plot(X1,'-o')
ax[1].set_yscale('log')
ax[1].plot(err,'-o')
ax[1].plot(err1,'-o')

plt.show()



