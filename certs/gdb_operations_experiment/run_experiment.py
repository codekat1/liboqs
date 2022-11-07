import os
import sys
import time

def terminal(cmd):
	os.system(cmd)


#gdb -nh -batch -ex 'set breakpoint pending on' -ex 'py fileName="run.cpp"' -ex 'py logTerminatesAtLine=30' -ex 'source python_gdb_trace.py' -ex 'break log' -ex 'run' -ex 'trace-asm' --args run.o "Dilithium2"

file = open('experiment_output.csv', 'w')
header = 'Algorithm,Operation,Opcode Histogram,'
file.write(header + '\n')
file.close()

algorithms = ['Dilithium2', 'Dilithium3',  'Dilithium5', 'Falcon-512', 'Falcon-1024', 'SPHINCS+-SHA256-128s-robust', 'SPHINCS+-SHA256-192s-robust', 'SPHINCS+-SHA256-256s-robust']
algorithms_pretty = ['Dilithium 2', 'Dilithium 3',  'Dilithium 5', 'Falcon 512', 'Falcon 1024', 'SPHINCS+ SHA256-128s-robust', 'SPHINCS+ SHA256-192s-robust', 'SPHINCS+ SHA256-256s-robust']
operations = ['Key generation', 'Signing', 'Verifying']
operations_files = ['gdb_keygen.cpp', 'gdb_sign.cpp', 'gdb_verify.cpp']
#operations_files = ['gdb_temp.cpp', 'gdb_temp.cpp', 'gdb_temp.cpp']

for i, algorithm in enumerate(algorithms):
	for j, operation in enumerate(operations):
		codeFile = operations_files[j]
		binaryFile = codeFile[:-4] + '.o'
		cmd = f"gdb -nh -batch -ex 'py algorithm=\"{algorithms_pretty[i]}\"' -ex 'py operation=\"{operation}\"' -ex 'set breakpoint pending on' -ex 'py fileName=\"{codeFile}\"' -ex 'py logTerminatesAtLine=30' -ex 'source python_gdb_trace.py' -ex 'break log' -ex 'run' -ex 'trace-asm' --args {binaryFile} \"{algorithm}\""
		terminal(cmd)
		#time.sleep(10) # Give the machine a break for a bit
