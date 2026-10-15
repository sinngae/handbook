import re
import time

code_pattern = re.compile(r'TN\d{18}')
err_pattern = re.compile(r'err|error', re.IGNORECASE)

def tail_log(log_file):
    """模拟tail -f，持续读取新增日志行"""
    with open(log_file, "r", encoding="utf-8", errors="ignore") as f:
        f.seek(0, 2)
        while True:
            line = f.readline()
            if not line:
                time.sleep(0.2)
                continue
            if err_pattern.search(line):
                codes = code_pattern.findall(line)
                for c in codes:
                    print(f"【实时捕获】{c} | {line.strip()}")

if __name__ == "__main__":
    tail_log("app.log")