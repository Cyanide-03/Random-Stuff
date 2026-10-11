#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Print the distance array once using servo and hcsr04 to see which type of values 
// come and whether this distance array is continuous or discontinuous

// Also the angle sweeped by servo is not perfectly from 0-180 degree so check that also

// Figure out the threshold, robot width

// turndelay factor in pivoturn (Arduino IDE) has to be calculated by testing

int findBestPath(int distances[],int threshold,float robot_width){
    // traverse the array to find which angles correspond to distances>threshold
    vector<int> validangles;
    for (int i=40;i<=150;i++){
        if(distances[i]>threshold){
            validangles.push_back(i);
        }
    }
    int start=validangles[0];
    int end=validangles[0];
    float max_span=-1.0;
    int best_start=-1;
    int best_end=-1;
    // Find consecutive angles in validangles and then the max width
    for (int i=0;i<validangles.size()-1;i++){
        if(validangles[i+1]-validangles[i]==1){
            end=validangles[i+1];
        }
        else{
            float current_span=(end-start)*(M_PI/180);
            float r1=distances[start];
            float r2=distances[end];
            float w=sqrt((r1*r1)+(r2*r2)-(2*r1*r2*cos(current_span)));
            if(w>robot_width && current_span>max_span){
                best_start=start;
                best_end=end;
                max_span=current_span;
            }
            start=validangles[i+1];
            end=validangles[i+1];
        }
    }
    float current_span=(end-start)*(M_PI/180);
    float r1=distances[start];
    float r2=distances[end];
    float w=sqrt((r1*r1)+(r2*r2)-(2*r1*r2*cos(current_span)));
    if(w>robot_width && current_span>max_span){
        best_start=start;
        best_end=end;
        max_span=current_span;
    }
    return (best_start+best_end)/2;
}

// int main(){
//     float distances[180];
//     for (int i=0;i<180;i++){
//         distances[i]=rand()%100;
//     }
//     int threshold=50;
//     cout<<findBestPath(distances,threshold,10)<<endl;
// }
