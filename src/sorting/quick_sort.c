#include "sorting.h"

// 交换两个整数
static void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// 分区函数：选择区间末尾元素作为主元
// 返回主元最终的位置索引
static int partition(int arr[], int low, int high) {
    // 选择区间末尾元素作为主元
    int pivot = arr[high];
    
    // i 指向小于等于主元的区域的最后一个元素
    int i = low - 1;
    
    // 遍历整个区间（不包括主元）
    for (int j = low; j < high; j++) {
        // 如果当前元素小于等于主元
        if (arr[j] <= pivot) {
            // 将 i 向后移动一位
            i++;
            // 交换 arr[i] 和 arr[j]，将当前元素放到小于等于主元的区域
            swap(&arr[i], &arr[j]);
        }
    }
    
    // 将主元放到正确的位置（i+1）
    swap(&arr[i + 1], &arr[high]);
    
    // 返回主元的最终位置
    return i + 1;
}

// 快速排序递归函数
static void quick_sort_recursive(int arr[], int low, int high) {
    // 递归终止条件：当 low >= high 时
    if (low < high) {
        // 分区，获取主元位置
        int pi = partition(arr, low, high);
        
        // 递归排序左半部分（小于主元的元素）
        quick_sort_recursive(arr, low, pi - 1);
        // 递归排序右半部分（大于主元的元素）
        quick_sort_recursive(arr, pi + 1, high);
    }
}

void quick_sort(int arr[], int n) {
    // 如果数组为空或只有一个元素，无需排序
    if (n <= 1) {
        return;
    }
    
    // 调用递归函数进行排序
    quick_sort_recursive(arr, 0, n - 1);
}