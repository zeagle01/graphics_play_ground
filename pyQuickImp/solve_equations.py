
import matplotlib.pyplot as plt
import numpy as np


def Solving_Loop(method,A,b,x_0,iterations,err0):

    X = np.zeros((iterations + 1, len(x_0)))
    err = np.zeros(iterations+1)
    X[0] = x_0
    method.precompute(A,b,x_0)
    for i in range(iterations):
        X[i+1,:]=method.update(A,b,X[i,:])
        err[i]=np.linalg.norm(X[i+1,:]-X[i,:])
        if(err[i]<err0 or i==iterations-1):
            Xo=X[:i+1]
            err_o=err[:i+1]
            break
    return Xo,err_o


class Jacobi:
    def precompute(self,A,b,x_0):
        self.D = np.diag(np.diag(A))

    def update(self,A,b,x):
        return np.dot(np.linalg.inv(self.D),b-np.dot(A-self.D,x))


class Gauss_Seidel:
    def precompute(self,A,b,x_0):
        self.L = np.tril(A)
        self.U = A - self.L

    def update(self,A,b,x):
        return np.dot(np.linalg.inv(self.L),b-np.dot(self.U,x))


class Gradient_Descent:
    def precompute(self,A,b,x_0):
        pass

    def update(self,A,b,x):
        r=b-np.dot(A,x)
        alpha=np.dot(r,r)/np.dot(r,np.dot(A,r))
        return x+alpha*r

class Conjugate_Gradient:
    def precompute(self,A,b,x_0):
        self.r=b-np.dot(A,x_0)
        self.p=self.r

    def update(self,A,b,x):
        alpha=np.dot(self.r,self.r)/np.dot(self.p,np.dot(A,self.p))
        x=x+alpha*self.p
        r0=np.copy(self.r)
        self.r=self.r-alpha*np.dot(A,self.p)
        beta=np.dot(self.r,self.r)/np.dot(r0,r0)
        self.p=self.r+beta*self.p
        return x


A=np.array(
   [
#       [4,-1,1],
#       [4, -8, 1],
#       [-2, 1, 5],
       [5,2],
       [2, 3]
   ]
)

b=np.array(
#    [7,-21,15]
     [1,1]
)

#x_0=np.array([1,2,2])
x_0=np.array([0,0])

x=np.linalg.solve(A,b)

fig,ax=plt.subplots(1,2)
ax[1].set_yscale('log')

lambd,Evtor=np.linalg.eig(A)


max_iteration=1000
limit_err=1e-12

methods=[Jacobi(),Gauss_Seidel(),Gradient_Descent(),Conjugate_Gradient()]
names=['Jacobi','GS','GD','CG']
R=[]
for i in range(len(methods)):
    R.append(Solving_Loop(methods[i],A,b,x_0,max_iteration,limit_err))
    ax[0].plot(R[i][0][:,0],R[i][0][:,1],'-o',label=names[i])
    ax[1].plot(R[i][1],'-o',label=names[i])

ax[0].legend()
ax[1].legend()


plt.show()



