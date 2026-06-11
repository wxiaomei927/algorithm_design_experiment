#ifndef KNAPSACK_H
#define KNAPSACK_H

typedef struct {
    int id;
    int weight;
    double value;
    double ratio;
} Item;

typedef struct {
    char algorithm[64];
    char status[32];
    int total_weight;
    double total_value;
    int selected_count;
    int *selected_item_ids;
    double elapsed_ms;
    long long estimated_memory_bytes;
    long long visited_nodes;
    long long estimated_operations;
    char note[256];
} KnapsackResult;

void generate_items(Item **items, int n, unsigned int seed);
void free_items(Item *items);
void free_result(KnapsackResult *result);
void save_items_to_csv(Item *items, int n, const char *filename);
void validate_selected_items(Item *items, int n, int capacity, KnapsackResult *result);

KnapsackResult brute_force_knapsack(Item *items, int n, int capacity);
KnapsackResult dynamic_programming_knapsack(Item *items, int n, int capacity);
KnapsackResult greedy_knapsack(Item *items, int n, int capacity);
KnapsackResult backtracking_knapsack(Item *items, int n, int capacity);
KnapsackResult dynamic_programming_optimized_knapsack(Item *items, int n, int capacity);

#endif
