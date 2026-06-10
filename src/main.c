#include <stdio.h>
#include <stdlib.h>

void runSortingTests();

int main() {
    int choice;
    
    while (1) {
        printf("\n===== 算法设计与分析实验 =====\n");
        printf("1. 运行排序实验\n");
        printf("2. 运行 0-1 背包实验\n");
        printf("0. 退出\n");
        printf("===============================\n");
        printf("请输入选择: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("输入错误，请输入数字\n");
            while (getchar() != '\n');
            continue;
        }
        
        switch (choice) {
            case 1:
                runSortingTests();
                break;
            case 2:
                printf("\n背包实验尚未实现\n");
                break;
            case 0:
                printf("程序退出\n");
                return 0;
            default:
                printf("无效选项，请重新输入\n");
        }
    }
    
    return 0;
}