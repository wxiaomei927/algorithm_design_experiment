#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "knapsack.h"

// 位图操作宏
#define BITMAP_INDEX(i, w, capacity) ((long long)(i) * ((capacity) + 1) + (w))
#define BITMAP_BYTE(idx) ((idx) / 8)
#define BITMAP_BIT(idx) ((idx) % 8)
#define SET_BIT(bitmap, idx) (bitmap[BITMAP_BYTE(idx)] |= (1 << BITMAP_BIT(idx)))
#define GET_BIT(bitmap, idx) ((bitmap[BITMAP_BYTE(idx)] >> BITMAP_BIT(idx)) & 1)

KnapsackResult dynamic_programming_optimized_knapsack(Item *items, int n, int capacity) {
    KnapsackResult result;
    memset(&result, 0, sizeof(result));
    strcpy(result.algorithm, "dynamic_programming_optimized");
    result.elapsed_ms = -1;
    result.visited_nodes = 0;
    result.total_value = 0.0;
    result.total_weight = 0;
    result.estimated_operations = (long long)n * capacity;

    if (capacity < 0 || n < 0) {
        strcpy(result.status, "failed");
        strcpy(result.note, "invalid capacity or n");
        return result;
    }

    // 内存估算：
    // 1. 一维dp数组: (capacity + 1) * sizeof(double)
    // 2. 位图: (n * (capacity + 1) + 7) / 8 bytes
    // 3. 辅助数组: selected_item_ids 最多 n 个 int
    long long dp_mem = (long long)(capacity + 1) * sizeof(double);
    long long bitmap_bits = (long long)n * (capacity + 1);
    long long bitmap_mem = (bitmap_bits + 7) / 8;
    long long aux_mem = (long long)n * sizeof(int);
    result.estimated_memory_bytes = dp_mem + bitmap_mem + aux_mem;

    if (result.estimated_memory_bytes > (long long)512 * 1024 * 1024) {
        strcpy(result.status, "skipped");
        strcpy(result.note, "skipped because estimated DP memory exceeds 512 MB");
        return result;
    }

    if (result.estimated_operations > (long long)5 * 100000000) {
        strcpy(result.status, "skipped");
        strcpy(result.note, "skipped because estimated DP operations exceed safe limit");
        return result;
    }

    double *dp = (double *)calloc(capacity + 1, sizeof(double));
    if (!dp) {
        strcpy(result.status, "failed");
        strcpy(result.note, "memory allocation failed");
        return result;
    }

    // 分配位图
    unsigned char *bitmap = (unsigned char *)calloc(bitmap_mem, 1);
    if (!bitmap) {
        strcpy(result.status, "failed");
        strcpy(result.note, "bitmap allocation failed");
        free(dp);
        return result;
    }

    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);

    // 一维DP，容量从大到小循环
    for (int i = 0; i < n; i++) {
        int wt = items[i].weight;
        double val = items[i].value;
        for (int w = capacity; w >= wt; w--) {
            double new_val = dp[w - wt] + val;
            if (new_val > dp[w]) {
                dp[w] = new_val;
                // 记录选择：物品i在容量w时被选择
                long long idx = BITMAP_INDEX(i, w, capacity);
                SET_BIT(bitmap, idx);
            }
        }
    }

    result.total_value = dp[capacity];

    QueryPerformanceCounter(&end);
    result.elapsed_ms = (double)(end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;

    // 从capacity反向恢复所选物品
    result.selected_count = 0;
    result.total_weight = 0;
    
    // 临时数组存储选中的物品索引
    int *selected_indices = (int *)malloc(n * sizeof(int));
    if (!selected_indices) {
        strcpy(result.status, "completed");
        strcpy(result.note, "selected items not recovered due to memory limit");
        free(dp);
        free(bitmap);
        return result;
    }

    int w = capacity;
    for (int i = n - 1; i >= 0; i--) {
        long long idx = BITMAP_INDEX(i, w, capacity);
        if (GET_BIT(bitmap, idx)) {
            selected_indices[result.selected_count++] = i;
            result.total_weight += items[i].weight;
            w -= items[i].weight;
        }
    }

    // 分配并填充selected_item_ids
    if (result.selected_count > 0) {
        result.selected_item_ids = (int *)malloc(result.selected_count * sizeof(int));
        if (result.selected_item_ids) {
            // 反转顺序（因为我们是从后往前收集的）
            for (int i = 0; i < result.selected_count; i++) {
                result.selected_item_ids[i] = items[selected_indices[result.selected_count - 1 - i]].id;
            }
            strcpy(result.status, "completed");
            strcpy(result.note, "");
            
            // 使用公共验证函数
            validate_selected_items(items, n, capacity, &result);
        } else {
            strcpy(result.status, "completed");
            strcpy(result.note, "selected items not recovered due to memory limit");
        }
    } else {
        strcpy(result.status, "completed");
        strcpy(result.note, "");
    }

    free(selected_indices);
    free(dp);
    free(bitmap);
    return result;
}
