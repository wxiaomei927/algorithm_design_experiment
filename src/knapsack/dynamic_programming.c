#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "knapsack.h"

KnapsackResult dynamic_programming_knapsack(Item *items, int n, int capacity) {
    KnapsackResult result;
    memset(&result, 0, sizeof(result));
    strcpy(result.algorithm, "dynamic_programming");
    result.elapsed_ms = -1;
    result.visited_nodes = 0;
    result.total_value = 0.0;
    result.total_weight = 0;

    if (capacity < 0 || n < 0) {
        strcpy(result.status, "failed");
        strcpy(result.note, "invalid capacity or n");
        return result;
    }

    long long mem_needed = (long long)(n + 1) * (capacity + 1) * sizeof(double);
    long long keep_mem = (long long)(n + 1) * (capacity + 1) * sizeof(int);
    result.estimated_memory_bytes = mem_needed + keep_mem;

    if (result.estimated_memory_bytes > (long long)512 * 1024 * 1024) {
        strcpy(result.status, "skipped");
        strcpy(result.note, "skipped because estimated DP memory exceeds 512 MB");
        return result;
    }

    double **dp = (double **)malloc((n + 1) * sizeof(double *));
    int **keep = (int **)malloc((n + 1) * sizeof(int *));

    if (!dp || !keep) {
        strcpy(result.status, "failed");
        strcpy(result.note, "memory allocation failed");
        if (dp) free(dp);
        if (keep) free(keep);
        return result;
    }

    int i;
    for (i = 0; i <= n; i++) {
        dp[i] = (double *)calloc(capacity + 1, sizeof(double));
        keep[i] = (int *)calloc(capacity + 1, sizeof(int));
        if (!dp[i] || !keep[i]) {
            strcpy(result.status, "failed");
            strcpy(result.note, "memory allocation failed");
            for (int j = 0; j < i; j++) {
                free(dp[j]);
                free(keep[j]);
            }
            free(dp);
            free(keep);
            return result;
        }
    }

    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);

    for (i = 1; i <= n; i++) {
        for (int w = 0; w <= capacity; w++) {
            dp[i][w] = dp[i - 1][w];
            keep[i][w] = 0;
            if (items[i - 1].weight <= w) {
                double val = dp[i - 1][w - items[i - 1].weight] + items[i - 1].value;
                if (val > dp[i][w]) {
                    dp[i][w] = val;
                    keep[i][w] = 1;
                }
            }
        }
    }

    result.total_value = dp[n][capacity];
    result.total_weight = 0;
    result.selected_count = 0;
    int w = capacity;
    for (i = n; i >= 1; i--) {
        if (keep[i][w]) {
            result.selected_count++;
            result.total_weight += items[i - 1].weight;
            w -= items[i - 1].weight;
        }
    }

    if (result.selected_count > 0) {
        result.selected_item_ids = (int *)malloc(result.selected_count * sizeof(int));
        if (!result.selected_item_ids) {
            strcpy(result.status, "failed");
            strcpy(result.note, "memory allocation failed");
            for (int j = 0; j <= n; j++) {
                free(dp[j]);
                free(keep[j]);
            }
            free(dp);
            free(keep);
            return result;
        }
        w = capacity;
        int idx = result.selected_count - 1;
        for (i = n; i >= 1; i--) {
            if (keep[i][w]) {
                result.selected_item_ids[idx--] = items[i - 1].id;
                w -= items[i - 1].weight;
            }
        }
    }

    QueryPerformanceCounter(&end);
    result.elapsed_ms = (double)(end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;
    strcpy(result.status, "completed");
    strcpy(result.note, "");

    for (i = 0; i <= n; i++) {
        free(dp[i]);
        free(keep[i]);
    }
    free(dp);
    free(keep);

    return result;
}
