#include <stdio.h>
#include <stdlib.h>

#include "sorting/sorting_test.h"

int main(int argc, char *argv[]) {
    int choice;
    
    // 支持命令行参数
    if (argc > 1) {
        choice = atoi(argv[1]);
    } else {
        printf("\n===== 算法设计与分析实验 =====\n");
        printf("1. 运行排序正确性测试\n");
        printf("2. 运行两组 100 个随机数排序实验\n");
        printf("3. 运行完整排序批量实验\n");
        printf("4. 运行 0-1 背包实验\n");
        printf("0. 退出\n");
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
            printf("\n背包实验尚未实现\n");
            break;
        case 0:
            printf("程序退出\n");
            return 0;
        default:
            printf("无效选项\n");
            return 1;
    }
    
    return 0;
}