import re
import os

# 正则表达式
# TN开头 + 18个数字，总长度20；同时匹配行中包含 error / err（忽略大小写）
code_pattern = re.compile(r'TN\d{18}')
err_pattern = re.compile(r'err|error', re.IGNORECASE)

def extract_tn_code(line: str):
    """从单行日志提取符合条件TN编码，没有返回空列表"""
    # 判断该行是否包含 err / error
    if not err_pattern.search(line):
        return []
    # 查找所有TN编码
    codes = code_pattern.findall(line)
    return codes

def scan_log_files(file_list, output_file="tn_error_codes.txt"):
    """
    按顺序扫描日志文件
    :param file_list: 有序日志文件路径列表
    :param output_file: 结果输出文件
    """
    result_set = set()  # 使用集合自动去重

    for log_path in file_list:
        if not os.path.exists(log_path):
            print(f"警告：文件不存在 {log_path}")
            continue
        print(f"正在处理日志：{log_path}")
        try:
            with open(log_path, "r", encoding="utf-8", errors="ignore") as f:
                for line_num, line in enumerate(f, start=1):
                    tn_codes = extract_tn_code(line)
                    for code in tn_codes:
                        result_set.add(code)
                        print(f"找到编码: {code}  行号:{line_num} 文件:{log_path}")
        except Exception as e:
            print(f"读取文件 {log_path} 异常: {str(e)}")

    # 写入结果
    with open(output_file, "w", encoding="utf-8") as out_f:
        for code in sorted(result_set):
            out_f.write(code + "\n")
    print(f"\n扫描完成！一共找到 {len(result_set)} 个唯一编码，结果保存至 {output_file}")

if __name__ == "__main__":
    # ========== 在这里填写你的有序日志文件列表，按日志时间先后排序 ==========
    log_files = [
        "log_01.txt",
        "log_02.txt",
        "log_03.txt",
        # 继续追加更多日志文件
    ]
    scan_log_files(log_files)