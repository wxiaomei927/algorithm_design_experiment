#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "knapsack.h"

void generate_items(Item **items, int n, unsigned int seed) {
    srand(seed);
    *items = (Item *)malloc(n * sizeof(Item));
    for (int i = 0; i < n; i++) {
        (*items)[i].id = i + 1;
        (*items)[i].weight = 1 + rand() % 100;
        double raw = 100.0 + (rand() % 90001) / 100.0;
        (*items)[i].value = ((int)(raw * 100)) / 100.0;
        (*items)[i].ratio = (*items)[i].value / (*items)[i].weight;
    }
}

void free_items(Item *items) {
    if (items) {
        free(items);
    }
}

void free_result(KnapsackResult *result) {
    if (result && result->selected_item_ids) {
        free(result->selected_item_ids);
        result->selected_item_ids = NULL;
    }
}

void save_items_to_csv(Item *items, int n, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) return;
    fprintf(fp, "id,weight,value,ratio\n");
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%d,%d,%.2f,%.6f\n", items[i].id, items[i].weight, items[i].value, items[i].ratio);
    }
    fclose(fp);
}

static int find_item_index(Item *items, int n, int id) {
    for (int i = 0; i < n; i++) {
        if (items[i].id == id) return i;
    }
    return -1;
}

void validate_selected_items(Item *items, int n, int capacity, KnapsackResult *result) {
    if (result->selected_count <= 0 || result->selected_item_ids == NULL) {
        return;
    }

    int *id_check = (int *)calloc(n + 1, sizeof(int));
    if (!id_check) {
        return;
    }

    int calc_weight = 0;
    double calc_value = 0.0;
    int valid = 1;

    for (int i = 0; i < result->selected_count; i++) {
        int id = result->selected_item_ids[i];

        // 检查id范围
        if (id < 1 || id > n) {
            valid = 0;
            strcpy(result->note, "invalid item id");
            break;
        }

        // 检查重复
        if (id_check[id] > 0) {
            valid = 0;
            strcpy(result->note, "duplicate item id");
            break;
        }
        id_check[id]++;

        // 从原始items重新计算
        int idx = find_item_index(items, n, id);
        if (idx < 0) {
            valid = 0;
            strcpy(result->note, "item not found");
            break;
        }
        calc_weight += items[idx].weight;
        calc_value += items[idx].value;
    }

    if (valid) {
        // 检查重量匹配
        if (calc_weight != result->total_weight) {
            valid = 0;
            strcpy(result->note, "weight mismatch");
        }
        // 检查价值匹配
        else if (fabs(calc_value - result->total_value) > 1e-6) {
            valid = 0;
            strcpy(result->note, "value mismatch");
        }
        // 检查不超过容量
        else if (calc_weight > capacity) {
            valid = 0;
            strcpy(result->note, "exceeds capacity");
        }
    }

    free(id_check);

    if (!valid) {
        strcpy(result->status, "failed");
        free(result->selected_item_ids);
        result->selected_item_ids = NULL;
        result->selected_count = 0;
        result->total_weight = 0;
        result->total_value = 0.0;
    }
}
