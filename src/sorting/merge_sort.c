#include "sorting.h"
#include <stdlib.h>

// 合并两个有序子数组
// left: 左子数组起始索引
// mid: 左子数组结束索引，右子数组起始索引 = mid + 1
// right: 右子数组结束索引
static void merge(int arr[], int left, int mid, int right) {
    // 计算两个子数组的长度
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    // 创建临时数组存储两个子数组
    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));
    
    // 将数据复制到临时数组
    for (int i = 0; i < n1; i++) {
        L[i] = arr[left + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = arr[mid + 1 + j];
    }
    
    // 合并两个临时数组回原数组
    int i = 0;  // 左子数组的索引
    int j = 0;  // 右子数组的索引
    int k = left;  // 原数组的索引
    
    // 比较两个子数组的元素，选择较小的放入原数组
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    // 将左子数组剩余元素复制到原数组
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    // 将右子数组剩余元素复制到原数组
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
    
    // 释放临时数组内存
    free(L);
    free(R);
}

// 归并排序递归函数
static void merge_sort_recursive(int arr[], int left, int right) {
    // 递归终止条件：当左边界 >= 右边界时
    if (left < right) {
        // 找到中间位置
        int mid = left + (right - left) / 2;
        
        // 递归排序左半部分
        merge_sort_recursive(arr, left, mid);
        // 递归排序右半部分
        merge_sort_recursive(arr, mid + 1, right);
        
        // 合并两个有序部分
        merge(arr, left, mid, right);
    }
}

void merge_sort(int arr[], int n) {
    // 如果数组为空或只有一个元素，无需排序
    if (n <= 1) {
        return;
    }
    
    // 调用递归函数进行排序
    merge_sort_recursive(arr, 0, n - 1);
}