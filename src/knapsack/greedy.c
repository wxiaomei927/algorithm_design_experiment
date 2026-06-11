#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "knapsack.h"

static int compare_ratio_desc(const void *a, const void *b) {
    Item *ia = (Item *)a;
    Item *ib = (Item *)b;
    if (ib->ratio > ia->ratio) return 1;
    if (ib->ratio < ia->ratio) return -1;
    return 0;
}

KnapsackResult greedy_knapsack(Item *items, int n, int capacity) {
    KnapsackResult result;
    memset(&result, 0, sizeof(result));
    strcpy(result.algorithm, "greedy");
    result.elapsed_ms = -1;
    result.visited_nodes = 0;
    result.estimated_memory_bytes = n * sizeof(Item) + n * sizeof(int);

    Item *sorted = (Item *)malloc(n * sizeof(Item));
    memcpy(sorted, items, n * sizeof(Item));
    qsort(sorted, n, sizeof(Item), compare_ratio_desc);

    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);

    int current_weight = 0;
    double current_value = 0;
    int count = 0;
    int *temp_ids = (int *)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        if (current_weight + sorted[i].weight <= capacity) {
            temp_ids[count++] = sorted[i].id;
            current_weight += sorted[i].weight;
            current_value += sorted[i].value;
        }
    }

    QueryPerformanceCounter(&end);
    result.elapsed_ms = (double)(end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;
    result.total_weight = current_weight;
    result.total_value = current_value;
    result.selected_count = count;
    result.selected_item_ids = (int *)malloc(count * sizeof(int));
    memcpy(result.selected_item_ids, temp_ids, count * sizeof(int));
    strcpy(result.status, "completed");
    strcpy(result.note, "approximation strategy; not guaranteed to be globally optimal for 0-1 knapsack");

    // 验证选择结果
    validate_selected_items(items, n, capacity, &result);

    free(sorted);
    free(temp_ids);
    return result;
}
