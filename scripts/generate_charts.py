import csv
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import os

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
    'bubble': '-',
    'merge': '--',
    'quick': '-.'
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

# ==================== 主函数 ====================
def main():
    # 确保目录存在
    os.makedirs("figures", exist_ok=True)
    
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
    
    # ==================== 1. 排序比较次数图 ====================
    fig, ax = plt.subplots(figsize=(10, 6))
    
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
    
    # 只绘制真实实测数据，移除理论参考线
    ax.plot(valid_scales_cmp, valid_bubble_cmp, marker=MARKERS['bubble'], 
            linestyle=LINESTYLES['bubble'], label='Bubble Sort',
            color=COLORS['bubble'], linewidth=2.2, markersize=6)
    ax.plot(scales, merge_comparisons, marker=MARKERS['merge'], 
            linestyle=LINESTYLES['merge'], label='Merge Sort',
            color=COLORS['merge'], linewidth=2.2, markersize=6)
    ax.plot(scales, quick_comparisons, marker=MARKERS['quick'], 
            linestyle=LINESTYLES['quick'], label='Quick Sort',
            color=COLORS['quick'], linewidth=2.2, markersize=6)
    
    apply_academic_style(ax, 'Sorting Algorithms: Number of Comparisons',
                        'Input Size (n)', 'Number of Comparisons',
                        xlog=True, ylog=True)
    configure_legend(ax, loc='upper left')
    plt.tight_layout()
    plt.savefig('figures/sorting_comparisons.png', dpi=300, bbox_inches='tight')
    plt.close()
    print("Generated: figures/sorting_comparisons.png")
    
    # ==================== 2. 排序执行时间图 ====================
    fig, ax = plt.subplots(figsize=(10, 6))
    
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
    
    ax.plot(valid_scales_time, valid_bubble_time, marker=MARKERS['bubble'], 
            linestyle=LINESTYLES['bubble'], label='Bubble Sort',
            color=COLORS['bubble'], linewidth=2.2, markersize=6)
    ax.plot(scales, merge_time, marker=MARKERS['merge'], 
            linestyle=LINESTYLES['merge'], label='Merge Sort',
            color=COLORS['merge'], linewidth=2.2, markersize=6)
    ax.plot(scales, quick_time, marker=MARKERS['quick'], 
            linestyle=LINESTYLES['quick'], label='Quick Sort',
            color=COLORS['quick'], linewidth=2.2, markersize=6)
    
    apply_academic_style(ax, 'Sorting Algorithms: Elapsed Time',
                        'Input Size (n)', 'Elapsed Time (ms)',
                        xlog=True, ylog=True)
    configure_legend(ax, loc='upper left')
    plt.tight_layout()
    plt.savefig('figures/sorting_elapsed_time.png', dpi=300, bbox_inches='tight')
    plt.close()
    print("Generated: figures/sorting_elapsed_time.png")
    
    print("\nAll sorting charts generated successfully!")

if __name__ == '__main__':
    main()
