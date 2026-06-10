#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "knapsack.h"
#include "knapsack_test.h"

#define EPSILON 1e-6

static int find_item_index(Item *items, int n, int id) {
    for (int i = 0; i < n; i++) {
        if (items[i].id == id) return i;
    }
    return -1;
}

static int validate_result(Item *items, int n, int capacity, KnapsackResult *result) {
    if (result->total_weight > capacity) return 0;
    double calc_value = 0;
    int calc_weight = 0;
    for (int i = 0; i < result->selected_count; i++) {
        int idx = find_item_index(items, n, result->selected_item_ids[i]);
        if (idx < 0) return 0;
        calc_value += items[idx].value;
        calc_weight += items[idx].weight;
    }
    if (fabs(calc_value - result->total_value) > EPSILON) return 0;
    if (calc_weight != result->total_weight) return 0;
    return 1;
}

static void save_validation_csv(const char *filename, int test_id, int n, int capacity,
                                unsigned int seed, KnapsackResult *results, int num_algorithms) {
    FILE *fp = fopen(filename, "a");
    if (!fp) return;
    for (int i = 0; i < num_algorithms; i++) {
        KnapsackResult *r = &results[i];
        if (strcmp(r->status, "completed") == 0) {
            fprintf(fp, "%d,%d,%d,%u,%s,%d,%.2f,%.6f,%lld,%lld,%s,%s\n",
                    test_id, n, capacity, seed, r->algorithm,
                    r->total_weight, r->total_value, r->elapsed_ms,
                    r->estimated_memory_bytes, r->visited_nodes,
                    r->status, r->note);
        } else {
            fprintf(fp, "%d,%d,%d,%u,%s,,,,,,%s,%s\n",
                    test_id, n, capacity, seed, r->algorithm,
                    r->status, r->note);
        }
    }
    fclose(fp);
}

static void save_selected_items_csv(const char *filename, int test_id, int n, int capacity,
                                    unsigned int seed, KnapsackResult *result, Item *items) {
    FILE *fp = fopen(filename, "a");
    if (!fp) return;
    if (strcmp(result->status, "completed") == 0 && result->selected_count > 0) {
        for (int i = 0; i < result->selected_count; i++) {
            int idx = find_item_index(items, n, result->selected_item_ids[i]);
            if (idx >= 0) {
                fprintf(fp, "%d,%d,%d,%u,%s,%d,%d,%.2f,%.6f\n",
                        test_id, n, capacity, seed, result->algorithm,
                        items[idx].id, items[idx].weight, items[idx].value, items[idx].ratio);
            }
        }
    }
    fclose(fp);
}

static void print_test_summary(int test_id, int n, int capacity, unsigned int seed,
                               KnapsackResult *results, int num_algorithms) {
    printf("\n--- 测试 %d (n=%d, capacity=%d, seed=%u) ---\n", test_id, n, capacity, seed);
    for (int i = 0; i < num_algorithms; i++) {
        KnapsackResult *r = &results[i];
        if (strcmp(r->status, "completed") == 0) {
            printf("%-20s: weight=%d, value=%.2f, time=%.4f ms, nodes=%lld\n",
                   r->algorithm, r->total_weight, r->total_value, r->elapsed_ms, r->visited_nodes);
        } else {
            printf("%-20s: %s - %s\n", r->algorithm, r->status, r->note);
        }
    }
}

void run_knapsack_small_scale_validation() {
    printf("\n===== 0-1 背包小规模正确性验证 =====\n\n");

    int tests[][3] = {
        {10, 100, 20260611},
        {15, 300, 20260612},
        {20, 500, 20260613}
    };
    int num_tests = 3;

    const char *val_file = "results/knapsack/small_scale_validation.csv";
    const char *sel_file = "results/knapsack/small_scale_selected_items.csv";

    FILE *fp_val = fopen(val_file, "w");
    if (fp_val) {
        fprintf(fp_val, "test_id,n,capacity,seed,algorithm,total_weight,total_value,elapsed_ms,estimated_memory_bytes,visited_nodes,status,note\n");
        fclose(fp_val);
    }

    FILE *fp_sel = fopen(sel_file, "w");
    if (fp_sel) {
        fprintf(fp_sel, "test_id,n,capacity,seed,algorithm,item_id,weight,value,ratio\n");
        fclose(fp_sel);
    }

    int all_pass = 1;

    for (int t = 0; t < num_tests; t++) {
        int n = tests[t][0];
        int capacity = tests[t][1];
        unsigned int seed = (unsigned int)tests[t][2];

        Item *items;
        generate_items(&items, n, seed);

        KnapsackResult results[4];
        results[0] = brute_force_knapsack(items, n, capacity);
        results[1] = dynamic_programming_knapsack(items, n, capacity);
        results[2] = greedy_knapsack(items, n, capacity);
        results[3] = backtracking_knapsack(items, n, capacity);

        print_test_summary(t + 1, n, capacity, seed, results, 4);

        double opt_value = -1;
        for (int i = 0; i < 4; i++) {
            if (strcmp(results[i].status, "completed") == 0) {
                if (!validate_result(items, n, capacity, &results[i])) {
                    printf("FAIL: %s 结果验证失败（重量或价值不匹配）\n", results[i].algorithm);
                    all_pass = 0;
                }
                if (strcmp(results[i].algorithm, "brute_force") == 0 ||
                    strcmp(results[i].algorithm, "dynamic_programming") == 0 ||
                    strcmp(results[i].algorithm, "backtracking") == 0) {
                    if (opt_value < 0 || fabs(results[i].total_value - opt_value) <= EPSILON) {
                        opt_value = results[i].total_value;
                    } else {
                        printf("FAIL: %s 最优值 %.2f 与其他最优算法不一致\n",
                               results[i].algorithm, results[i].total_value);
                        all_pass = 0;
                    }
                }
                if (strcmp(results[i].algorithm, "greedy") == 0) {
                    if (opt_value > 0 && results[i].total_value > opt_value + EPSILON) {
                        printf("FAIL: 贪心法价值 %.2f 超过最优值 %.2f\n",
                               results[i].total_value, opt_value);
                        all_pass = 0;
                    }
                }
            }
        }

        if (opt_value < 0) {
            printf("WARNING: 没有 completed 的最优算法\n");
        }

        save_validation_csv(val_file, t + 1, n, capacity, seed, results, 4);
        for (int i = 0; i < 4; i++) {
            save_selected_items_csv(sel_file, t + 1, n, capacity, seed, &results[i], items);
        }

        for (int i = 0; i < 4; i++) {
            free_result(&results[i]);
        }
        free_items(items);
    }

    printf("\n===== 生成 1000 个物品数据 =====\n");
    Item *items_1000;
    generate_items(&items_1000, 1000, 20260620);
    save_items_to_csv(items_1000, 1000, "data/knapsack/items_1000.csv");
    printf("已生成 data/knapsack/items_1000.csv\n");
    free_items(items_1000);

    printf("\n===== 验证结果 =====\n");
    if (all_pass) {
        printf("所有测试: PASS\n");
    } else {
        printf("部分测试: FAIL\n");
    }
    printf("\nCSV 输出:\n");
    printf("  - %s\n", val_file);
    printf("  - %s\n", sel_file);
}
