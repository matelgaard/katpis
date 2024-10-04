import os
import sys
import subprocess

time_limit = 5

main_path = 'build/main.exe'

problem = sys.argv[1]
test_folder = 'problems/' + problem + '/tests/'

class fcolors:
    BLACK = '\033[90m'
    RED = '\033[91m'
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    BLUE = '\033[94m'
    MAGENTA = '\033[95m'
    CYAN = '\033[96m'
    WHITE = '\033[97m'
    ENDC = '\033[0m'

if not os.path.exists(test_folder):
    print(problem + ': no tests to run')
    sys.exit(0)

test_files = os.listdir(test_folder)

if len(test_files) == 0:
    print(problem + ': no tests to run')

tests = []

for file in test_files:
    if not file.endswith('.in'):
        continue

    test_name = file.replace('.in', '')
    test_path = os.path.join(test_folder, file)

    answer_str = None
    answer_path = test_path.replace('.in', '.ans')
    if os.path.isfile(answer_path):
        answer = open(answer_path, 'r', encoding='ascii')
        answer_str = answer.read()
        answer_str = answer_str.removesuffix('\n')

    test = open(test_path, 'r', encoding='ascii')

    test_str = test.read()
    tests.append((test_name, test_str, answer_str))

for i, test in enumerate(tests):
    name = test[0]
    input = test[1]
    answer = test[2]

    print(name)
    print(fcolors.CYAN + test[1] + fcolors.ENDC, end='')

    try:
        result = subprocess.run(args=main_path, input=input, encoding='ascii', capture_output=True, timeout=time_limit)
        if result.returncode != 0:
            print(fcolors.RED + 'ERROR' + fcolors.ENDC)
            continue
        out = result.stdout.removesuffix('\n')
        if (test[2]):
            if (out == answer):
                print(fcolors.GREEN + out + fcolors.ENDC)
            else:
                print(fcolors.RED + out + fcolors.ENDC)
        else:
            print(out)
    except subprocess.TimeoutExpired:
        print(fcolors.RED + 'TIMED OUT' + fcolors.ENDC)
    except subprocess.SubprocessError:
        print(fcolors.RED + 'ERROR' + fcolors.ENDC)
    
    print('')

