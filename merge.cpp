#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>
#include <cstdlib>
using namespace std;
using namespace std::chrono;

void merge(vector<int> &arr, int low, int mid, int high) {
    vector<int> temp;
    int left = low;
    int right = mid + 1;

    while (left <= mid && right <= high) {
        if (arr[left] <= arr[right]) {
            temp.push_back(arr[left]);
            left++;
        } else {
            temp.push_back(arr[right]);
            right++;
        }
    }

    while (left <= mid) {
        temp.push_back(arr[left]);
        left++;
    }

    while (right <= high) {
        temp.push_back(arr[right]);
        right++;
    }

    for (int i = low; i <= high; i++) {
        arr[i] = temp[i - low];
    }
}

void mergeSort(vector<int> &arr, int low, int high) {
    if (low < high) {
        int mid = low + (high - low) / 2;
        mergeSort(arr, low, mid);
        mergeSort(arr, mid + 1, high);
        merge(arr, low, mid, high);
    }
}

void parallelMergeSort(vector<int> &arr, int low, int high) {
    if (low < high) {
        int mid = low + (high - low) / 2;

        if ((high - low) < 1000) {
            mergeSort(arr, low, high);
            return;
        }

        #pragma omp parallel
        {
            #pragma omp single nowait
            {
                #pragma omp task
                parallelMergeSort(arr, low, mid);
                #pragma omp task
                parallelMergeSort(arr, mid + 1, high);
                #pragma omp taskwait
                merge(arr, low, mid, high);
            }
        }
    }
}

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n), brr(n);
    srand(time(0));

    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 10000;
        brr[i] = arr[i];
    }

    // cout<<"Original Array : ";
    // for(int i=0;i<n;i++)
    // {
    //     cout<<arr[i]<<" ";
    // }
    auto start_time = omp_get_wtime();
    mergeSort(arr, 0, n - 1);
    auto end_time = omp_get_wtime();
    double sequential_time = (end_time - start_time);

    cout << "\nTime taken by Sequential Merge Sort: " << sequential_time << " seconds\n";
    // cout << "Sorted array (Sequential): ";
    // for (int num : arr) cout << num << " ";
    cout << "\n";

    arr = brr;
    // cout<<"Original Array : ";
    // for(int i=0;i<n;i++)
    // {
    //    cout<< arr[i]<<" ";
    // }
    cout<<endl;
    start_time = omp_get_wtime();
    parallelMergeSort(arr, 0, n - 1);
    end_time = omp_get_wtime();
    double parallel_time = (end_time - start_time);

    cout << "Time taken by Parallel Merge Sort: " << parallel_time << " seconds\n";
    // cout << "Sorted array (Parallel): ";
    // for (int num : arr) cout << num << " ";
    cout << "\n";

    return 0;
}
