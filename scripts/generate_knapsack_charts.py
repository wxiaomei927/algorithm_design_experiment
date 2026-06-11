import sys
import csv
import os

# 移除用户目录中的包路径，优先使用系统 Python
new_path = []
for p in sys.path:
    if 'AppData\\Roaming\\Python' not in p:
        new_path.append(p)
sys.path = new_path

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

# ==================== 统一视觉规范配置 ====================
COLORS = {
    'dp': '#2F6690',        # 深蓝色
    'dp_secondary': '#3A7D8C',  # 蓝绿色
    'greedy': '#E07A5F',    # 橙色
    'greedy_secondary': '#C97B63',  # 棕橙色
    'greedy_tertiary': '#B65D4A',   # 深红橙色
    'bubble': '#277DA1',    # 蓝色
    'merge': '#4C956C',     # 绿色
    'quick': '#F4A261',     # 橙色
    'gray': '#6C757D'       # 辅助灰色
}

MARKERS = {
    'dp': 'o',
    'greedy': 's',
    'bubble': 'o',
    'merge': 's',
    'quick': '^'
}

LINESTYLES = {
    'c1': '-',    # C=10000
    'c2': '--',   # C=100000
    'c3': ':'     # C=1000000
}

# ==================== 统一学术风格辅助函数 ====================
def apply_academic_style(ax, title, xlabel, ylabel, xlog=False, ylog=False):
    """应用统一的学术报告风格"""
    # 设置标题
    ax.set_title(title, fontsize=16, pad=15, fontweight='normal')
    
    # 设置坐标轴标签
    ax.set_xlabel(xlabel, fontsize=13)
    ax.set_ylabel(ylabel, fontsize=13)
    
    # 设置刻度字号
    ax.tick_params(axis='both', labelsize=11)
    
    # 设置网格
    ax.grid(True, which='major', alpha=0.25, color='#DDDDDD', linewidth=0.8)
    ax.grid(True, which='minor', alpha=0.1, color='#EEEEEE', linewidth=0.5)
    
    # 设置对数坐标
    if xlog:
        ax.set_xscale('log')
    if ylog:
        ax.set_yscale('log')
    
    # 设置背景
    ax.set_facecolor('white')
    
    return ax

def configure_legend(ax, loc='upper left'):
    """配置图例样式"""
    legend = ax.legend(fontsize=10, loc=loc, 
                       facecolor='white', edgecolor='#CCCCCC', 
                       framealpha=0.9, borderpad=0.8)
    return legend

# ==================== 数据读取函数 ====================
def read_csv(filename):
    data = []
    with open(filename, 'r', encoding='utf-8') as f:
        reader = csv.DictReader(f)
        for row in reader:
            data.append(row)
    return data

