#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>

using namespace std;
using namespace chrono;

// Partition function to divide the array into two parts
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    #pragma omp parallel for
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

// Quicksort function
void quicksort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        #pragma omp task
        quicksort(arr, low, pi - 1);
        #pragma omp task
        quicksort(arr, pi + 1, high);
    }
}

int main() {
    vector<int> arr = {10, 7, 8, 9, 1, 5};
    int n = arr.size();

    // Start measuring execution time
    auto start = high_resolution_clock::now();

    // Perform quicksort with OpenMP parallelization
    #pragma omp parallel
    {
        #pragma omp single
        quicksort(arr, 0, n - 1);
    }

    // Stop measuring execution time
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    // Print sorted array
    cout << "Sorted array: ";
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";
    cout << endl;

    // Print execution time
    cout << "Execution time: " << duration.count() << " microseconds" << endl;

    return 0;
}
