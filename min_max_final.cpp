#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>
#include <cstdlib>
using namespace std;
using namespace std::chrono;

int minvalp(vector<int>& arr) {
    int minval = arr[0];
    #pragma omp parallel for reduction(min:minval)
    for (int i = 1; i < arr.size(); i++) {
        if (arr[i] < minval) {
            minval = arr[i];
        }
    }
    return minval;
}

int minvals(vector<int>& arr) {
    int minval = arr[0];
    for (int i = 1; i < arr.size(); i++) {
        if (arr[i] < minval) {
            minval = arr[i];
        }
    }
    return minval;
}

int maxvalp(vector<int>& arr) {
    int maxval = arr[0];
    #pragma omp parallel for reduction(max:maxval)
    for (int i = 1; i < arr.size(); i++) {
        if (arr[i] > maxval) {
            maxval = arr[i];
        }
    }
    return maxval;
}

int maxvals(vector<int>& arr) {
    int maxval = arr[0];
    for (int i = 1; i < arr.size(); i++) {
        if (arr[i] > maxval) {
            maxval = arr[i];
        }
    }
    return maxval;
}

int sump(vector<int>& arr) {
    int sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < arr.size(); i++) {
        sum += arr[i];
    }
    return sum;
}

int sums(vector<int>& arr) {
    int sum = 0;
    for (int i = 0; i < arr.size(); i++) {
        sum += arr[i];
    }
    return sum;
}

int avgp(vector<int>& arr) {
    return sump(arr) / arr.size();
}

int avgs(vector<int>& arr) {
    return sums(arr) / arr.size();
}

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;
    vector<int> arr(n);

    srand(time(0));
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 10000; // Generate random numbers between 0 and 9999
    }

    // cout << "Array: ";
    // for (int i = 0; i < n; i++) {
    //     cout << arr[i] << " ";
    // }
    cout << endl;

    double starttime, endtime;

    starttime = omp_get_wtime();
    cout << "Minimum value: " << minvalp(arr) << endl;
    endtime = omp_get_wtime();
    cout << "Parallel Time: " << (endtime - starttime) << endl;

    starttime = omp_get_wtime();
    cout << "Minimum value: " << minvals(arr) << endl;
    endtime = omp_get_wtime();
    cout << "Sequential Time: " << (endtime - starttime) << endl;

    starttime = omp_get_wtime();
    cout << "Maximum value: " << maxvalp(arr) << endl;
    endtime = omp_get_wtime();
    cout << "Parallel Time: " << (endtime - starttime) << endl;

    starttime = omp_get_wtime();
    cout << "Maximum value: " << maxvals(arr) << endl;
    endtime = omp_get_wtime();
    cout << "Sequential Time: " << (endtime - starttime) << endl;

    starttime = omp_get_wtime();
    cout << "Sum: " << sump(arr) << endl;
    endtime = omp_get_wtime();
    cout << "Parallel Time: " << (endtime - starttime) << endl;

    starttime = omp_get_wtime();
    cout << "Sum: " << sums(arr) << endl;
    endtime = omp_get_wtime();
    cout << "Sequential Time: " << (endtime - starttime) << endl;

    starttime = omp_get_wtime();
    cout << "Average value: " << avgp(arr) << endl;
    endtime = omp_get_wtime();
    cout << "Parallel Time: " << (endtime - starttime) << endl;

    starttime = omp_get_wtime();
    cout << "Average value: " << avgs(arr) << endl;
    endtime = omp_get_wtime();
    cout << "Sequential Time: " << (endtime - starttime) << endl;
}
