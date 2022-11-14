import os
import sys
import time

# If it doesn't find the library, type this into the terminal:
"""
LD_LIBRARY_PATH=/usr/local/lib
export LD_LIBRARY_PATH
"""

def terminal(cmd):
	os.system(cmd)

print('Compiling files...\n')
terminal('./compile.sh')

file = open('experiment_output.csv', 'w')
header = 'Algorithm,'
header += 'Experiment,'
header += 'Line Start,'
header += 'Line End,'
header += 'Address Start,'
header += 'Address End,'
header += 'Opcode Histogram,'
file.write(header + '\n')
file.close()

algorithms = ['Dilithium2', 'Dilithium3',  'Dilithium5', 'Falcon-512', 'Falcon-1024', 'SPHINCS+-SHA256-128s-robust', 'SPHINCS+-SHA256-192s-robust', 'SPHINCS+-SHA256-256s-robust']
algorithms_pretty = ['Dilithium 2', 'Dilithium 3',  'Dilithium 5', 'Falcon 512', 'Falcon 1024', 'SPHINCS+ SHA256-128s-robust', 'SPHINCS+ SHA256-192s-robust', 'SPHINCS+ SHA256-256s-robust']

operations = ['Verifying', 'Signing', 'Key Generation']
operations_files = ['gdb_verify.cpp', 'gdb_sign.cpp', 'gdb_keygen.cpp']
#operations_files = ['gdb_baseline.cpp', 'gdb_baseline.cpp', 'gdb_baseline.cpp']

print('Running baseline measurement...')
cmd = 'gdb -nx'
#cmd += ' -batch'
cmd += ' -batch-silent'
cmd += ' -ex "py algorithm=\'BASELINE MEASUREMENT (NO CODE)\'"'
cmd += ' -ex "py operation=\'N/A\'"'
cmd += ' -ex "py fileName=\'gdb_baseline.cpp\'"'
cmd += ' -ex "set breakpoint pending on"'
cmd += ' -ex "source python_gdb_trace.py"'
cmd += ' -ex "break log"'
cmd += ' -ex "run"'
cmd += ' -ex "break-return"'
cmd += ' -ex "run"' # Resets pc, since we called "up" in break-return
cmd += ' -ex "trace-asm"'
cmd += ' --args gdb_baseline.o "BASELINE"'
time_start = time.time()
terminal(cmd)
time_end = time.time()
print(f'    Success! That took {time_end - time_start} seconds.')

for i, algorithm in enumerate(algorithms):
	for j, operation in enumerate(operations):
		print(f'Running {algorithms_pretty[i]} ({operation})...')
		
		codeFile = operations_files[j]
		binaryFile = codeFile[:-4] + '.o'
		cmd = 'gdb -nx'
		# cmd += ' -batch'		# Printing enabled
		cmd += ' -batch-silent'	# Printing disabled
		cmd += f' -ex "py algorithm=\'{algorithms_pretty[i]}\'"'
		cmd += f' -ex "py operation=\'{operation}\'"'
		cmd += f' -ex "py fileName=\'{codeFile}\'"'
		cmd += ' -ex "set breakpoint pending on"'
		cmd += ' -ex "source python_gdb_trace.py"'
		cmd += ' -ex "break log"'
		cmd += ' -ex "run"'
		cmd += ' -ex "break-return"'
		cmd += ' -ex "run"' # Resets pc, since we called "up" in break-return
		cmd += ' -ex "trace-asm"'
		cmd += f' --args {binaryFile} "{algorithm}"'

		time_start = time.time()
		terminal(cmd)
		time_end = time.time()
		print(f'    Success! That took {time_end - time_start} seconds.')

# gdb -nx -batch-silent -ex "py algorithm='Dilithium 2'" -ex "py operation='Verifying'" -ex "py fileName='gdb_verify.cpp'" -ex "set breakpoint pending on" -ex "source python_gdb_trace.py" -ex "break log" -ex "run" -ex "next" -ex "break-return" -ex "run" -ex "trace-asm" --args gdb_verify.o "Dilithium2"

# gdb -nx -batch-silent -ex "py algorithm='Dilithium 2'" -ex "py operation='Signing'" -ex "py fileName='gdb_sign.cpp'" -ex "set breakpoint pending on" -ex "source python_gdb_trace.py" -ex "break log" -ex "run" -ex "next" -ex "break-return" -ex "run" -ex "trace-asm" --args gdb_sign.o "Dilithium2"

