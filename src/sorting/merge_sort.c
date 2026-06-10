#include "sorting.h"
#include <stdlib.h>
#include <stdio.h>

static void merge(int arr[], int left, int mid, int right, long long *comparisons) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));
    
    for (int i = 0; i < n1; i++) {
        L[i] = arr[left + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = arr[mid + 1 + j];
    }
    
    int i = 0, j = 0, k = left;
    
    while (i < n1 && j < n2) {
        (*comparisons)++;
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
    
    free(L);
    free(R);
}

static void merge_sort_recursive(int arr[], int left, int right, long long *comparisons) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        
        merge_sort_recursive(arr, left, mid, comparisons);
        merge_sort_recursive(arr, mid + 1, right, comparisons);
        
        merge(arr, left, mid, right, comparisons);
    }
}

long long merge_sort(int arr[], int n) {
    long long comparisons = 0;
    
    if (n <= 1) {
        return comparisons;
    }
    
    merge_sort_recursive(arr, 0, n - 1, &comparisons);
    
    return comparisons;
}

// 带递归记录的合并排序

static void merge_sort_recursive_with_recursion(int arr[], int left, int right, 
                                               long long *comparisons,
                                               int record_recursion, FILE *recursion_fp,
                                               int original_n, int *call_index, int recursion_depth,
                                               int *max_depth, int *min_subproblem, int *max_subproblem) {
    int subproblem_size = right - left + 1;
    
    // 更新递归统计信息
    (*call_index)++;
    if (recursion_depth > *max_depth) *max_depth = recursion_depth;
    if (subproblem_size < *min_subproblem) *min_subproblem = subproblem_size;
    if (subproblem_size > *max_subproblem) *max_subproblem = subproblem_size;
    
    // 记录递归调用信息到文件
    if (record_recursion && recursion_fp != NULL) {
        fprintf(recursion_fp, "merge_sort,%d,%d,%d,%d\n", 
                original_n, *call_index, recursion_depth, subproblem_size);
    }
    
    if (left < right) {
        int mid = left + (right - left) / 2;
        
        merge_sort_recursive_with_recursion(arr, left, mid, comparisons,
                                           record_recursion, recursion_fp,
                                           original_n, call_index, recursion_depth + 1,
                                           max_depth, min_subproblem, max_subproblem);
        merge_sort_recursive_with_recursion(arr, mid + 1, right, comparisons,
                                           record_recursion, recursion_fp,
                                           original_n, call_index, recursion_depth + 1,
                                           max_depth, min_subproblem, max_subproblem);
        
        merge(arr, left, mid, right, comparisons);
    }
}

long long merge_sort_with_recursion(int arr[], int n, int record_recursion, 
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
    
    merge_sort_recursive_with_recursion(arr, 0, n - 1, &comparisons,
                                       record_recursion, recursion_fp,
                                       original_n, &call_index, 0,
                                       &max_depth, &min_subproblem, &max_subproblem);
    
    // 写入汇总信息到单独的文件
    if (record_recursion) {
        FILE *summary_fp = fopen("results/sorting/recursion_summary.csv", "a");
        if (summary_fp != NULL) {
            fprintf(summary_fp, "merge_sort,%d,%d,%d,%d,%d\n", 
                    n, call_index, max_depth, min_subproblem, max_subproblem);
            fclose(summary_fp);
        }
    }
    
    if (recursion_fp != NULL) {
        fclose(recursion_fp);
    }
    
    return comparisons;
}