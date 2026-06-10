# 算法设计与分析实验项目

本项目用于完成《算法设计与分析》课程实验，包含排序算法和 0-1 背包问题的多种实现。

## 目录结构

```
algorithm-design-experiment/
├── README.md          # 项目说明文档
├── .gitignore         # Git 忽略规则
├── Makefile           # 编译脚本
├── src/               # 源代码目录
│   ├── main.c         # 主程序入口
│   ├── common.h       # 公共头文件
│   ├── common.c       # 公共实现
│   ├── sorting/       # 排序算法模块
│   └── knapsack/      # 背包问题模块
├── data/              # 测试数据目录
├── results/           # 实验结果目录（CSV 文件）
├── figures/           # 图表目录
├── report/            # 实验报告目录
└── docs/              # 文档目录
```

## 编译命令

```bash
make
```

## 运行命令

```bash
./algorithm_experiment
```

## 功能模块

### 排序算法
- 冒泡排序
- 合并排序（归并排序）
- 快速排序

### 0-1 背包问题
- 蛮力法
- 动态规划法
- 贪心法
- 回溯法

## 实验功能
- 随机测试数据生成
- 算法运行时间统计
- 空间占用估算
- CSV 结果导出
- 图表生成