# ==================== 主函数 ====================
def main():
    # 确保目录存在
    os.makedirs("figures", exist_ok=True)
    
    # 读取数据
    scale_data = read_csv("results/knapsack/required_scale_results.csv")
    quality_data = read_csv("results/knapsack/required_solution_quality.csv")
    
    # ==================== 1. 背包执行时间总览图 ====================
    fig, ax = plt.subplots(figsize=(10, 6))
    
    # DP, C=10000
    n_vals, times = [], []
    for row in scale_data:
        if int(row['capacity']) == 10000 and row['algorithm'] == 'dynamic_programming_optimized':
            if row['status'] == 'completed' and row['elapsed_ms'] and row['elapsed_ms'].strip():
                n_vals.append(int(row['n']))
                times.append(float(row['elapsed_ms']))
    if n_vals:
        sorted_pairs = sorted(zip(n_vals, times))
        n_sorted, t_sorted = zip(*sorted_pairs)
        ax.plot(n_sorted, t_sorted, marker=MARKERS['dp'], linestyle=LINESTYLES['c1'],
                label='DP, C=10,000', color=COLORS['dp'], linewidth=2.2, markersize=6)
    
    # Greedy, C=10000
    n_vals, times = [], []
    for row in scale_data:
        if int(row['capacity']) == 10000 and row['algorithm'] == 'greedy':
            if row['status'] == 'completed' and row['elapsed_ms'] and row['elapsed_ms'].strip():
                n_vals.append(int(row['n']))
                times.append(float(row['elapsed_ms']))
    if n_vals:
        sorted_pairs = sorted(zip(n_vals, times))
        n_sorted, t_sorted = zip(*sorted_pairs)
        ax.plot(n_sorted, t_sorted, marker=MARKERS['greedy'], linestyle=LINESTYLES['c2'],
                label='Greedy, C=10,000', color=COLORS['greedy'], linewidth=2.2, markersize=6)
    
    # DP, C=100000
    n_vals, times = [], []
    for row in scale_data:
        if int(row['capacity']) == 100000 and row['algorithm'] == 'dynamic_programming_optimized':
            if row['status'] == 'completed' and row['elapsed_ms'] and row['elapsed_ms'].strip():
                n_vals.append(int(row['n']))
                times.append(float(row['elapsed_ms']))
    if n_vals:
        sorted_pairs = sorted(zip(n_vals, times))
        n_sorted, t_sorted = zip(*sorted_pairs)
        ax.plot(n_sorted, t_sorted, marker=MARKERS['dp'], linestyle=LINESTYLES['c2'],
                label='DP, C=100,000', color=COLORS['dp_secondary'], linewidth=2.2, markersize=6)
    
    # Greedy, C=100000
    n_vals, times = [], []
    for row in scale_data:
        if int(row['capacity']) == 100000 and row['algorithm'] == 'greedy':
            if row['status'] == 'completed' and row['elapsed_ms'] and row['elapsed_ms'].strip():
                n_vals.append(int(row['n']))
                times.append(float(row['elapsed_ms']))
    if n_vals:
        sorted_pairs = sorted(zip(n_vals, times))
        n_sorted, t_sorted = zip(*sorted_pairs)
        ax.plot(n_sorted, t_sorted, marker=MARKERS['greedy'], linestyle=LINESTYLES['c1'],
                label='Greedy, C=100,000', color=COLORS['greedy_secondary'], linewidth=2.2, markersize=6)
    
    # Greedy, C=1000000
    n_vals, times = [], []
    for row in scale_data:
        if int(row['capacity']) == 1000000 and row['algorithm'] == 'greedy':
            if row['status'] == 'completed' and row['elapsed_ms'] and row['elapsed_ms'].strip():
                n_vals.append(int(row['n']))
                times.append(float(row['elapsed_ms']))
    if n_vals:
        sorted_pairs = sorted(zip(n_vals, times))
        n_sorted, t_sorted = zip(*sorted_pairs)
        ax.plot(n_sorted, t_sorted, marker=MARKERS['greedy'], linestyle=LINESTYLES['c3'],
                label='Greedy, C=1,000,000', color=COLORS['greedy_tertiary'], linewidth=2.2, markersize=6)
    
    apply_academic_style(ax, '0-1 Knapsack: Elapsed Time Comparison',
                        'Number of Items n', 'Elapsed Time (ms)',
                        xlog=True, ylog=True)
    configure_legend(ax, loc='upper left')
    plt.tight_layout()
    plt.savefig('figures/knapsack_elapsed_time_comparison.png', dpi=300, bbox_inches='tight')
    plt.close()
    print("Generated: figures/knapsack_elapsed_time_comparison.png")
    
    # ==================== 2. DP 单独趋势图 ====================
    fig, ax = plt.subplots(figsize=(10, 6))
    
    # DP, C=10000
    n_vals, times = [], []
    for row in scale_data:
        if int(row['capacity']) == 10000 and row['algorithm'] == 'dynamic_programming_optimized':
            if row['status'] == 'completed' and row['elapsed_ms'] and row['elapsed_ms'].strip():
                n_vals.append(int(row['n']))
                times.append(float(row['elapsed_ms']))
    if n_vals:
        sorted_pairs = sorted(zip(n_vals, times))
        n_sorted, t_sorted = zip(*sorted_pairs)
        ax.plot(n_sorted, t_sorted, marker=MARKERS['dp'], linestyle=LINESTYLES['c1'],
                label='C=10,000', color=COLORS['dp'], linewidth=2.2, markersize=6)
    
    # DP, C=100000
    n_vals, times = [], []
    for row in scale_data:
        if int(row['capacity']) == 100000 and row['algorithm'] == 'dynamic_programming_optimized':
            if row['status'] == 'completed' and row['elapsed_ms'] and row['elapsed_ms'].strip():
                n_vals.append(int(row['n']))
                times.append(float(row['elapsed_ms']))
    if n_vals:
        sorted_pairs = sorted(zip(n_vals, times))
        n_sorted, t_sorted = zip(*sorted_pairs)
        ax.plot(n_sorted, t_sorted, marker=MARKERS['dp'], linestyle=LINESTYLES['c2'],
                label='C=100,000', color=COLORS['dp_secondary'], linewidth=2.2, markersize=6)
    
    apply_academic_style(ax, 'Optimized Dynamic Programming: Elapsed Time Trend',
                        'Number of Items n', 'Elapsed Time (ms)',
                        xlog=True, ylog=True)
    configure_legend(ax, loc='upper left')
    plt.tight_layout()
    plt.savefig('figures/knapsack_dp_elapsed_time.png', dpi=300, bbox_inches='tight')
    plt.close()
    print("Generated: figures/knapsack_dp_elapsed_time.png")
    
    # ==================== 3. 空间估算图 ====================
    fig, ax = plt.subplots(figsize=(10, 6))
    
    # DP, C=10000
    n_vals, mems = [], []
    for row in scale_data:
        if int(row['capacity']) == 10000 and row['algorithm'] == 'dynamic_programming_optimized':
            if row['status'] == 'completed' and row['estimated_memory_bytes'] and row['estimated_memory_bytes'].strip():
                n_vals.append(int(row['n']))
                mems.append(int(row['estimated_memory_bytes']) / (1024 * 1024))
    if n_vals:
        sorted_pairs = sorted(zip(n_vals, mems))
        n_sorted, m_sorted = zip(*sorted_pairs)
        ax.plot(n_sorted, m_sorted, marker=MARKERS['dp'], linestyle=LINESTYLES['c1'],
                label='DP, C=10,000', color=COLORS['dp'], linewidth=2.2, markersize=6)
    
    # DP, C=100000
    n_vals, mems = [], []
    for row in scale_data:
        if int(row['capacity']) == 100000 and row['algorithm'] == 'dynamic_programming_optimized':
            if row['status'] == 'completed' and row['estimated_memory_bytes'] and row['estimated_memory_bytes'].strip():
                n_vals.append(int(row['n']))
                mems.append(int(row['estimated_memory_bytes']) / (1024 * 1024))
    if n_vals:
        sorted_pairs = sorted(zip(n_vals, mems))
        n_sorted, m_sorted = zip(*sorted_pairs)
        ax.plot(n_sorted, m_sorted, marker=MARKERS['dp'], linestyle=LINESTYLES['c2'],
                label='DP, C=100,000', color=COLORS['dp_secondary'], linewidth=2.2, markersize=6)
    
    # Greedy (合并所有容量，只画一条线)
    n_vals, mems = [], []
    for row in scale_data:
        if row['algorithm'] == 'greedy':
            if row['status'] == 'completed' and row['estimated_memory_bytes'] and row['estimated_memory_bytes'].strip():
                n_vals.append(int(row['n']))
                mems.append(int(row['estimated_memory_bytes']) / (1024 * 1024))
    if n_vals:
        sorted_pairs = sorted(zip(n_vals, mems))
        n_sorted, m_sorted = zip(*sorted_pairs)
        ax.plot(n_sorted, m_sorted, marker=MARKERS['greedy'], linestyle=LINESTYLES['c2'],
                label='Greedy', color=COLORS['greedy'], linewidth=2.2, markersize=6)
    
    apply_academic_style(ax, '0-1 Knapsack: Estimated Core Memory Usage',
                        'Number of Items n', 'Estimated Core Memory (MB)',
                        xlog=False, ylog=True)
    configure_legend(ax, loc='upper left')
    
    # 添加说明文字
    ax.text(0.5, -0.12, 'Estimated core data-structure memory; not OS peak working set.',
            transform=ax.transAxes, ha='center', fontsize=10, color='#666666')
    
    plt.tight_layout()
    plt.savefig('figures/knapsack_memory_usage.png', dpi=300, bbox_inches='tight')
    plt.close()
    print("Generated: figures/knapsack_memory_usage.png")
    
    # ==================== 4. 解质量图 ====================
    fig, ax = plt.subplots(figsize=(10, 6))
    
    # C=10000
    n_vals, gaps = [], []
    for row in quality_data:
        if int(row['capacity']) == 10000:
            gap_val = row['greedy_gap_percent']
            if gap_val and gap_val.strip():
                n_vals.append(int(row['n']))
                gaps.append(float(gap_val))
    if n_vals:
        sorted_pairs = sorted(zip(n_vals, gaps))
        n_sorted, g_sorted = zip(*sorted_pairs)
        ax.plot(n_sorted, g_sorted, marker='o', linestyle=LINESTYLES['c1'],
                label='C=10,000', color=COLORS['greedy'], linewidth=2.2, markersize=6)
    
    # C=100000
    n_vals, gaps = [], []
    for row in quality_data:
        if int(row['capacity']) == 100000:
            gap_val = row['greedy_gap_percent']
            if gap_val and gap_val.strip():
                n_vals.append(int(row['n']))
                gaps.append(float(gap_val))
    if n_vals:
        sorted_pairs = sorted(zip(n_vals, gaps))
        n_sorted, g_sorted = zip(*sorted_pairs)
        ax.plot(n_sorted, g_sorted, marker='s', linestyle=LINESTYLES['c2'],
                label='C=100,000', color=COLORS['greedy_tertiary'], linewidth=2.2, markersize=6)
    
    apply_academic_style(ax, 'Greedy Strategy: Optimality Gap',
                        'Number of Items n', 'Greedy Optimality Gap (%)',
                        xlog=False, ylog=False)
    configure_legend(ax, loc='upper right')
    plt.tight_layout()
    plt.savefig('figures/knapsack_solution_quality.png', dpi=300, bbox_inches='tight')
    plt.close()
    print("Generated: figures/knapsack_solution_quality.png")
    
    print("\nAll knapsack charts generated successfully!")

if __name__ == '__main__':
    main()
