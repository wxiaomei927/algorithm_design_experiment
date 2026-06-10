#include "sorting.h"

void bubble_sort(int arr[], int n) {
    // 如果数组为空或只有一个元素，无需排序
    if (n <= 1) {
        return;
    }
    
    // 外层循环：控制排序的轮数
    // 每一轮都会将最大的元素"冒泡"到末尾
    for (int i = 0; i < n - 1; i++) {
        // 标记本轮是否发生交换
        int swapped = 0;
        
        // 内层循环：比较相邻元素
        // 每轮结束后，末尾 i+1 个元素已经有序
        for (int j = 0; j < n - 1 - i; j++) {
            // 如果前一个元素大于后一个元素，交换它们
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = 1;
            }
        }
        
        // 如果本轮没有发生交换，说明数组已经有序，提前结束
        if (!swapped) {
            break;
        }
    }
}