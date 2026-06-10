#ifndef SORTING_H
#define SORTING_H

// 排序算法接口（不记录递归信息，用于性能测试）
long long bubble_sort(int arr[], int n);
long long merge_sort(int arr[], int n);
long long quick_sort(int arr[], int n);

// 排序算法接口（记录递归信息，用于分析递归调用）
// record_recursion: 是否记录递归调用信息
// recursion_file: 递归信息输出文件（NULL表示不输出到文件）
// original_n: 原始数组大小（用于记录）
long long merge_sort_with_recursion(int arr[], int n, int record_recursion, 
                                    const char *recursion_file, int original_n);
long long quick_sort_with_recursion(int arr[], int n, int record_recursion, 
                                    const char *recursion_file, int original_n);

#endif