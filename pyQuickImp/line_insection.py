
import numpy as np
import matplotlib.pyplot as plt
import heapq


configure = {
    "line":[[-0.5,-0.5],[0,0.5],[-0.4,0.1],[0.6,-0.1]]
}



class My_Heap:
    def __init__(self,initial=None,key=lambda x:x):
    #def __init___(self,initial,key):
        self.key=key
        self.index=0
        if initial:
            self.data=[(key(item),i,item) for i,item in enumerate(initial)]
            self.index=len(self.data)
            heapq.heapify(self.data)
        else:
            self.data=[]

    def push(self,item):
        heapq.heappush(self.data,(self.key(item),self.index,item))
        self.index+=1

    def pop(self):
        self.index-=1
        return heapq.heappop(self.data)[2]


def y_at_x(l,x):
    ly=[l.p0.y,l.p1.y]
    lx=[l.p0.x,l.p1.x]
    return (ly[1]-ly[0])/(lx[1]-lx[0])*(x-lx[0])

def line_y_less_at_x(l0,l1,x):
    return y_at_x(l0,x)<y_at_x(l1,x)

def line_segment_intersect(line_pair):
    l0x0=np.array([line_pair[0].p0.x,line_pair[0].p0.y])
    l0x1=np.array([line_pair[0].p1.x,line_pair[0].p1.y])
    l1x0=np.array([line_pair[1].p0.x,line_pair[1].p0.y])
    l1x1=np.array([line_pair[1].p1.x,line_pair[1].p1.y])

    a=np.concatenate(((l0x1-l0x0)[np.newaxis,:],(l1x0-l1x1)[np.newaxis,:]))
    b=l1x0-l0x0
    inv_at=np.linalg.inv(a.transpose())
    t=np.dot(inv_at,b)
    is_intersect=np.all(t<1.) and np.all(t>0.)

    point=l0x0+t[0]*(l0x1-l0x0)
    return is_intersect,point


class Active_Lines:
    def __init__(self):
        self.lines=[]
        self.current_li=0

    def swap(self,line_pair):
        pass

    def insert(self,line,x):
        if len(self.lines)==0:
            self.lines.append(line)
            self.current_li = 0
        else:
            lines_origin=self.lines.copy()
            for li,l in enumerate(lines_origin):
                if line_y_less_at_x(line,l,x):
                    self.lines.insert(li,line)
                    self.current_li=li

    def get_current_pairs(self):
        ret=[]
        if self.current_li>1:
            ret.append((self.lines[self.current_li-1],self.lines[self.current_li]))
        if self.current_li<len(self.lines)-1:
            ret.append((self.lines[self.current_li],self.lines[self.current_li+1]))
        return ret

    def get_lines(self):
        return self.lines


class Line_Intersection:
    class Point:
        def __init__(self, x, y):
            self.x = x
            self.y = y
            self.l=[]

    class Line:
        def __init__(self,p0,p1):
            self.p0=p0
            self.p1=p1

        def get_plot_data(self):
            return np.array([[self.p0.x,self.p1.x],[self.p0.y,self.p1.y]])



    def __init__(self,configure):
        self.configure=configure
        subplots=plt.subplots(1,1)
        self.fig=subplots[0]
        self.axes=subplots[1]
        points = np.array(self.configure['line'])
        line_num=points.shape[0]//2
        self.raw_points=points.reshape((line_num,2,2))

    def plot_lines(self):
        for line in self.raw_points:
            self.axes.plot(line[:,0],line[:,1],'-o',c=[0,0,1,0.3])

    def plot_active_lines(self):
        for active_line in self.active_lines.get_lines():
            self.axes.plot(*active_line.get_plot_data(), '-', c='r')

    def plot_line_pair(self,line_pair):
        self.axes.plot(*line_pair[0].get_plot_data(), '-', c='g')
        self.axes.plot(*line_pair[1].get_plot_data(), '-', c='g')

    def prepare_event_list(self):
        self.event_list=[]
        for line in self.raw_points:
            p0=self.Point(*line[0])
            p1=self.Point(*line[1])
            l=self.Line(p0,p1)
            p0.l.append(l)
            p1.l.append(l)
            self.event_list.append(p0)
            self.event_list.append(p1)
        self.event_list=My_Heap(self.event_list,lambda i:i.x)



    def find_intersection(self):
        self.prepare_event_list()
        self.y_max=np.max(self.raw_points[:,:,0])
        self.y_min=np.min(self.raw_points[:,:,0])
        self.active_lines=Active_Lines()
        while(self.event_list.index>0):
            self.axes.cla()
            self.plot_lines()
            event_point=self.event_list.pop()
            self.axes.scatter(event_point.x,event_point.y,c='r')
            self.axes.plot([event_point.x,event_point.x,],[self.y_min,self.y_max],c='r')
            event_lines=event_point.l
            if len(event_lines)==1:
                event_line=event_lines[0]
                if event_line.p0 is event_point:
                    self.active_lines.insert(event_line,event_point.x)
            elif len(event_lines)==2:
                self.active_lines.swap(event_lines)
                continue

            self.plot_active_lines()
            line_paris=self.active_lines.get_current_pairs()
            for line_pair in line_paris:
                self.plot_line_pair(line_pair)
                is_intersect,intersection_point=line_segment_intersect(line_pair)
                if is_intersect and intersection_point[0]>event_point.x:
                    p = self.Point(*intersection_point)
                    p.l.append(line_pair[0])
                    p.l.append(line_pair[1])
                    self.event_list.push(p)
                    pass









li=Line_Intersection(configure)
li.find_intersection()
plt.show()
