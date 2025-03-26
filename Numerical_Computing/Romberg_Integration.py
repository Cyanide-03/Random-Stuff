import numpy as np

def f(x):
    return 4/(1+x*x)

def romberg(f,n):
    R=np.zeros((n,n))
    a=0
    b=1
    h=b-a
    R[0][0]=(h/2)*(f(a)+f(b))
    for i in range(1,n):
        h=h/2
        sum=R[i-1][0]/2
        for k in range(1,2**(i-1)+1):
            sum+=h*f(a+(2*k-1)*h)

        R[i][0]=sum

        for j in range(1,i+1):
            R[i][j]=R[i][j-1]+(R[i][j-1]-R[i-1][j-1])/(4**j-1)
    
    for row in R:
        print(row)

romberg(f,10)

    

    