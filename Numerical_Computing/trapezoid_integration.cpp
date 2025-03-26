#include <iostream>
#include <bits/stdc++.h>
using namespace std;

float f(float x){
    return 4/(1+x*x);
}

int main(){
    float a=0,b=1;
    float h=(b-a)/1000;
    float sum=(f(a)+f(b))/2;
    for(int i=1;i<1000;i++){
        sum+=f(a+i*h);
    }
    float ans=sum*h;
    cout<<ans<<endl;
}