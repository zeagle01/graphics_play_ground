
import numpy as np
import matplotlib.pyplot as plt
import bisect

plt.ion()

n=3
lines=np.random.random((n,2,2))


fig,ax=plt.subplots(1,1)

for i in range(n):
    ax.plot(lines[i,:,0],lines[i,:,1],'-o')
    #plt.pause(1)


class event_point:
    def __init__(self,type,p,l):
        self.type=type;
        self.point=p
        self.line=l

event_points=[]
for i,line in enumerate(lines):
    event_points.append(event_point('begin',line[0],i))
    event_points.append(event_point('end',line[1],i))


event_points=sorted(event_points,key=lambda event_point:event_point.point[0])


def find_bisect(v,l,nb,ne,key):
    i=(nb+ne)//2
    if v<key(l[i]):
        find_bisect(nb,i)
    elif v==key(l[i]):
        return i
    else:
        find_bisect(i+1,ne)

    return -1


sweep_line_status=[]
while len(event_points)!=0:
    event_point=event_points[0]
    sweep_line_x=event_point.point[0]

    ## debug
    sweep_line=np.array([
        [sweep_line_x,0],
        [sweep_line_x, 1]
    ])
    ax.plot(sweep_line[:,0],sweep_line[:,1])

    event_point_interset_y=event_point.point[1]

    find_bisect()

    sweep_line_status.append([event_point_interset_y,event_point])
    sweep_line_status=sorted(sweep_line_status,key=lambda r:r[0])
    #sweep_line_status=bisect.insort_left(sweep_line_status,event_point_interset_y)



    event_points=event_points[1:len(event_points)]


