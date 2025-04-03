import os
import subprocess
import sys

def read_file(file_path):
    if os.path.exists(file_path):
        with open(file_path, "r") as f:
            return f.read()
    return ""

def run_single_test(test_file):
    cpp_executable = os.path.join(os.path.dirname(__file__), "../../../build/paraCL")
    
    if not os.path.isfile(cpp_executable) or not os.access(cpp_executable, os.X_OK):
        print(f"File '{cpp_executable}' not found or not executable")
        sys.exit(1)
    
    current_directory = os.getcwd()
    data_folder = os.path.join(current_directory, "data")
    input_folder = os.path.join(current_directory, "input")
    
    test_path = os.path.join(data_folder, test_file)
    test_number = test_file.split('.')[0] 
    
    input_file = f"{test_number}_input.dat"
    input_path = os.path.join(input_folder, input_file)
    
    # Читаем входные данные (если файла нет, будет пустая строка)
    input_data = read_file(input_path)

    # Передаём тестовый файл как аргумент командной строки
    args = [cpp_executable, test_path]
    try:
        result = subprocess.run(
            args,
            input=input_data,  # Передаём входные данные через stdin
            text=True,
            capture_output=True,  # Захватываем stdout и stderr
            check=False  # Разрешаем ошибки выполнения
        )
        program_stdout = result.stdout.strip()  # Убираем лишние пробелы и переносы строк
        program_stderr = result.stderr.strip()  # Убираем лишние пробелы и переносы строк
    except Exception as e:
        print(f"Error while testing test: {test_number}: {e}")
        sys.exit(1)
    
    # Логика проверки
    if program_stdout != "" or program_stderr != "":
        print(f"Test {test_number}: passed (output detected)")
        sys.exit(0)
    else:
        print(f"Test {test_number}: failed (no output detected)")
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python3 run_tests.py <test_file>")
        sys.exit(1)
    
    test_file = sys.argv[1]
    run_single_test(test_file)