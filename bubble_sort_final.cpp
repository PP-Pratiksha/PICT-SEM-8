#include<iostream>
#include<omp.h>
#include<vector>
#include<chrono>
using namespace std;
using namespace std::chrono;

void bubble_seq(vector<int>&arr)
{
    int n=arr.size();
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n-i-1;j++)
        {
            if(arr[j]>arr[j+1])
            {
                swap(arr[j],arr[j+1]);
            }
        }
    }
}

void bubble_par(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n; i++) {
        if (i % 2 == 0) {
            // Even phase
            #pragma omp parallel for
            for (int j = 0; j < n - 1; j += 2) {
                if (arr[j] > arr[j + 1]) {
                    swap(arr[j], arr[j + 1]);
                }
            }
        } else {
            // Odd phase
            #pragma omp parallel for
            for (int j = 1; j < n - 1; j += 2) {
                if (arr[j] > arr[j + 1]) {
                    swap(arr[j], arr[j + 1]);
                }
            }
        }
    }
}


void print(vector<int> temp)
{
    for(int i=0;i<temp.size();i++)
    {
        cout<<temp[i]<<" ";
    }
    cout<<endl;
}

int main()
{
    int n;
    cout<<"Enter number of elements: ";
    cin>>n;
    
    // Generate random numbers for testing
    vector<int>arr(n);
    vector<int>brr(n);
    
    for(int i=0;i<n;i++)
    {
        arr[i] = rand() % 1000 + 1;  // Random numbers between 1 and 1000
        brr[i] = arr[i];
    }
    // cout<<"Original unsorted arrays (Sequential) : "<<endl;
	// print(arr);
	// cout<<"Original unsorted arrays (Parallel) : "<<endl;
	// print(brr);

    // Sequential Sort Timing
    auto start = omp_get_wtime();
    bubble_seq(arr);
    auto stop = omp_get_wtime();
    auto duration = (stop - start);
    cout<<"Sequential time of Bubble: "<<duration << " seconds"<<endl;
    // print(arr);

    // Parallel Sort Timing
    start = omp_get_wtime();
    bubble_par(brr);
    stop = omp_get_wtime();
    duration = (stop - start);
    cout<<"Parallel time of Bubble: "<<duration << " seconds"<<endl;
    // print(brr);
    
    return 0;
}