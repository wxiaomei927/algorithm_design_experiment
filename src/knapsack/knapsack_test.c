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

void run_knapsack_scaled_experiment() {
    printf("\n===== 0-1 背包正式规模实验 =====\n\n");

    int tests[][2] = {
        {10, 100},
        {15, 300},
        {20, 500},
        {25, 1000},
        {30, 2000},
        {40, 5000},
        {100, 10000},
        {500, 50000},
        {1000, 100000}
    };
    int num_tests = 9;

    const char *results_file = "results/knapsack/scaled_knapsack_results.csv";
    const char *quality_file = "results/knapsack/solution_quality_comparison.csv";
    const char *nodes_file = "results/knapsack/search_nodes_comparison.csv";

    FILE *fp_results = fopen(results_file, "w");
    if (fp_results) {
        fprintf(fp_results, "n,capacity,seed,algorithm,total_weight,total_value,elapsed_ms,estimated_memory_bytes,visited_nodes,status,note\n");
        fclose(fp_results);
    }

    FILE *fp_quality = fopen(quality_file, "w");
    if (fp_quality) {
        fprintf(fp_quality, "n,capacity,seed,reference_algorithm,reference_value,greedy_value,greedy_gap,greedy_gap_percent,status,note\n");
        fclose(fp_quality);
    }

    FILE *fp_nodes = fopen(nodes_file, "w");
    if (fp_nodes) {
        fprintf(fp_nodes, "n,capacity,seed,brute_force_nodes,backtracking_nodes,backtracking_reduction_percent,status,note\n");
        fclose(fp_nodes);
    }

    for (int t = 0; t < num_tests; t++) {
        int n = tests[t][0];
        int capacity = tests[t][1];
        unsigned int seed = 20260701 + n;

        printf("\n--- 规模 %d: n=%d, capacity=%d, seed=%u ---\n", t + 1, n, capacity, seed);

        Item *items;
        generate_items(&items, n, seed);

        KnapsackResult results[4];
        printf("运行 brute_force... ");
        results[0] = brute_force_knapsack(items, n, capacity);
        printf("%s\n", results[0].status);

        printf("运行 dynamic_programming... ");
        results[1] = dynamic_programming_knapsack(items, n, capacity);
        printf("%s\n", results[1].status);

        printf("运行 greedy... ");
        results[2] = greedy_knapsack(items, n, capacity);
        printf("%s\n", results[2].status);

        printf("运行 backtracking... ");
        results[3] = backtracking_knapsack(items, n, capacity);
        printf("%s\n", results[3].status);

        for (int i = 0; i < 4; i++) {
            if (strcmp(results[i].status, "completed") == 0) {
                if (results[i].total_weight > capacity) {
                    printf("ERROR: %s total_weight=%d > capacity=%d\n",
                           results[i].algorithm, results[i].total_weight, capacity);
                }
            }
        }

        fp_results = fopen(results_file, "a");
        if (fp_results) {
            for (int i = 0; i < 4; i++) {
                KnapsackResult *r = &results[i];
                if (strcmp(r->status, "completed") == 0) {
                    fprintf(fp_results, "%d,%d,%u,%s,%d,%.2f,%.6f,%lld,%lld,%s,%s\n",
                            n, capacity, seed, r->algorithm,
                            r->total_weight, r->total_value, r->elapsed_ms,
                            r->estimated_memory_bytes, r->visited_nodes,
                            r->status, r->note);
                } else {
                    fprintf(fp_results, "%d,%d,%u,%s,,,,%lld,,%s,%s\n",
                            n, capacity, seed, r->algorithm,
                            r->estimated_memory_bytes,
                            r->status, r->note);
                }
            }
            fclose(fp_results);
        }

        double dp_value = -1;
        double greedy_value = -1;
        if (strcmp(results[1].status, "completed") == 0) {
            dp_value = results[1].total_value;
        }
        if (strcmp(results[2].status, "completed") == 0) {
            greedy_value = results[2].total_value;
        }

        fp_quality = fopen(quality_file, "a");
        if (fp_quality) {
            if (dp_value >= 0 && greedy_value >= 0) {
                double gap = dp_value - greedy_value;
                double gap_percent = gap / dp_value * 100.0;
                if (gap < -EPSILON) {
                    fprintf(fp_quality, "%d,%d,%u,dynamic_programming,%.2f,%.2f,ERROR,ERROR,error,greedy_value exceeds reference_value\n",
                            n, capacity, seed, dp_value, greedy_value);
                } else {
                    fprintf(fp_quality, "%d,%d,%u,dynamic_programming,%.2f,%.2f,%.2f,%.2f,completed,\n",
                            n, capacity, seed, dp_value, greedy_value, gap, gap_percent);
                }
            } else if (greedy_value >= 0) {
                fprintf(fp_quality, "%d,%d,%u,,,%f,,,unavailable,DP skipped because estimated memory exceeds 512 MB; greedy value retained but optimality gap unavailable\n",
                        n, capacity, seed, greedy_value);
            } else {
                fprintf(fp_quality, "%d,%d,%u,,,,,,unavailable,DP skipped or greedy failed\n",
                        n, capacity, seed);
            }
            fclose(fp_quality);
        }

        fp_nodes = fopen(nodes_file, "a");
        if (fp_nodes) {
            if (strcmp(results[0].status, "completed") == 0 &&
                strcmp(results[3].status, "completed") == 0) {
                long long bf_nodes = results[0].visited_nodes;
                long long bt_nodes = results[3].visited_nodes;
                double reduction = (double)(bf_nodes - bt_nodes) / bf_nodes * 100.0;
                fprintf(fp_nodes, "%d,%d,%u,%lld,%lld,%.2f,completed,\n",
                        n, capacity, seed, bf_nodes, bt_nodes, reduction);
            } else {
                fprintf(fp_nodes, "%d,%d,%u,,,,unavailable,brute_force or backtracking skipped\n",
                        n, capacity, seed);
            }
            fclose(fp_nodes);
        }

        for (int i = 0; i < 4; i++) {
            free_result(&results[i]);
        }
        free_items(items);
    }

    printf("\n===== 正式规模实验完成 =====\n");
    printf("\nCSV 输出:\n");
    printf("  - %s\n", results_file);
    printf("  - %s\n", quality_file);
    printf("  - %s\n", nodes_file);
}

