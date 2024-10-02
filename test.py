import os
import sys
import subprocess

time_limit = 5

main_path = 'build/main.exe'

problem = sys.argv[1]
test_file_path = 'problems/' + problem + '/tests.txt'

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

if not os.path.isfile(test_file_path):
    print(problem + ' tests missing')
    sys.exit(0)

f = open(test_file_path, 'r', encoding='ascii')

tests_str = f.read()
tests = tests_str.split('\n\n')
for i, test in enumerate(tests):
    print('test ' + str(i + 1))
    print(fcolors.CYAN + test + fcolors.ENDC)

    try:
        result = subprocess.run(args=main_path, input=test, encoding='ascii', capture_output=True, timeout=time_limit)
        if result.returncode != 0:
            print(fcolors.RED + 'ERROR' + fcolors.ENDC)
            continue
        print(result.stdout)
    except subprocess.TimeoutExpired:
        print(fcolors.RED + 'TIMED OUT' + fcolors.ENDC)
    except subprocess.SubprocessError:
        print(fcolors.RED + 'ERROR' + fcolors.ENDC)

