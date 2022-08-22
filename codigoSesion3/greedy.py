import sys

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
ratio=[]
orden=[]
for i in range(len(profit)):
    orden.append(i)
    ratio.append(0.0-profit[i]/weight[i])  #orden al revés
#orden=[x for _,x in sorted(zip(ratio,orden))] #esto hace quicksort
#""" esto hace bubblesort
while True:
    cambio=False
    for i in range(len(orden)-1):
        if ratio[orden[i]]>ratio[orden[i+1]]:
            temporal=orden[i]
            orden[i]=orden[i+1]
            orden[i+1]=temporal
            cambio=True
    if cambio==False:
        break
#"""

"""
print(orden)
for i in range(len(orden)):
    print(orden[i],ratio[orden[i]])
"""
libre=c
obj=0
print("usa:",end=" ")
for i in range(len(orden)):
    if libre>=weight[orden[i]]:
        libre -= weight[orden[i]]
        obj += profit[orden[i]]
        print(orden[i],end=" ")
print("\nbeneficio total:",obj);

    