# gdb -nx -batch-silent -ex "py algorithm='Dilithium 2'" -ex "py operation='Key Generation'" -ex "py fileName='gdb_keygen.cpp'" -ex "set breakpoint pending on" -ex "source python_gdb_trace.py" -ex "break log" -ex "run" -ex "next" -ex "break-return" -ex "run" -ex "trace-asm" --args gdb_keygen.o "Dilithium2"

# gdb -nx -batch-silent -ex "py algorithm='Dilithium 3'" -ex "py operation='Verifying'" -ex "py fileName='gdb_verify.cpp'" -ex "set breakpoint pending on" -ex "source python_gdb_trace.py" -ex "break log" -ex "run" -ex "next" -ex "break-return" -ex "run" -ex "trace-asm" --args gdb_verify.o "Dilithium3"

# gdb -nx -batch-silent -ex "py algorithm='Dilithium 3'" -ex "py operation='Signing'" -ex "py fileName='gdb_sign.cpp'" -ex "set breakpoint pending on" -ex "source python_gdb_trace.py" -ex "break log" -ex "run" -ex "next" -ex "break-return" -ex "run" -ex "trace-asm" --args gdb_sign.o "Dilithium3"

# gdb -nx -batch-silent -ex "py algorithm='Dilithium 3'" -ex "py operation='Key Generation'" -ex "py fileName='gdb_keygen.cpp'" -ex "set breakpoint pending on" -ex "source python_gdb_trace.py" -ex "break log" -ex "run" -ex "next" -ex "break-return" -ex "run" -ex "trace-asm" --args gdb_keygen.o "Dilithium3"

# gdb -nx -batch-silent -ex "py algorithm='Dilithium 5'" -ex "py operation='Verifying'" -ex "py fileName='gdb_verify.cpp'" -ex "set breakpoint pending on" -ex "source python_gdb_trace.py" -ex "break log" -ex "run" -ex "next" -ex "break-return" -ex "run" -ex "trace-asm" --args gdb_verify.o "Dilithium5"

# gdb -nx -batch-silent -ex "py algorithm='Dilithium 5'" -ex "py operation='Signing'" -ex "py fileName='gdb_sign.cpp'" -ex "set breakpoint pending on" -ex "source python_gdb_trace.py" -ex "break log" -ex "run" -ex "next" -ex "break-return" -ex "run" -ex "trace-asm" --args gdb_sign.o "Dilithium5"

# gdb -nx -batch-silent -ex "py algorithm='Dilithium 5'" -ex "py operation='Key Generation'" -ex "py fileName='gdb_keygen.cpp'" -ex "set breakpoint pending on" -ex "source python_gdb_trace.py" -ex "break log" -ex "run" -ex "next" -ex "break-return" -ex "run" -ex "trace-asm" --args gdb_keygen.o "Dilithium5"

# gdb -nx -batch-silent -ex "py algorithm='Falcon 512'" -ex "py operation='Verifying'" -ex "py fileName='gdb_verify.cpp'" -ex "set breakpoint pending on" -ex "source python_gdb_trace.py" -ex "break log" -ex "run" -ex "next" -ex "break-return" -ex "run" -ex "trace-asm" --args gdb_verify.o "Falcon-512"

# gdb -nx -batch-silent -ex "py algorithm='Falcon 512'" -ex "py operation='Signing'" -ex "py fileName='gdb_sign.cpp'" -ex "set breakpoint pending on" -ex "source python_gdb_trace.py" -ex "break log" -ex "run" -ex "next" -ex "break-return" -ex "run" -ex "trace-asm" --args gdb_sign.o "Falcon-512"

# gdb -nx -batch-silent -ex "py algorithm='Falcon 512'" -ex "py operation='Key Generation'" -ex "py fileName='gdb_keygen.cpp'" -ex "set breakpoint pending on" -ex "source python_gdb_trace.py" -ex "break log" -ex "run" -ex "next" -ex "break-return" -ex "run" -ex "trace-asm" --args gdb_keygen.o "Falcon-512"

# gdb -nx -batch-silent -ex "py algorithm='Falcon 1024'" -ex "py operation='Verifying'" -ex "py fileName='gdb_verify.cpp'" -ex "set breakpoint pending on" -ex "source python_gdb_trace.py" -ex "break log" -ex "run" -ex "next" -ex "break-return" -ex "run" -ex "trace-asm" --args gdb_verify.o "Falcon-1024"

# gdb -nx -batch-silent -ex "py algorithm='Falcon 1024'" -ex "py operation='Signing'" -ex "py fileName='gdb_sign.cpp'" -ex "set breakpoint pending on" -ex "source python_gdb_trace.py" -ex "break log" -ex "run" -ex "next" -ex "break-return" -ex "run" -ex "trace-asm" --args gdb_sign.o "Falcon-1024"

