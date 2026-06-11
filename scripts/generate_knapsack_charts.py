import sys
import csv

# 移除用户目录中的包路径，优先使用 Anaconda 中的版本
new_path = []
for p in sys.path:
    if 'AppData\\Roaming\\Python' not in p:
        new_path.append(p)
sys.path = new_path

import matplotlib
matplotlib.use("Agg")
matplotlib.rcParams["font.family"] = ["SimHei", "WenQuanYi Micro Hei", "Heiti TC"]
matplotlib.rcParams["axes.unicode_minus"] = False
import matplotlib.pyplot as plt
import os

# 确保目录存在
os.makedirs("figures", exist_ok=True)

# 读取数据
def read_csv(filename):
    data = []
    with open(filename, "r", encoding="utf-8") as f:
        reader = csv.DictReader(f)
        for row in reader:
            data.append(row)
    return data

# 读取规模结果
scale_data = read_csv("results/knapsack/required_scale_results.csv")

# 1. 生成时间图
plt.figure(figsize=(12, 8))
capacities = [10000, 100000, 1000000]
algorithms = ["dynamic_programming_optimized", "greedy"]
# 使用6种不同颜色，参照排序图表风格
colors = {
    (10000, "dynamic_programming_optimized"): "#1f77b4",  # 蓝色
    (10000, "greedy"): "#ff7f0e",                         # 橙色
    (100000, "dynamic_programming_optimized"): "#2ca02c",  # 绿色
    (100000, "greedy"): "#d62728",                        # 红色
    (1000000, "dynamic_programming_optimized"): "#9467bd", # 紫色
    (1000000, "greedy"): "#8c564b",                       # 棕色
}
markers = {"dynamic_programming_optimized": "o", "greedy": "s"}

for capacity in capacities:
    for algo in algorithms:
        n_vals = []
        times = []
        for row in scale_data:
            if int(row["capacity"]) == capacity and row["algorithm"] == algo and row["status"] == "completed":
                n_vals.append(int(row["n"]))
                times.append(float(row["elapsed_ms"]))
        if n_vals:
            color = colors.get((capacity, algo), "gray")
            plt.plot(n_vals, times, marker=markers[algo], label=f"{algo} (cap={capacity})", 
                     color=color, linestyle="-", linewidth=2, markersize=8)

plt.xlabel("物品数量 n", fontsize=12)
plt.ylabel("执行时间 (ms)", fontsize=12)
plt.title("背包算法执行时间对比", fontsize=14)
plt.legend(fontsize=11, loc="upper left")
plt.grid(True, which="both", ls="-", alpha=0.5)
plt.yscale("log")
plt.tight_layout()
plt.savefig("figures/knapsack_elapsed_time.png", dpi=300, bbox_inches="tight")
plt.close()

# 2. 生成内存图
plt.figure(figsize=(12, 8))
for capacity in capacities:
    for algo in algorithms:
        n_vals = []
        mems = []
        for row in scale_data:
            if int(row["capacity"]) == capacity and row["algorithm"] == algo and row["status"] == "completed":
                n_vals.append(int(row["n"]))
                mems.append(int(row["estimated_memory_bytes"]) / (1024 * 1024))  # 转换为 MB
        if n_vals:
            color = colors.get((capacity, algo), "gray")
            plt.plot(n_vals, mems, marker=markers[algo], label=f"{algo} (cap={capacity})", 
                     color=color, linestyle="-", linewidth=2, markersize=8)

plt.xlabel("物品数量 n", fontsize=12)
plt.ylabel("估算内存 (MB)", fontsize=12)
plt.title("背包算法内存使用对比\n(注：此为核心数据结构的估算空间，非操作系统实测峰值内存)", fontsize=14)
plt.legend(fontsize=11, loc="upper left")
plt.grid(True, which="both", ls="-", alpha=0.5)
plt.tight_layout()
plt.savefig("figures/knapsack_memory_usage.png", dpi=300, bbox_inches="tight")
plt.close()

# 3. 生成解质量图
quality_data = read_csv("results/knapsack/required_solution_quality.csv")
plt.figure(figsize=(12, 8))
# 解质量图只有 greedy，按 capacity 分3条线
quality_colors = {10000: "#ff7f0e", 100000: "#d62728", 1000000: "#8c564b"}

for capacity in capacities:
    n_vals = []
    gaps = []
    for row in quality_data:
        if int(row["capacity"]) == capacity:
            gap_val = row["greedy_gap_percent"]
            if gap_val and gap_val.strip():  # 跳过空值
                n_vals.append(int(row["n"]))
                gaps.append(float(gap_val))
    if n_vals:
        color = quality_colors.get(capacity, "gray")
        plt.plot(n_vals, gaps, marker="^", label=f"cap={capacity}", 
                 color=color, linestyle="-", linewidth=2, markersize=8)

plt.xlabel("物品数量 n", fontsize=12)
plt.ylabel("贪心算法差距 (%)", fontsize=12)
plt.title("背包算法解质量对比", fontsize=14)
plt.legend(fontsize=11, loc="upper right")
plt.grid(True, which="both", ls="-", alpha=0.5)
plt.tight_layout()
plt.savefig("figures/knapsack_solution_quality.png", dpi=300, bbox_inches="tight")
plt.close()

print("背包图表生成成功!")
print("  - figures/knapsack_elapsed_time.png")
print("  - figures/knapsack_memory_usage.png")
print("  - figures/knapsack_solution_quality.png")
