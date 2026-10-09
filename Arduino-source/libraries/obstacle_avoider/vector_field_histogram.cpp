#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Print the distance array once using servo and hcsr04 to see which type of values 
// come and whether this distance array is continuous or discontinuous

// Also the angle sweeped by servo is not perfectly from 0-180 degree so check that also

// Figure out the threshold 

int findBestPath(float distances[],int threshold,float robot_width){
    // traverse the array to find which angles correspond to distances>threshold
    vector<int> validangles;
    for (int i=0;i<180;i++){
        if(distances[i]>threshold){
            validangles.push_back(i);
        }
    }
    int max_span=INT_MIN;
    // Find consecutive angles in validangles and then the max width
    int start=validangles[0];
    int end=validangles[0];
    int best_start=0;
    int best_end=0;
    for (int i=0;i<validangles.size()-1;i++){
        if(validangles[i+1]-validangles[i]==1){
            end=validangles[i+1];
        }
        else{
            int current_span=(end-start)*(M_PI/180);
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
    return (best_start+best_end)/2;
}

// int main(){
//     int distances[180];
//     for (int i=0;i<180;i++){
//         distances[i]=rand()%100;
//     }
//     int threshold=50;
//     cout<<findBestPath(distances,threshold)<<endl;
// }
