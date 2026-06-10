import csv
import matplotlib.pyplot as plt
import os

# 设置中文字体
plt.rcParams['font.sans-serif'] = ['Microsoft YaHei', 'SimHei', 'Arial Unicode MS']
plt.rcParams['axes.unicode_minus'] = False

# 读取排序结果数据
results = []
with open('results/sorting/scaled_sorting_results.csv', 'r') as f:
    reader = csv.DictReader(f)
    for row in reader:
        results.append(row)

# 提取数据
scales = [10, 100, 1000, 2000, 5000, 10000, 100000]

# 比较次数数据
bubble_comparisons = []
merge_comparisons = []
quick_comparisons = []

# 执行时间数据
bubble_time = []
merge_time = []
quick_time = []

for n in scales:
    for row in results:
        if int(row['n']) == n:
            if row['algorithm'] == 'bubble_sort':
                if row['status'] == 'completed':
                    bubble_comparisons.append(int(row['comparisons']))
                    bubble_time.append(float(row['elapsed_ms']))
                else:
                    bubble_comparisons.append(None)
                    bubble_time.append(None)
            elif row['algorithm'] == 'merge_sort':
                merge_comparisons.append(int(row['comparisons']))
                merge_time.append(float(row['elapsed_ms']))
            elif row['algorithm'] == 'quick_sort':
                quick_comparisons.append(int(row['comparisons']))
                quick_time.append(float(row['elapsed_ms']))

# 创建比较次数图
plt.figure(figsize=(12, 8))

# 过滤掉 None 值用于绘图
valid_scales_cmp = []
valid_bubble_cmp = []
valid_merge_cmp = []
valid_quick_cmp = []

for i, n in enumerate(scales):
    if bubble_comparisons[i] is not None:
        valid_scales_cmp.append(n)
        valid_bubble_cmp.append(bubble_comparisons[i])
        valid_merge_cmp.append(merge_comparisons[i])
        valid_quick_cmp.append(quick_comparisons[i])

plt.plot(valid_scales_cmp, valid_bubble_cmp, marker='o', label='Bubble Sort', linestyle='-', linewidth=2, markersize=8, color='#1f77b4')
plt.plot(scales, merge_comparisons, marker='s', label='Merge Sort', linestyle='--', linewidth=2, markersize=8, color='#2ca02c')
plt.plot(scales, quick_comparisons, marker='^', label='Quick Sort', linestyle='-.', linewidth=2, markersize=8, color='#ff7f0e')

plt.xscale('log')
plt.yscale('log')
plt.xlabel('Input Size (n)', fontsize=12)
plt.ylabel('Number of Comparisons', fontsize=12)
plt.title('Sorting Algorithm Comparisons - Number of Comparisons vs Input Size', fontsize=14)
plt.legend(fontsize=11, loc='upper left')
plt.grid(True, which="both", ls="-", alpha=0.5)

# 添加理论复杂度参考线
import numpy as np
x_ref = np.array([10, 100000])
y_bubble_ref = x_ref ** 2 / 2
y_nlogn_ref = x_ref * np.log2(x_ref) * 2
plt.plot(x_ref, y_bubble_ref, 'b--', alpha=0.3, label='O(n^2) reference')
plt.plot(x_ref, y_nlogn_ref, 'g--', alpha=0.3, label='O(n log n) reference')

plt.tight_layout()
plt.savefig('figures/sorting_comparisons.png', dpi=300, bbox_inches='tight')
plt.close()

# 创建执行时间图
plt.figure(figsize=(12, 8))

valid_scales_time = []
valid_bubble_time = []
valid_merge_time = []
valid_quick_time = []

for i, n in enumerate(scales):
    if bubble_time[i] is not None:
        valid_scales_time.append(n)
        valid_bubble_time.append(bubble_time[i])
        valid_merge_time.append(merge_time[i])
        valid_quick_time.append(quick_time[i])

plt.plot(valid_scales_time, valid_bubble_time, marker='o', label='Bubble Sort', linestyle='-', linewidth=2, markersize=8, color='#1f77b4')
plt.plot(scales, merge_time, marker='s', label='Merge Sort', linestyle='--', linewidth=2, markersize=8, color='#2ca02c')
plt.plot(scales, quick_time, marker='^', label='Quick Sort', linestyle='-.', linewidth=2, markersize=8, color='#ff7f0e')

plt.xscale('log')
plt.yscale('log')
plt.xlabel('Input Size (n)', fontsize=12)
plt.ylabel('Execution Time (ms)', fontsize=12)
plt.title('Sorting Algorithm Comparisons - Execution Time vs Input Size', fontsize=14)
plt.legend(fontsize=11, loc='upper left')
plt.grid(True, which="both", ls="-", alpha=0.5)

plt.tight_layout()
plt.savefig('figures/sorting_elapsed_time.png', dpi=300, bbox_inches='tight')
plt.close()

print("Charts generated successfully!")
print("  - figures/sorting_comparisons.png")
print("  - figures/sorting_elapsed_time.png")
