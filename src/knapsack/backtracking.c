#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "knapsack.h"

#define MAX_VISITED_NODES 100000000LL

static int compare_ratio_desc_bt(const void *a, const void *b) {
    Item *ia = (Item *)a;
    Item *ib = (Item *)b;
    if (ib->ratio > ia->ratio) return 1;
    if (ib->ratio < ia->ratio) return -1;
    return 0;
}

static int backtrack(Item *items, int n, int capacity, int idx, int current_weight,
                     double current_value, double *best_value, int *best_weight,
                     int *current_path, int *best_path, int *best_path_len,
                     long long *visited_nodes, int *exceeded) {
    if (*exceeded) return 0;
    
    (*visited_nodes)++;
    
    if (*visited_nodes > MAX_VISITED_NODES) {
        *exceeded = 1;
        return 0;
    }

    if (current_weight > capacity) return 1;

    if (current_value > *best_value) {
        *best_value = current_value;
        *best_weight = current_weight;
        *best_path_len = idx;
        for (int i = 0; i < idx; i++) {
            best_path[i] = current_path[i];
        }
    }

    if (idx >= n) return 1;

    double upper_bound = current_value;
    for (int i = idx; i < n; i++) {
        upper_bound += items[i].value;
    }
    if (upper_bound <= *best_value) return 1;

    current_path[idx] = 1;
    backtrack(items, n, capacity, idx + 1, current_weight + items[idx].weight,
              current_value + items[idx].value, best_value, best_weight,
              current_path, best_path, best_path_len, visited_nodes, exceeded);

    if (*exceeded) return 0;

    current_path[idx] = 0;
    backtrack(items, n, capacity, idx + 1, current_weight, current_value,
              best_value, best_weight, current_path, best_path, best_path_len,
              visited_nodes, exceeded);

    return 1;
}

KnapsackResult backtracking_knapsack(Item *items, int n, int capacity) {
    KnapsackResult result;
    memset(&result, 0, sizeof(result));
    strcpy(result.algorithm, "backtracking");
    result.elapsed_ms = -1;
    result.visited_nodes = 0;
    result.total_value = 0.0;
    result.total_weight = 0;

    if (n > 40) {
        strcpy(result.status, "skipped");
        strcpy(result.note, "skipped because backtracking worst-case complexity is O(2^n)");
        result.estimated_memory_bytes = 0;
        return result;
    }

    Item *sorted = (Item *)malloc(n * sizeof(Item));
    if (!sorted) {
        strcpy(result.status, "failed");
        strcpy(result.note, "memory allocation failed");
        return result;
    }
    memcpy(sorted, items, n * sizeof(Item));
    qsort(sorted, n, sizeof(Item), compare_ratio_desc_bt);

    int *current_path = (int *)calloc(n, sizeof(int));
    int *best_path = (int *)calloc(n, sizeof(int));
    int best_path_len = 0;

    if (!current_path || !best_path) {
        strcpy(result.status, "failed");
        strcpy(result.note, "memory allocation failed");
        free(sorted);
        if (current_path) free(current_path);
        if (best_path) free(best_path);
        return result;
    }

    result.estimated_memory_bytes = n * sizeof(Item) + n * sizeof(int) + n * sizeof(int) + sizeof(int);

    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);

    double best_value = 0;
    int best_weight = 0;
    long long visited = 0;
    int exceeded = 0;

    backtrack(sorted, n, capacity, 0, 0, 0.0, &best_value, &best_weight,
              current_path, best_path, &best_path_len, &visited, &exceeded);

    QueryPerformanceCounter(&end);
    result.elapsed_ms = (double)(end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;
    result.visited_nodes = visited;

    if (exceeded) {
        strcpy(result.status, "skipped");
        strcpy(result.note, "skipped because backtracking exceeded safe node limit");
        result.total_value = 0;
        result.total_weight = 0;
        result.selected_count = 0;
    } else {
        result.total_value = best_value;
        result.total_weight = best_weight;

        int count = 0;
        for (int i = 0; i < best_path_len; i++) {
            if (best_path[i]) count++;
        }
        result.selected_count = count;
        if (count > 0) {
            result.selected_item_ids = (int *)malloc(count * sizeof(int));
            if (result.selected_item_ids) {
                int idx = 0;
                for (int i = 0; i < best_path_len; i++) {
                    if (best_path[i]) {
                        result.selected_item_ids[idx++] = sorted[i].id;
                    }
                }
            }
        }

        strcpy(result.status, "completed");
        strcpy(result.note, "");
    }

    free(sorted);
    free(current_path);
    free(best_path);
    return result;
}
