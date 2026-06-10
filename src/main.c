#include <stdio.h>
#include <stdlib.h>

#include "sorting/sorting_test.h"
#include "knapsack/knapsack_test.h"

int main(int argc, char *argv[]) {
    int choice;

    if (argc > 1) {
        choice = atoi(argv[1]);
    } else {
        printf("\n===== 算法设计与分析实验 =====\n");
        printf("1. 运行排序正确性测试\n");
        printf("2. 运行两组 100 个随机数排序实验\n");
        printf("3. 运行完整排序批量实验\n");
        printf("4. 运行背包小规模正确性验证\n");
        printf("5. 退出\n");
        printf("===============================\n");
        printf("请输入选择: ");

        if (scanf("%d", &choice) != 1) {
            printf("输入错误，请输入数字\n");
            return 1;
        }
    }

    switch (choice) {
        case 1:
            runSortingTests();
            break;
        case 2:
            run_sorting_two_sets_experiment();
            break;
        case 3:
            run_full_sorting_experiment();
            break;
        case 4:
            run_knapsack_small_scale_validation();
            break;
        case 5:
            printf("程序退出\n");
            return 0;
        default:
            printf("无效选项\n");
            return 1;
    }

    return 0;
}
