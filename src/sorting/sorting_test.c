#include "sorting.h"
#include <stdio.h>
#include <stdlib.h>

// 判断数组是否已经按照从小到大排序
int is_sorted(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return 0;  // 未排序
        }
    }
    return 1;  // 已排序
}

// 打印数组
void print_array(int arr[], int n) {
    if (n == 0) {
        printf("[]");
        return;
    }
    
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) {
            printf(", ");
        }
    }
    printf("]");
}

// 复制数组
void copy_array(int dest[], int src[], int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}

// 测试单个用例
void test_case(int original[], int n, const char *case_name) {
    printf("\n=== 测试用例: %s ===\n", case_name);
    printf("原始数组: ");
    print_array(original, n);
    printf("\n");
    
    // 创建三个副本，分别用于三个排序算法
    int *arr_bubble = (int *)malloc(n * sizeof(int));
    int *arr_merge = (int *)malloc(n * sizeof(int));
    int *arr_quick = (int *)malloc(n * sizeof(int));
    
    copy_array(arr_bubble, original, n);
    copy_array(arr_merge, original, n);
    copy_array(arr_quick, original, n);
    
    // 分别运行三个排序算法
    bubble_sort(arr_bubble, n);
    merge_sort(arr_merge, n);
    quick_sort(arr_quick, n);
    
    // 打印排序结果
    printf("冒泡排序结果: ");
    print_array(arr_bubble, n);
    printf(" -> %s\n", is_sorted(arr_bubble, n) ? "通过" : "失败");
    
    printf("合并排序结果: ");
    print_array(arr_merge, n);
    printf(" -> %s\n", is_sorted(arr_merge, n) ? "通过" : "失败");
    
    printf("快速排序结果: ");
    print_array(arr_quick, n);
    printf(" -> %s\n", is_sorted(arr_quick, n) ? "通过" : "失败");
    
    // 释放内存
    free(arr_bubble);
    free(arr_merge);
    free(arr_quick);
}

void runSortingTests() {
    printf("\n===== 排序算法测试 =====\n");
    
    // 测试用例 1: 普通无序数组
    int arr1[] = {5, 2, 9, 1, 5, 6};
    test_case(arr1, 6, "普通无序数组");
    
    // 测试用例 2: 已经有序的数组
    int arr2[] = {1, 2, 3, 4, 5, 6};
    test_case(arr2, 6, "已经有序的数组");
    
    // 测试用例 3: 逆序数组
    int arr3[] = {6, 5, 4, 3, 2, 1};
    test_case(arr3, 6, "逆序数组");
    
    // 测试用例 4: 包含重复元素的数组
    int arr4[] = {4, 2, 4, 1, 2, 3};
    test_case(arr4, 6, "包含重复元素的数组");
    
    // 测试用例 5: 只有一个元素的数组
    int arr5[] = {7};
    test_case(arr5, 1, "只有一个元素的数组");
    
    // 测试用例 6: 空数组
    int *arr6 = NULL;
    test_case(arr6, 0, "空数组");
    
    printf("\n===== 排序测试完成 =====\n");
}