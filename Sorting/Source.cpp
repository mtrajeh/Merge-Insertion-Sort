#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>
using namespace std;

vector<int> generateRandomVector(int size) {
    vector<int> result;
    for (int i = 0; i < size; ++i) {
        result.push_back(rand() % 100);
    }
    return result;
}

void insertionSort(vector<int>& a) {
    int n = a.size();
    for (int i = 1; i < n; i++) {
        int tmp = a[i];
        int j;
        for (j = i; j > 0 && tmp < a[j - 1]; j--) {
            a[j] = a[j - 1];
        }
        a[j] = tmp;
    }
}

void merge(vector<int>& arr, int first, int mid, int last) {
    int size = last - first + 1;
    vector<int> tempArr(size);

    int first1 = first;
    int last1 = mid;
    int first2 = mid + 1;
    int last2 = last;
    int index = first1;

    for (; (first1 <= last1) && (first2 <= last2); ++index) {
        if (arr[first1] < arr[first2])
            tempArr[index] = arr[first1++];
        else
            tempArr[index] = arr[first2++];
    }

    while (first1 <= last1) {
        tempArr[index++] = arr[first1++];
    }

    while (first2 <= last2) {
        tempArr[index++] = arr[first2++];
    }

    for (index = first; index <= last; ++index)
        arr[index] = tempArr[index];
}

void mergeInsertionSort(vector<int>& arr, int first, int last, int threshold) {
    if (first < last) {
        int size = last - first + 1;

        if (size <= threshold) {
            insertionSort(arr);
        }
        else {
            int mid = (first + last) / 2;
            mergeInsertionSort(arr, first, mid, threshold);
            mergeInsertionSort(arr, mid + 1, last, threshold);
            merge(arr, first, mid, last);
        }
    }
}

int main() {
    vector<int> inputSizes = { 250, 500, 1000, 2000, 4000, 8000,16000, 32000,64000, 128000, 256000, 512000, 1024000, 2048000, 4096000, 8192000, 16384000, 32768000, 65536000, 131072000 };

    double M_prev = 0.0;
    double I_prev = 0.0;
    double MI_prev = 0.0;

    int insertion_threshold = 0;

    cout << "Enter the insertion threshold: ";
    cin >> insertion_threshold;
    cout << "\n\n";

    printf(" %10s %30s %30s %30s %30s\n", "Input size", "MergeInsertion time", "Merge time", "Insertion time", "MergeInsertion time/prv");

    for (int size : inputSizes) {
        vector<int> arr = generateRandomVector(size);
        vector<int> arr_merge = arr;
        vector<int> arr_insertion = arr;

        auto MI_start = chrono::high_resolution_clock::now();
        mergeInsertionSort(arr, 0, arr.size() - 1, insertion_threshold);
        auto MI_stop = chrono::high_resolution_clock::now();
        auto MI_duration = chrono::duration_cast<chrono::microseconds>(MI_stop - MI_start);
        double MI_elapsedTime = MI_duration.count();
        double MI_timePrev = MI_prev == 0.0 ? 0.0 : MI_elapsedTime / MI_prev;

        auto Merge_start = chrono::high_resolution_clock::now();
        merge(arr_merge, 0, (arr_merge.size() - 1) / 2, arr_merge.size() - 1);
        auto Merge_stop = chrono::high_resolution_clock::now();
        auto Merge_duration = chrono::duration_cast<chrono::microseconds>(Merge_stop - Merge_start);
        double Merge_elapsedTime = Merge_duration.count();
        double Merge_timePrev = M_prev == 0.0 ? 0.0 : Merge_elapsedTime / M_prev;

        auto Insertion_start = chrono::high_resolution_clock::now();
        insertionSort(arr_insertion);
        auto Insertion_stop = chrono::high_resolution_clock::now();
        auto Insertion_duration = chrono::duration_cast<chrono::microseconds>(Insertion_stop - Insertion_start);
        double Insertion_elapsedTime = Insertion_duration.count();
        double Insertion_timePrev = I_prev == 0.0 ? 0.0 : Insertion_elapsedTime / I_prev;

        printf(" %10d %30.1f %30.1f %30.1f %30.1f\n", size, MI_elapsedTime, Merge_elapsedTime, Insertion_elapsedTime, MI_timePrev);

        MI_prev = MI_elapsedTime;
        M_prev = Merge_elapsedTime;
        I_prev = Insertion_elapsedTime;
    }

    return 0;
}
