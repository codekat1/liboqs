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
header = 'Algorithm,Experiment,Opcode Histogram,'
file.write(header + '\n')
file.close()

algorithms = ['Dilithium2', 'Dilithium3',  'Dilithium5', 'Falcon-512', 'Falcon-1024', 'SPHINCS+-SHA256-128s-robust', 'SPHINCS+-SHA256-192s-robust', 'SPHINCS+-SHA256-256s-robust']
algorithms_pretty = ['Dilithium 2', 'Dilithium 3',  'Dilithium 5', 'Falcon 512', 'Falcon 1024', 'SPHINCS+ SHA256-128s-robust', 'SPHINCS+ SHA256-192s-robust', 'SPHINCS+ SHA256-256s-robust']
# algorithms = ['Falcon-512', 'Falcon-1024', 'SPHINCS+-SHA256-128s-robust', 'SPHINCS+-SHA256-192s-robust', 'SPHINCS+-SHA256-256s-robust']
# algorithms_pretty = ['Falcon 512', 'Falcon 1024', 'SPHINCS+ SHA256-128s-robust', 'SPHINCS+ SHA256-192s-robust', 'SPHINCS+ SHA256-256s-robust']
operations = ['Key Generation', 'Signing', 'Verifying']
operations_files = ['gdb_keygen.cpp', 'gdb_sign.cpp', 'gdb_verify.cpp']
#operations_files = ['gdb_baseline.cpp', 'gdb_baseline.cpp', 'gdb_baseline.cpp']

print('Running baseline measurement...')
cmd = 'gdb -nx -batch-silent'
cmd += f" -ex 'py algorithm=\"BASELINE MEASUREMENT (NO CODE)\"'"
cmd += f" -ex 'py operation=\"N/A\"'"
cmd += f" -ex 'py fileName=\"'gdb_baseline.cpp'\"'"
cmd += " -ex 'py logTerminatesAtLine=30'"
cmd += " -ex 'set breakpoint pending on'"
cmd += " -ex 'source python_gdb_trace.py'"
cmd += " -ex 'break log'"
cmd += " -ex 'run'"
cmd += " -ex 'trace-asm'"
cmd += f" --args gdb_baseline.o \"BASELINE\""
time_start = time.time()
terminal(cmd)
time_end = time.time()
print(f'Success! That took {time_end - time_start} seconds.')

for i, algorithm in enumerate(algorithms):
	for j, operation in enumerate(operations):
		print(f'Running {algorithms_pretty[i]} ({operation})...')
		
		codeFile = operations_files[j]
		binaryFile = codeFile[:-4] + '.o'
		cmd = 'gdb -nx'
		# cmd += ' -batch'		# Printing enabled
		cmd += ' -batch-silent'	# Printing disabled
		cmd += f" -ex 'py algorithm=\"{algorithms_pretty[i]}\"'"
		cmd += f" -ex 'py operation=\"{operation}\"'"
		cmd += f" -ex 'py fileName=\"{codeFile}\"'"
		cmd += " -ex 'py logTerminatesAtLine=30'"
		cmd += " -ex 'set breakpoint pending on'"
		cmd += " -ex 'source python_gdb_trace.py'"
		cmd += " -ex 'break log'"
		cmd += " -ex 'run'"
		cmd += " -ex 'trace-asm'"
		cmd += f" --args {binaryFile} \"{algorithm}\""

		time_start = time.time()
		terminal(cmd)
		time_end = time.time()
		print(f'Success! That took {time_end - time_start} seconds.')