# gdb -nx -batch-silent -ex "py algorithm='Falcon 1024'" -ex "py operation='Key Generation'" -ex "py fileName='gdb_keygen.cpp'" -ex "set breakpoint pending on" -ex "source python_gdb_trace.py" -ex "break log" -ex "run" -ex "next" -ex "break-return" -ex "run" -ex "trace-asm" --args gdb_keygen.o "Falcon-1024"

# gdb -nx -batch-silent -ex "py algorithm='SPHINCS+ SHA256-128s-robust'" -ex "py operation='Verifying'" -ex "py fileName='gdb_verify.cpp'" -ex "set breakpoint pending on" -ex "source python_gdb_trace.py" -ex "break log" -ex "run" -ex "next" -ex "break-return" -ex "run" -ex "trace-asm" --args gdb_verify.o "SPHINCS+-SHA256-128s-robust"

# gdb -nx -batch-silent -ex "py algorithm='SPHINCS+ SHA256-128s-robust'" -ex "py operation='Signing'" -ex "py fileName='gdb_sign.cpp'" -ex "set breakpoint pending on" -ex "source python_gdb_trace.py" -ex "break log" -ex "run" -ex "next" -ex "break-return" -ex "run" -ex "trace-asm" --args gdb_sign.o "SPHINCS+-SHA256-128s-robust"

# gdb -nx -batch-silent -ex "py algorithm='SPHINCS+ SHA256-128s-robust'" -ex "py operation='Key Generation'" -ex "py fileName='gdb_keygen.cpp'" -ex "set breakpoint pending on" -ex "source python_gdb_trace.py" -ex "break log" -ex "run" -ex "next" -ex "break-return" -ex "run" -ex "trace-asm" --args gdb_keygen.o "SPHINCS+-SHA256-128s-robust"

# gdb -nx -batch-silent -ex "py algorithm='SPHINCS+ SHA256-192s-robust'" -ex "py operation='Verifying'" -ex "py fileName='gdb_verify.cpp'" -ex "set breakpoint pending on" -ex "source python_gdb_trace.py" -ex "break log" -ex "run" -ex "next" -ex "break-return" -ex "run" -ex "trace-asm" --args gdb_verify.o "SPHINCS+-SHA256-192s-robust"

# gdb -nx -batch-silent -ex "py algorithm='SPHINCS+ SHA256-192s-robust'" -ex "py operation='Signing'" -ex "py fileName='gdb_sign.cpp'" -ex "set breakpoint pending on" -ex "source python_gdb_trace.py" -ex "break log" -ex "run" -ex "next" -ex "break-return" -ex "run" -ex "trace-asm" --args gdb_sign.o "SPHINCS+-SHA256-192s-robust"

# gdb -nx -batch-silent -ex "py algorithm='SPHINCS+ SHA256-192s-robust'" -ex "py operation='Key Generation'" -ex "py fileName='gdb_keygen.cpp'" -ex "set breakpoint pending on" -ex "source python_gdb_trace.py" -ex "break log" -ex "run" -ex "next" -ex "break-return" -ex "run" -ex "trace-asm" --args gdb_keygen.o "SPHINCS+-SHA256-192s-robust"

# gdb -nx -batch-silent -ex "py algorithm='SPHINCS+ SHA256-256s-robust'" -ex "py operation='Verifying'" -ex "py fileName='gdb_verify.cpp'" -ex "set breakpoint pending on" -ex "source python_gdb_trace.py" -ex "break log" -ex "run" -ex "next" -ex "break-return" -ex "run" -ex "trace-asm" --args gdb_verify.o "SPHINCS+-SHA256-256s-robust"

# gdb -nx -batch-silent -ex "py algorithm='SPHINCS+ SHA256-256s-robust'" -ex "py operation='Signing'" -ex "py fileName='gdb_sign.cpp'" -ex "set breakpoint pending on" -ex "source python_gdb_trace.py" -ex "break log" -ex "run" -ex "next" -ex "break-return" -ex "run" -ex "trace-asm" --args gdb_sign.o "SPHINCS+-SHA256-256s-robust"

# gdb -nx -batch-silent -ex "py algorithm='SPHINCS+ SHA256-256s-robust'" -ex "py operation='Key Generation'" -ex "py fileName='gdb_keygen.cpp'" -ex "set breakpoint pending on" -ex "source python_gdb_trace.py" -ex "break log" -ex "run" -ex "next" -ex "break-return" -ex "run" -ex "trace-asm" --args gdb_keygen.o "SPHINCS+-SHA256-256s-robust"
