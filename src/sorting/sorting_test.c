#include "sorting.h"
#include "sorting_test.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>

int is_sorted(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return 0;
        }
    }
    return 1;
}

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

void copy_array(int dest[], int src[], int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}

void test_case(int original[], int n, const char *case_name) {
    printf("\n=== 测试用例: %s ===\n", case_name);
    printf("原始数组: ");
    print_array(original, n);
    printf("\n");
    
    int *arr_bubble = (int *)malloc(n * sizeof(int));
    int *arr_merge = (int *)malloc(n * sizeof(int));
    int *arr_quick = (int *)malloc(n * sizeof(int));
    
    copy_array(arr_bubble, original, n);
    copy_array(arr_merge, original, n);
    copy_array(arr_quick, original, n);
    
    long long cmp_bubble = bubble_sort(arr_bubble, n);
    long long cmp_merge = merge_sort(arr_merge, n);
    long long cmp_quick = quick_sort(arr_quick, n);
    
    int bubble_pass = is_sorted(arr_bubble, n);
    int merge_pass = is_sorted(arr_merge, n);
    int quick_pass = is_sorted(arr_quick, n);
    
    printf("Bubble Sort: %s, comparisons = %lld\n", bubble_pass ? "PASS" : "FAIL", cmp_bubble);
    printf("Merge Sort:  %s, comparisons = %lld\n", merge_pass ? "PASS" : "FAIL", cmp_merge);
    printf("Quick Sort:  %s, comparisons = %lld\n", quick_pass ? "PASS" : "FAIL", cmp_quick);
    
    free(arr_bubble);
    free(arr_merge);
    free(arr_quick);
}

void runSortingTests(void) {
    printf("\n===== 排序算法测试 =====\n");
    
    int arr1[] = {5, 2, 9, 1, 5, 6};
    test_case(arr1, 6, "普通无序数组");
    
    int arr2[] = {1, 2, 3, 4, 5, 6};
    test_case(arr2, 6, "已经有序的数组");
    
    int arr3[] = {6, 5, 4, 3, 2, 1};
    test_case(arr3, 6, "逆序数组");
    
    int arr4[] = {4, 2, 4, 1, 2, 3};
    test_case(arr4, 6, "包含重复元素的数组");
    
    int arr5[] = {7};
    test_case(arr5, 1, "只有一个元素的数组");
    
    int *arr6 = NULL;
    test_case(arr6, 0, "空数组");
    
    printf("\n===== 人工验证测试 =====\n");
    int arr7[] = {4, 3, 2, 1};
    test_case(arr7, 4, "逆序数组 {4, 3, 2, 1} (验证冒泡排序比较次数为 6)");
    
    printf("\n===== 排序测试完成 =====\n");
}

void generate_random_array(int arr[], int n, unsigned int seed, int min_val, int max_val) {
    srand(seed);
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % (max_val - min_val + 1) + min_val;
    }
}

void save_random_data_to_csv(const char *filename, int arr[], int n) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("无法创建文件: %s\n", filename);
        return;
    }
    
    fprintf(fp, "index,value\n");
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%d,%d\n", i, arr[i]);
    }
    
    fclose(fp);
}

void append_comparison_result(const char *filename, int dataset_id, unsigned int seed, 
                               int n, const char *algorithm, long long comparisons, int sorted_correctly) {
    FILE *fp = fopen(filename, "a");
    if (fp == NULL) {
        printf("无法打开文件: %s\n", filename);
        return;
    }
    
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    if (file_size == 0) {
        fprintf(fp, "dataset_id,seed,n,algorithm,comparisons,sorted_correctly\n");
    }
    
    fprintf(fp, "%d,%u,%d,%s,%lld,%s\n", dataset_id, seed, n, algorithm, comparisons, 
            sorted_correctly ? "yes" : "no");
    
    fclose(fp);
}

