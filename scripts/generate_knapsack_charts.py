import csv
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
plt.figure(figsize=(12, 6))
capacities = [10000, 100000, 1000000]
algorithms = ["dynamic_programming_optimized", "greedy"]
colors = {"dynamic_programming_optimized": "blue", "greedy": "orange"}

for capacity in capacities:
    for algo in algorithms:
        n_vals = []
        times = []
        for row in scale_data:
            if int(row["capacity"]) == capacity and row["algorithm"] == algo and row["status"] == "completed":
                n_vals.append(int(row["n"]))
                times.append(float(row["elapsed_ms"]))
        if n_vals:
            plt.plot(n_vals, times, marker="o", label=f"{algo} (cap={capacity})", color=colors[algo])

plt.xlabel("物品数量 n")
plt.ylabel("执行时间 (ms)")
plt.title("背包算法执行时间对比")
plt.legend()
plt.grid(True)
plt.yscale("log")
plt.savefig("figures/knapsack_elapsed_time.png", dpi=300, bbox_inches="tight")
plt.close()

# 2. 生成内存图
plt.figure(figsize=(12, 6))
for capacity in capacities:
    for algo in algorithms:
        n_vals = []
        mems = []
        for row in scale_data:
            if int(row["capacity"]) == capacity and row["algorithm"] == algo and row["status"] == "completed":
                n_vals.append(int(row["n"]))
                mems.append(int(row["estimated_memory_bytes"]) / (1024 * 1024))  # 转换为 MB
        if n_vals:
            plt.plot(n_vals, mems, marker="s", label=f"{algo} (cap={capacity})", color=colors[algo])

plt.xlabel("物品数量 n")
plt.ylabel("估算内存 (MB)")
plt.title("背包算法内存使用对比\n(注：此为核心数据结构的估算空间，非操作系统实测峰值内存)")
plt.legend()
plt.grid(True)
plt.savefig("figures/knapsack_memory_usage.png", dpi=300, bbox_inches="tight")
plt.close()

# 3. 生成解质量图
quality_data = read_csv("results/knapsack/required_solution_quality.csv")
plt.figure(figsize=(12, 6))

for capacity in capacities:
    n_vals = []
    gaps = []
    for row in quality_data:
        if int(row["capacity"]) == capacity:
            n_vals.append(int(row["n"]))
            gaps.append(float(row["greedy_gap_percent"]))
    if n_vals:
        plt.plot(n_vals, gaps, marker="^", label=f"cap={capacity}")

plt.xlabel("物品数量 n")
plt.ylabel("贪心算法差距 (%)")
plt.title("背包算法解质量对比")
plt.legend()
plt.grid(True)
plt.savefig("figures/knapsack_solution_quality.png", dpi=300, bbox_inches="tight")
plt.close()

print("背包图表生成成功!")
print("  - figures/knapsack_elapsed_time.png")
print("  - figures/knapsack_memory_usage.png")
print("  - figures/knapsack_solution_quality.png")
