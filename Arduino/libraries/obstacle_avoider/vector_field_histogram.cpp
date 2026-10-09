#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Print the distance array once using servo and hcsr04 to see which type of values 
// come and whether this distance array is continuous or discontinuous

// Also the angle sweeped by servo is not perfectly from 0-180 degree so check that also

// Figure out the threshold 

int findBestPath(int distances[],int threshold){
    // traverse the array to find which angles correspond to distances>threshold
    vector<int> validangles;
    for (int i=0;i<180;i++){
        if(distances[i]>threshold){
            validangles.push_back(i);
        }
    }
    int max_width=INT_MIN;
    int max_span=INT_MIN;
    // Find consecutive angles in validangles and then the max width
    int start=validangles[0];
    int end=validangles[0];
    for (int i=0;i<validangles.size()-1;i++){
        if(validangles[i+1]-validangles[i]==1){
            end=validangles[i+1];
        }
        else{
            start=validangles[i+1];
            end=validangles[i+1];
        }
        max_span=max(max_span,end-start);
    }
    // cosine rule
    max_width=sqrt((distances[start]*distances[start])+(distances[end]*distances[end])-(2*distances[start]*distances[end]*cos(max_span)));
    return max_width;
}

// int main(){
//     int distances[180];
//     for (int i=0;i<180;i++){
//         distances[i]=rand()%100;
//     }
//     int threshold=50;
//     cout<<findBestPath(distances,threshold)<<endl;
// }