void run_single_dataset_experiment(int original[], int n, int dataset_id, unsigned int seed, 
                                    const char *results_file) {
    int *arr_bubble = (int *)malloc(n * sizeof(int));
    int *arr_merge = (int *)malloc(n * sizeof(int));
    int *arr_quick = (int *)malloc(n * sizeof(int));
    
    copy_array(arr_bubble, original, n);
    copy_array(arr_merge, original, n);
    copy_array(arr_quick, original, n);
    
    long long cmp_bubble = bubble_sort(arr_bubble, n);
    long long cmp_merge = merge_sort(arr_merge, n);
    long long cmp_quick = quick_sort(arr_quick, n);
    
    int bubble_pass = is_sorted(arr_bubble, n);
    int merge_pass = is_sorted(arr_merge, n);
    int quick_pass = is_sorted(arr_quick, n);
    
    append_comparison_result(results_file, dataset_id, seed, n, "bubble_sort", cmp_bubble, bubble_pass);
    append_comparison_result(results_file, dataset_id, seed, n, "merge_sort", cmp_merge, merge_pass);
    append_comparison_result(results_file, dataset_id, seed, n, "quick_sort", cmp_quick, quick_pass);
    
    printf("\nDataset %d (seed=%u):\n", dataset_id, seed);
    printf("  Bubble Sort: comparisons = %lld, sorted = %s\n", cmp_bubble, bubble_pass ? "yes" : "no");
    printf("  Merge Sort:  comparisons = %lld, sorted = %s\n", cmp_merge, merge_pass ? "yes" : "no");
    printf("  Quick Sort:  comparisons = %lld, sorted = %s\n", cmp_quick, quick_pass ? "yes" : "no");
    
    free(arr_bubble);
    free(arr_merge);
    free(arr_quick);
}

void run_sorting_two_sets_experiment(void) {
    const int N = 100;
    const int MIN_VAL = 1;
    const int MAX_VAL = 100000;
    const unsigned int SEED1 = 20260601;
    const unsigned int SEED2 = 20260602;
    
    const char *data_file1 = "data/sorting/random_set_1.csv";
    const char *data_file2 = "data/sorting/random_set_2.csv";
    const char *results_file = "results/sorting/two_sets_comparisons.csv";
    
    printf("\n===== 两组 100 个随机数排序实验 =====\n");
    printf("随机数范围: %d ~ %d\n", MIN_VAL, MAX_VAL);
    printf("数据集大小: %d\n", N);
    
    int *data1 = (int *)malloc(N * sizeof(int));
    int *data2 = (int *)malloc(N * sizeof(int));
    
    printf("\n生成随机数据...\n");
    generate_random_array(data1, N, SEED1, MIN_VAL, MAX_VAL);
    generate_random_array(data2, N, SEED2, MIN_VAL, MAX_VAL);
    
    save_random_data_to_csv(data_file1, data1, N);
    save_random_data_to_csv(data_file2, data2, N);
    printf("随机数据已保存到:\n");
    printf("  %s\n", data_file1);
    printf("  %s\n", data_file2);
    
    FILE *fp = fopen(results_file, "w");
    if (fp != NULL) {
        fclose(fp);
    }
    
    printf("\n运行排序实验...\n");
    run_single_dataset_experiment(data1, N, 1, SEED1, results_file);
    run_single_dataset_experiment(data2, N, 2, SEED2, results_file);
    
    printf("\n比较结果已保存到: %s\n", results_file);
    printf("\n===== 实验完成 =====\n");
    
    free(data1);
    free(data2);
}

// 使用 QueryPerformanceCounter 获取高精度时间（毫秒）
double get_current_time_ms() {
    LARGE_INTEGER freq, count;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&count);
    return (double)count.QuadPart * 1000.0 / (double)freq.QuadPart;
}

// 运行单算法单次测试，返回比较次数，输出执行时间
long long run_single_algorithm(int arr[], int n, const char *algorithm, double *elapsed_ms) {
    *elapsed_ms = 0.0;
    long long comparisons = 0;
    
    double start = get_current_time_ms();
    
    if (strcmp(algorithm, "bubble_sort") == 0) {
        comparisons = bubble_sort(arr, n);
    } else if (strcmp(algorithm, "merge_sort") == 0) {
        comparisons = merge_sort(arr, n);
    } else if (strcmp(algorithm, "quick_sort") == 0) {
        comparisons = quick_sort(arr, n);
    }
    
    double end = get_current_time_ms();
    *elapsed_ms = end - start;
    
    return comparisons;
}

