#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "knapsack.h"

KnapsackResult brute_force_knapsack(Item *items, int n, int capacity) {
    KnapsackResult result;
    memset(&result, 0, sizeof(result));
    strcpy(result.algorithm, "brute_force");
    result.elapsed_ms = -1;
    result.visited_nodes = 0;

    if (n > 25) {
        strcpy(result.status, "skipped");
        strcpy(result.note, "skipped because brute force is O(2^n)");
        return result;
    }

    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);

    double best_value = 0;
    int best_weight = 0;
    long long best_mask = 0;
    long long total_subsets = 1LL << n;
    result.visited_nodes = total_subsets;

    for (long long mask = 0; mask < total_subsets; mask++) {
        int w = 0;
        double v = 0;
        for (int i = 0; i < n; i++) {
            if (mask & (1LL << i)) {
                w += items[i].weight;
                v += items[i].value;
            }
        }
        if (w <= capacity && v > best_value) {
            best_value = v;
            best_weight = w;
            best_mask = mask;
        }
    }

    QueryPerformanceCounter(&end);
    result.elapsed_ms = (double)(end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;
    result.total_weight = best_weight;
    result.total_value = best_value;
    result.selected_count = 0;
    for (int i = 0; i < n; i++) {
        if (best_mask & (1LL << i)) result.selected_count++;
    }
    result.selected_item_ids = (int *)malloc(result.selected_count * sizeof(int));
    int idx = 0;
    for (int i = 0; i < n; i++) {
        if (best_mask & (1LL << i)) {
            result.selected_item_ids[idx++] = items[i].id;
        }
    }
    strcpy(result.status, "completed");
    strcpy(result.note, "");
    return result;
}
