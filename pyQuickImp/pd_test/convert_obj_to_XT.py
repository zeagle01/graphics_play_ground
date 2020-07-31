
import numpy as np

def float_to_str(number):
    #return '{:.8f}'.format(number)
    #return str(number)+'f'
    return str(number)





def read_obj(file):
    X = []
    T = []
    with open(file) as fin:
        lines = fin.readlines()
        for line in lines:
            words = line.split(" ")
            if words[0] == 'v':
                x = [float(words[1]), float(words[2]), float(words[3])]
                X.append(x)
            if words[0] == 'f':
                t = []
                for i in range(3):
                    tv = words[i + 1]
                    tv = tv.split('/')
                    t.append(int(tv[0])-1)
                T.append(t)
    return X,T


def write_obj(file,X,T):
    with open(file, 'w') as fout:
        for x in X:
            fout.write('v ' + float_to_str(x[0]) + ' ' + float_to_str(x[1]) + ' ' +float_to_str(x[2]) +'\n')
        for t in T:
            fout.write('f ' + str(t[0] + 1) + ' ' + str(t[1] + 1) + ' ' + str(t[2] + 1) + '\n')



def generate_triangle_stride(b,e):
    for i in range(b,e):
        n=2**i
        X=np.zeros((n+2,3))
        X[0,0]=0
        X[0,1]=0.5
        X[1:,0]=np.linspace(-0.3,0.3,n+1)
        X[1:,1]=0
        X[:,2]=0

        T=[]
        for ti in range(n):
            t=[0,ti+1,ti+2]
            T.append(t)
        T=np.array(T)
        write_obj('triangle_stride'+str(n)+'.obj',X,T)


#generate_triangle_stride(1,6)