// 运行单规模完整实验
void run_single_scale_experiment(int n, unsigned int seed, const char *results_file,
                                  const char *merge_recursion_file, 
                                  const char *quick_recursion_file,
                                  FILE *summary_fp, int enable_recursion) {
    const int MIN_VAL = 1;
    const int MAX_VAL = 100000;
    const int REPEAT = 5;
    
    printf("\n处理 n = %d...\n", n);
    
    // 生成随机数组
    int *original = (int *)malloc(n * sizeof(int));
    if (original == NULL) {
        printf("  内存分配失败\n");
        return;
    }
    generate_random_array(original, n, seed, MIN_VAL, MAX_VAL);
    
    // 为三种算法准备副本
    int *arr_bubble = (int *)malloc(n * sizeof(int));
    int *arr_merge = (int *)malloc(n * sizeof(int));
    int *arr_quick = (int *)malloc(n * sizeof(int));
    
    FILE *results_fp = fopen(results_file, "a");
    if (results_fp == NULL) {
        printf("  无法打开结果文件\n");
        free(original);
        return;
    }
    
    // 检查文件是否为空
    fseek(results_fp, 0, SEEK_END);
    long file_size = ftell(results_fp);
    if (file_size == 0) {
        fprintf(results_fp, "n,seed,algorithm,comparisons,elapsed_ms,sorted_correctly,status,note\n");
    }
    
    // 冒泡排序：n > 10000 时跳过
    if (n <= 10000) {
        double total_time = 0.0;
        long long total_comparisons = 0;
        
        for (int i = 0; i < REPEAT; i++) {
            copy_array(arr_bubble, original, n);
            double elapsed_ms;
            total_comparisons += run_single_algorithm(arr_bubble, n, "bubble_sort", &elapsed_ms);
            total_time += elapsed_ms;
        }
        
        double avg_time = total_time / REPEAT;
        long long avg_comparisons = total_comparisons / REPEAT;
        int sorted = is_sorted(arr_bubble, n);
        
        fprintf(results_fp, "%d,%u,bubble_sort,%lld,%.4f,%s,completed,\n", 
                n, seed, avg_comparisons, avg_time, sorted ? "yes" : "no");
        
        printf("  Bubble Sort: comparisons = %lld, time = %.2f ms, sorted = %s\n", 
               avg_comparisons, avg_time, sorted ? "yes" : "no");
    } else {
        fprintf(results_fp, "%d,%u,bubble_sort,,,N/A,skipped,skipped because bubble sort is O(n^2)\n", n, seed);
        printf("  Bubble Sort: skipped (O(n^2) for large n)\n");
    }
    
    // 合并排序
    double merge_total_time = 0.0;
    long long merge_total_comparisons = 0;
    
    for (int i = 0; i < REPEAT; i++) {
        copy_array(arr_merge, original, n);
        double elapsed_ms;
        merge_total_comparisons += run_single_algorithm(arr_merge, n, "merge_sort", &elapsed_ms);
        merge_total_time += elapsed_ms;
    }
    
    double merge_avg_time = merge_total_time / REPEAT;
    long long merge_avg_comparisons = merge_total_comparisons / REPEAT;
    int merge_sorted = is_sorted(arr_merge, n);
    
    fprintf(results_fp, "%d,%u,merge_sort,%lld,%.4f,%s,completed,\n", 
            n, seed, merge_avg_comparisons, merge_avg_time, merge_sorted ? "yes" : "no");
    
    printf("  Merge Sort:  comparisons = %lld, time = %.2f ms, sorted = %s\n", 
           merge_avg_comparisons, merge_avg_time, merge_sorted ? "yes" : "no");
    
    // 快速排序
    double quick_total_time = 0.0;
    long long quick_total_comparisons = 0;
    
    for (int i = 0; i < REPEAT; i++) {
        copy_array(arr_quick, original, n);
        double elapsed_ms;
        quick_total_comparisons += run_single_algorithm(arr_quick, n, "quick_sort", &elapsed_ms);
        quick_total_time += elapsed_ms;
    }
    
    double quick_avg_time = quick_total_time / REPEAT;
    long long quick_avg_comparisons = quick_total_comparisons / REPEAT;
    int quick_sorted = is_sorted(arr_quick, n);
    
    fprintf(results_fp, "%d,%u,quick_sort,%lld,%.4f,%s,completed,\n", 
            n, seed, quick_avg_comparisons, quick_avg_time, quick_sorted ? "yes" : "no");
    
    printf("  Quick Sort:  comparisons = %lld, time = %.2f ms, sorted = %s\n", 
           quick_avg_comparisons, quick_avg_time, quick_sorted ? "yes" : "no");
    
    // 递归记录（对所有规模）
    if (enable_recursion) {
        // 合并排序递归记录
        FILE *merge_fp = fopen(merge_recursion_file, "a");
        if (merge_fp != NULL) {
            fseek(merge_fp, 0, SEEK_END);
            if (ftell(merge_fp) == 0) {
                fprintf(merge_fp, "algorithm,original_n,call_index,recursion_depth,subproblem_size\n");
            }
            fclose(merge_fp);
        }
        
        int *arr_merge_recur = (int *)malloc(n * sizeof(int));
        copy_array(arr_merge_recur, original, n);
        merge_sort_with_recursion(arr_merge_recur, n, 1, merge_recursion_file, n);
        free(arr_merge_recur);
        
        // 快速排序递归记录
        FILE *quick_fp = fopen(quick_recursion_file, "a");
        if (quick_fp != NULL) {
            fseek(quick_fp, 0, SEEK_END);
            if (ftell(quick_fp) == 0) {
                fprintf(quick_fp, "algorithm,original_n,call_index,recursion_depth,subproblem_size\n");
            }
            fclose(quick_fp);
        }
        
        int *arr_quick_recur = (int *)malloc(n * sizeof(int));
        copy_array(arr_quick_recur, original, n);
        quick_sort_with_recursion(arr_quick_recur, n, 1, quick_recursion_file, n);
        free(arr_quick_recur);
        
        printf("  递归记录已保存\n");
    }
    
    fclose(results_fp);
    free(original);
    free(arr_bubble);
    free(arr_merge);
    free(arr_quick);
}

