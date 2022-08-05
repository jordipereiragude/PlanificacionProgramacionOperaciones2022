import sys
import numpy as np

#instancia
n=0
c=0
profit=[]
weight=[]

if len(sys.argv)<2:
    print("fichero de entrada")
with open(sys.argv[1]) as f:
    lines = f.readlines()
    n=(int)(lines[0].split(" ")[0].strip())
    c=(int)(lines[0].split(" ")[1].strip())
    for i in range(1,len(lines)):
        weight.append((int)(lines[i].split(" ")[0].strip()))
        profit.append((int)(lines[i].split(" ")[1].strip()))
print(n,c,len(profit))

p=np.zeros((n,c+1)).astype('int32')
d=np.zeros((n,c+1)).astype('int32')
p[0][weight[0]]=profit[0]
d[0][weight[0]]=1
for i in range(1,n):
    for j in range(weight[i]):
        p[i][j]=p[i-1][j]
        d[i][j]=0
    for j in range(weight[i],c+1):
        p[i][j]=p[i-1][j]
        d[i][j]=0
        if (p[i-1][j-weight[i]]+profit[i])>p[i][j]:
            p[i][j]=p[i-1][j-weight[i]]+profit[i]
            d[i][j]=1
obj=0
currentWeight=0
for i in range(c+1):
    if obj<p[n-1][i]:
        obj=p[n-1][i]
        currentWeight=i
print("objective",obj)
for i in range(n-1,-1,-1): #va de n-1 a 0
    if d[i][currentWeight]==1:
        print("use item",i,"profit",profit[i])
        currentWeight=currentWeight-weight[i]

