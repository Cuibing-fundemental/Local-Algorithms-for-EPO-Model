import os
import subprocess
from multiprocessing import Pool, cpu_count
import glob

def process_file(input_file):
    """处理单个文件的函数"""
    # 获取相对路径名称（用于输出文件名）
    relative_path = os.path.relpath(input_file, './data')
    output_filename = os.path.splitext(relative_path)[0] + '.txt'
    output_file = os.path.join('./ans', output_filename)
    
    if os.path.exists(output_file):
        print(f"Skip: {output_file} already exists, skipping...")
        return f"SKIPPED: {input_file}"
    
    # 构建命令
    command = ['./bin/test/test_MyIterator', input_file]
    print(f"Processing: {input_file}")
    
    try:
        # 执行命令并将输出重定向到文件
        with open(output_file, 'w') as out_f:
            result = subprocess.run(
                command,
                stdout=out_f,
                stderr=subprocess.PIPE,
                text=True
            )
        
        # 检查命令执行是否成功
        if result.returncode != 0:
            error_msg = f"Warning: Failure in {input_file}\n{result.stderr}"
            print(error_msg)
            return f"FAILED: {input_file}"
        else:
            print(f"Successful: {input_file}!")
            return f"SUCCESS: {input_file}"
            
    except Exception as e:
        error_msg = f"Failure in {input_file}: {e}"
        print(error_msg)
        return f"ERROR: {input_file}"

def main():
    # 获取数据文件列表
    data_files = glob.glob('./data/*', recursive=True)
    data_files = [f for f in data_files if os.path.isfile(f)]
    
    print(f"Found {len(data_files)} files to process")
    print(f"Using {cpu_count()} CPU cores")
    
    # 使用多进程并行处理
    with Pool(processes=cpu_count()) as pool:
        results = pool.map(process_file, data_files)
    
    # 统计结果
    success_count = sum(1 for r in results if r.startswith('SUCCESS'))
    skipped_count = sum(1 for r in results if r.startswith('SKIPPED'))
    failed_count = sum(1 for r in results if r.startswith('FAILED'))
    error_count = sum(1 for r in results if r.startswith('ERROR'))
    
    print(f"\nAll done! Results:")
    print(f"Success: {success_count}")
    print(f"Skipped: {skipped_count}")
    print(f"Failed: {failed_count}")
    print(f"Errors: {error_count}")

if __name__ == "__main__":
    main()