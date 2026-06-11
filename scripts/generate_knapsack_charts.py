import sys
import csv
import matplotlib.pyplot as plt
import os

# 移除用户目录中的包路径，优先使用 Anaconda 中的版本
new_path = []
for p in sys.path:
    if 'AppData\\Roaming\\Python' not in p:
        new_path.append(p)
sys.path = new_path

# 设置中文字体
plt.rcParams['font.sans-serif'] = ['Microsoft YaHei', 'SimHei', 'Arial Unicode MS']
plt.rcParams['axes.unicode_minus'] = False

# 确保目录存在
os.makedirs("figures", exist_ok=True)

# 读取规模结果数据
scale_results = []
with open('results/knapsack/required_scale_results.csv', 'r', encoding='utf-8') as f:
    reader = csv.DictReader(f)
    for row in reader:
        scale_results.append(row)

# 提取数据
capacities = [10000, 100000, 1000000]
scales = [1000, 2000, 5000, 10000, 20000, 40000, 80000, 160000, 320000]

# 执行时间数据
dp_time = {10000: [], 100000: [], 1000000: []}
greedy_time = {10000: [], 100000: [], 1000000: []}
valid_scales_time = {10000: [], 100000: [], 1000000: []}

# 内存数据
dp_mem = {10000: [], 100000: [], 1000000: []}
greedy_mem = {10000: [], 100000: [], 1000000: []}

for capacity in capacities:
    for n in scales:
        for row in scale_results:
            if int(row['n']) == n and int(row['capacity']) == capacity:
                if row['algorithm'] == 'dynamic_programming_optimized':
                    if row['status'] == 'completed':
                        dp_time[capacity].append(float(row['elapsed_ms']))
                        dp_mem[capacity].append(int(row['estimated_memory_bytes']) / (1024 * 1024))
                        valid_scales_time[capacity].append(n)
                    else:
                        dp_time[capacity].append(None)
                        dp_mem[capacity].append(None)
                elif row['algorithm'] == 'greedy':
                    greedy_time[capacity].append(float(row['elapsed_ms']))
                    greedy_mem[capacity].append(int(row['estimated_memory_bytes']) / (1024 * 1024))

# 1. 生成执行时间图
plt.figure(figsize=(12, 8))

# 使用与排序图相同的颜色和风格
colors = ['#1f77b4', '#2ca02c', '#ff7f0e']  # 蓝、绿、橙
markers = ['o', 's', '^']
linestyles = ['-', '--', '-.']

for i, capacity in enumerate(capacities):
    # DP 算法
    valid_dp_time = []
    valid_scales = []
    for j, t in enumerate(dp_time[capacity]):
        if t is not None:
            valid_dp_time.append(t)
            valid_scales.append(valid_scales_time[capacity][j])
    
    if valid_scales:
        plt.plot(valid_scales, valid_dp_time, marker=markers[i], 
                 label=f'DP (cap={capacity})', linestyle=linestyles[i], 
                 linewidth=2, markersize=8, color=colors[i])
    
    # Greedy 算法
    if greedy_time[capacity]:
        plt.plot(scales, greedy_time[capacity], marker=markers[i], 
                 label=f'Greedy (cap={capacity})', linestyle=linestyles[i], 
                 linewidth=2, markersize=8, color=colors[i], alpha=0.7)

plt.xscale('log')
plt.yscale('log')
plt.xlabel('物品数量 n', fontsize=12)
plt.ylabel('执行时间 (ms)', fontsize=12)
plt.title('背包算法执行时间对比', fontsize=14)
plt.legend(fontsize=11, loc='upper left')
plt.grid(True, which="both", ls="-", alpha=0.5)
plt.tight_layout()
plt.savefig('figures/knapsack_elapsed_time.png', dpi=300, bbox_inches='tight')
plt.close()

# 2. 生成内存使用图
plt.figure(figsize=(12, 8))

for i, capacity in enumerate(capacities):
    # DP 算法
    valid_dp_mem = []
    valid_scales = []
    for j, m in enumerate(dp_mem[capacity]):
        if m is not None:
            valid_dp_mem.append(m)
            valid_scales.append(valid_scales_time[capacity][j])
    
    if valid_scales:
        plt.plot(valid_scales, valid_dp_mem, marker=markers[i], 
                 label=f'DP (cap={capacity})', linestyle=linestyles[i], 
                 linewidth=2, markersize=8, color=colors[i])
    
    # Greedy 算法
    if greedy_mem[capacity]:
        plt.plot(scales, greedy_mem[capacity], marker=markers[i], 
                 label=f'Greedy (cap={capacity})', linestyle=linestyles[i], 
                 linewidth=2, markersize=8, color=colors[i], alpha=0.7)

plt.xscale('log')
plt.xlabel('物品数量 n', fontsize=12)
plt.ylabel('估算内存 (MB)', fontsize=12)
plt.title('背包算法内存使用对比', fontsize=14)
plt.legend(fontsize=11, loc='upper left')
plt.grid(True, which="both", ls="-", alpha=0.5)
plt.tight_layout()
plt.savefig('figures/knapsack_memory_usage.png', dpi=300, bbox_inches='tight')
plt.close()

# 3. 生成解质量对比图
quality_results = []
with open('results/knapsack/required_solution_quality.csv', 'r', encoding='utf-8') as f:
    reader = csv.DictReader(f)
    for row in reader:
        quality_results.append(row)

plt.figure(figsize=(12, 8))

for i, capacity in enumerate(capacities):
    n_vals = []
    gaps = []
    for row in quality_results:
        if int(row["capacity"]) == capacity:
            gap_val = row["greedy_gap_percent"]
            if gap_val and gap_val.strip():
                n_vals.append(int(row["n"]))
                gaps.append(float(gap_val))
    
    if n_vals:
        plt.plot(n_vals, gaps, marker=markers[i], 
                 label=f'cap={capacity}', linestyle=linestyles[i], 
                 linewidth=2, markersize=8, color=colors[i])

plt.xscale('log')
plt.xlabel('物品数量 n', fontsize=12)
plt.ylabel('贪心算法差距 (%)', fontsize=12)
plt.title('背包算法解质量对比', fontsize=14)
plt.legend(fontsize=11, loc='upper left')
plt.grid(True, which="both", ls="-", alpha=0.5)
plt.tight_layout()
plt.savefig('figures/knapsack_solution_quality.png', dpi=300, bbox_inches='tight')
plt.close()

print("背包图表生成成功!")
print("  - figures/knapsack_elapsed_time.png")
print("  - figures/knapsack_memory_usage.png")
print("  - figures/knapsack_solution_quality.png")