void run_full_sorting_experiment(void) {
    const int scales[] = {10, 100, 1000, 2000, 5000, 10000, 100000};
    const int num_scales = sizeof(scales) / sizeof(scales[0]);
    
    const char *results_file = "results/sorting/scaled_sorting_results.csv";
    const char *merge_recursion_file = "results/sorting/merge_recursion_sizes.csv";
    const char *quick_recursion_file = "results/sorting/quick_recursion_sizes.csv";
    const char *summary_file = "results/sorting/recursion_summary.csv";
    
    printf("\n===== 完整排序批量实验 =====\n");
    printf("测试规模: ");
    for (int i = 0; i < num_scales; i++) {
        printf("%d%s", scales[i], (i < num_scales - 1) ? ", " : "\n");
    }
    
    // 创建空的结果文件
    FILE *fp = fopen(results_file, "w");
    if (fp != NULL) fclose(fp);
    
    fp = fopen(merge_recursion_file, "w");
    if (fp != NULL) fclose(fp);
    
    fp = fopen(quick_recursion_file, "w");
    if (fp != NULL) fclose(fp);
    
    fp = fopen(summary_file, "w");
    if (fp != NULL) {
        fprintf(fp, "algorithm,n,total_calls,max_depth,min_subproblem_size,max_subproblem_size\n");
        fclose(fp);
    }
    
    // 对每个规模运行实验（所有规模都记录递归）
    for (int i = 0; i < num_scales; i++) {
        int n = scales[i];
        unsigned int seed = 20260610 + n;
        run_single_scale_experiment(n, seed, results_file, 
                                    merge_recursion_file, quick_recursion_file,
                                    NULL, 1);  // enable_recursion = 1 对所有规模启用
    }
    
    printf("\n===== 实验完成 =====\n");
    printf("结果已保存到:\n");
    printf("  %s\n", results_file);
    printf("  %s\n", merge_recursion_file);
    printf("  %s\n", quick_recursion_file);
    printf("  %s\n", summary_file);
}