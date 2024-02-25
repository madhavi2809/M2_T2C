#include <iostream>
#include <vector>
#include <pthread.h>

using namespace std;

// Structure to pass arguments to the thread function
struct ThreadArgs {
    vector<int>& arr;
    int low;
    int high;
};

// Partition function to divide the array into two parts
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
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
void* quicksort(void* arg) {
    ThreadArgs* args = static_cast<ThreadArgs*>(arg);
    int low = args->low;
    int high = args->high;
    vector<int>& arr = args->arr;
    if (low < high) {
        int pi = partition(arr, low, high);
        
        // Create arguments for left and right partitions
        ThreadArgs leftArgs = {arr, low, pi - 1};
        ThreadArgs rightArgs = {arr, pi + 1, high};

        // Create threads for left and right partitions
        pthread_t leftThread, rightThread;
        pthread_create(&leftThread, NULL, quicksort, &leftArgs);
        pthread_create(&rightThread, NULL, quicksort, &rightArgs);

        // Wait for threads to finish
        pthread_join(leftThread, NULL);
        pthread_join(rightThread, NULL);
    }
    pthread_exit(NULL);
}

int main() {
    vector<int> arr = {10, 7, 8, 9, 1, 5};
    int n = arr.size();

    // Create arguments for the main partition
    ThreadArgs args = {arr, 0, n - 1};

    // Create thread for the main partition
    pthread_t mainThread;
    pthread_create(&mainThread, NULL, quicksort, &args);

    // Wait for the main thread to finish
    pthread_join(mainThread, NULL);

    cout << "Sorted array: ";
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";
    cout << endl;

    return 0;
}
