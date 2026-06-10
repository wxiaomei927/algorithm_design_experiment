#include "sorting.h"
#include <stdio.h>

static void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

static int partition(int arr[], int low, int high, long long *comparisons) {
    int pivot = arr[high];
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        (*comparisons)++;
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    
    swap(&arr[i + 1], &arr[high]);
    
    return i + 1;
}

static void quick_sort_recursive(int arr[], int low, int high, long long *comparisons) {
    if (low < high) {
        int pi = partition(arr, low, high, comparisons);
        
        quick_sort_recursive(arr, low, pi - 1, comparisons);
        quick_sort_recursive(arr, pi + 1, high, comparisons);
    }
}

long long quick_sort(int arr[], int n) {
    long long comparisons = 0;
    
    if (n <= 1) {
        return comparisons;
    }
    
    quick_sort_recursive(arr, 0, n - 1, &comparisons);
    
    return comparisons;
}

// 带递归记录的快速排序

static void quick_sort_recursive_with_recursion(int arr[], int low, int high, 
                                               long long *comparisons,
                                               int record_recursion, FILE *recursion_fp,
                                               int original_n, int *call_index, int recursion_depth,
                                               int *max_depth, int *min_subproblem, int *max_subproblem) {
    // 仅当 low <= high 时记录（避免记录空区间）
    if (low <= high) {
        int subproblem_size = high - low + 1;
        
        // 更新递归统计信息
        (*call_index)++;
        if (recursion_depth > *max_depth) *max_depth = recursion_depth;
        if (subproblem_size < *min_subproblem) *min_subproblem = subproblem_size;
        if (subproblem_size > *max_subproblem) *max_subproblem = subproblem_size;
        
        // 记录递归调用信息到文件
        if (record_recursion && recursion_fp != NULL) {
            fprintf(recursion_fp, "quick_sort,%d,%d,%d,%d\n", 
                    original_n, *call_index, recursion_depth, subproblem_size);
        }
        
        if (low < high) {
            int pi = partition(arr, low, high, comparisons);
            
            quick_sort_recursive_with_recursion(arr, low, pi - 1, comparisons,
                                               record_recursion, recursion_fp,
                                               original_n, call_index, recursion_depth + 1,
                                               max_depth, min_subproblem, max_subproblem);
            quick_sort_recursive_with_recursion(arr, pi + 1, high, comparisons,
                                               record_recursion, recursion_fp,
                                               original_n, call_index, recursion_depth + 1,
                                               max_depth, min_subproblem, max_subproblem);
        }
    }
}

long long quick_sort_with_recursion(int arr[], int n, int record_recursion, 
                                    const char *recursion_file, int original_n) {
    long long comparisons = 0;
    
    if (n <= 1) {
        return comparisons;
    }
    
    FILE *recursion_fp = NULL;
    if (record_recursion && recursion_file != NULL) {
        recursion_fp = fopen(recursion_file, "a");
        if (recursion_fp == NULL) {
            record_recursion = 0;
        }
    }
    
    int call_index = 0;
    int max_depth = 0;
    int min_subproblem = n;
    int max_subproblem = 0;
    
    quick_sort_recursive_with_recursion(arr, 0, n - 1, &comparisons,
                                       record_recursion, recursion_fp,
                                       original_n, &call_index, 0,
                                       &max_depth, &min_subproblem, &max_subproblem);
    
    // 写入汇总信息到单独的文件
    if (record_recursion) {
        FILE *summary_fp = fopen("results/sorting/recursion_summary.csv", "a");
        if (summary_fp != NULL) {
            fprintf(summary_fp, "quick_sort,%d,%d,%d,%d,%d\n", 
                    n, call_index, max_depth, min_subproblem, max_subproblem);
            fclose(summary_fp);
        }
    }
    
    if (recursion_fp != NULL) {
        fclose(recursion_fp);
    }
    
    return comparisons;
}