void run_knapsack_required_experiment() {
    printf("\n===== 作业要求规模的背包实验 =====\n\n");

    const char *results_file = "results/knapsack/required_scale_results.csv";
    const char *quality_file = "results/knapsack/required_solution_quality.csv";
    const char *items_file = "results/knapsack/required_selected_items.csv";

    FILE *fp_results = fopen(results_file, "w");
    if (fp_results) {
        fprintf(fp_results, "n,capacity,seed,algorithm,total_weight,total_value,elapsed_ms,estimated_memory_bytes,estimated_operations,visited_nodes,status,note\n");
        fclose(fp_results);
    }

    FILE *fp_quality = fopen(quality_file, "w");
    if (fp_quality) {
        fprintf(fp_quality, "n,capacity,seed,reference_algorithm,reference_value,greedy_value,greedy_gap,greedy_gap_percent,status,note\n");
        fclose(fp_quality);
    }

    FILE *fp_items = fopen(items_file, "w");
    if (fp_items) {
        fprintf(fp_items, "n,capacity,seed,algorithm,item_id,weight,value,ratio\n");
        fclose(fp_items);
    }

    int n_values[] = {1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000, 20000, 40000, 80000, 160000, 320000};
    int capacities[] = {10000, 100000, 1000000};
    int num_n = sizeof(n_values) / sizeof(n_values[0]);
    int num_cap = sizeof(capacities) / sizeof(capacities[0]);

    for (int i = 0; i < num_n; i++) {
        int n = n_values[i];
        unsigned int seed = 20260801 + n;

        Item *items = NULL;
        generate_items(&items, n, seed);
        
        // 保存 n=1000 的物品数据
        if (n == 1000) {
            save_items_to_csv(items, n, "data/knapsack/required_items_1000.csv");
        }

        for (int j = 0; j < num_cap; j++) {
            int capacity = capacities[j];
            printf("--- 规模 %d: n=%d, capacity=%d, seed=%u ---\n", i * num_cap + j + 1, n, capacity, seed);

            KnapsackResult results[4];

            printf("  执行 brute_force... ");
            results[0] = brute_force_knapsack(items, n, capacity);
            if (strcmp(results[0].status, "skipped") != 0) {
                strcpy(results[0].status, "skipped");
                strcpy(results[0].note, "skipped because brute force is O(2^n)");
                results[0].total_weight = 0;
                results[0].total_value = 0.0;
                results[0].elapsed_ms = -1;
            }
            printf("%s\n", results[0].status);

            printf("  执行 backtracking... ");
            results[1] = backtracking_knapsack(items, n, capacity);
            if (strcmp(results[1].status, "skipped") != 0) {
                strcpy(results[1].status, "skipped");
                strcpy(results[1].note, "skipped because backtracking worst-case complexity is O(2^n)");
                results[1].total_weight = 0;
                results[1].total_value = 0.0;
                results[1].elapsed_ms = -1;
            }
            printf("%s\n", results[1].status);

            printf("  执行 dynamic_programming_optimized... ");
            results[2] = dynamic_programming_optimized_knapsack(items, n, capacity);
            printf("%s\n", results[2].status);

            printf("  执行 greedy... ");
            results[3] = greedy_knapsack(items, n, capacity);
            printf("%s\n", results[3].status);

            fp_results = fopen(results_file, "a");
            if (fp_results) {
                for (int k = 0; k < 4; k++) {
                    KnapsackResult *r = &results[k];
                    if (strcmp(r->status, "completed") == 0) {
                        fprintf(fp_results, "%d,%d,%u,%s,%d,%.2f,%.6f,%lld,%lld,%lld,%s,%s\n",
                                n, capacity, seed, r->algorithm,
                                r->total_weight, r->total_value, r->elapsed_ms,
                                r->estimated_memory_bytes, r->estimated_operations,
                                r->visited_nodes, r->status, r->note);
                    } else {
                        fprintf(fp_results, "%d,%d,%u,%s,,,,%lld,%lld,,%s,%s\n",
                                n, capacity, seed, r->algorithm,
                                r->estimated_memory_bytes, r->estimated_operations,
                                r->status, r->note);
                    }
                }
                fclose(fp_results);
            }

            double dp_value = -1;
            double greedy_value = -1;
            if (strcmp(results[2].status, "completed") == 0) {
                dp_value = results[2].total_value;
            }
            if (strcmp(results[3].status, "completed") == 0) {
                greedy_value = results[3].total_value;
            }

            fp_quality = fopen(quality_file, "a");
            if (fp_quality) {
                if (dp_value >= 0 && greedy_value >= 0) {
                    double gap = dp_value - greedy_value;
                    double gap_percent = gap / dp_value * 100.0;
                    fprintf(fp_quality, "%d,%d,%u,dynamic_programming_optimized,%.2f,%.2f,%.2f,%.2f,completed,\n",
                            n, capacity, seed, dp_value, greedy_value, gap, gap_percent);
                } else if (greedy_value >= 0) {
                    fprintf(fp_quality, "%d,%d,%u,,,%f,,,unavailable,DP skipped; greedy value retained but optimality gap unavailable\n",
                            n, capacity, seed, greedy_value);
                } else {
                    fprintf(fp_quality, "%d,%d,%u,,,,,,unavailable,DP skipped or greedy failed\n",
                            n, capacity, seed);
                }
                fclose(fp_quality);
            }

            fp_items = fopen(items_file, "a");
            if (fp_items) {
                for (int k = 0; k < 4; k++) {
                    KnapsackResult *r = &results[k];
                    if (strcmp(r->status, "completed") == 0 && r->selected_count > 0 && r->selected_item_ids != NULL) {
                        for (int m = 0; m < r->selected_count; m++) {
                            int item_id = r->selected_item_ids[m];
                            int idx = find_item_index(items, n, item_id);
                            if (idx >= 0) {
                                fprintf(fp_items, "%d,%d,%u,%s,%d,%d,%.2f,%.4f\n",
                                        n, capacity, seed, r->algorithm,
                                        items[idx].id, items[idx].weight,
                                        items[idx].value, items[idx].ratio);
                            }
                        }
                    }
                }
                fclose(fp_items);
            }

            for (int k = 0; k < 4; k++) {
                free_result(&results[k]);
            }
        }

        free_items(items);
    }

    printf("\n===== 作业要求规模实验完成 =====\n");
    printf("\nCSV 输出:\n");
    printf("  - %s\n", results_file);
    printf("  - %s\n", quality_file);
    printf("  - %s\n", items_file);
}
