#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
