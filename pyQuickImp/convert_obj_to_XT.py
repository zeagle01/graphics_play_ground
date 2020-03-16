

X=[]
T=[]

with open("D:\sprojs\one_layer_button.obj") as fin:
    lines=fin.readlines()
    for line in lines:
       words=line.split(" ")
       if words[0]=='v':
           x=[float(words[1]),float(words[2]),float(words[3])]
           X.append(x)
       if words[0]=='f':
           t=[]
           for i in range(3):
               tv=words[i+1]
               tv=tv.split('/')
               t.append(int(tv[0]))
           T.append(t)


def float_to_str(number):
    #return '{:.8f}'.format(number)
    return str(number)+'f'

with open("D:\sprojs\one_layer_button_out.obj",'w') as fout:
    for x in X:
        fout.write('{'+float_to_str(x[0])+','+float_to_str(x[1])+','+float_to_str(0.0)+'},\n')
    for x in X:
        fout.write('{'+float_to_str(x[0])+','+float_to_str(x[1])+'},\n')
    for t in T:
        fout.write('{'+str(t[0]-1)+','+str(t[1]-1)+','+str(t[2]-1)+'},\n')

