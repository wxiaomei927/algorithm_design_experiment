#include "sorting.h"

long long bubble_sort(int arr[], int n) {
    long long comparisons = 0;
    
    if (n <= 1) {
        return comparisons;
    }
    
    for (int i = 0; i < n - 1; i++) {
        int swapped = 0;
        
        for (int j = 0; j < n - 1 - i; j++) {
            comparisons++;
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = 1;
            }
        }
        
        if (!swapped) {
            break;
        }
    }
    
    return comparisons;
}