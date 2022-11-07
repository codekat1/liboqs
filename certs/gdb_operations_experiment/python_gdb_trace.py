import os
import sys
import time

# These variables are set within gdb using -ex 'py fileName="FILE NAME"' and -ex 'py logTerminatesAtLine=30'
assert len(fileName) > 0
assert logTerminatesAtLine > 0
print(f'File "{fileName} terminates at line {logTerminatesAtLine}. Algorithm = {algorithm}, Operation = {operation}')

class TraceAsm(gdb.Command):
	def __init__(self):
		super().__init__(
			'trace-asm',
			gdb.COMMAND_BREAKPOINTS,
			gdb.COMPLETE_NONE,
			False
		)
	def invoke(self, argument, from_tty):
		argv = gdb.string_to_argv(argument)
		if argv:
			gdb.write('Does not take any arguments.\n')
		else:
			occurances = {}
			count = 0
			#done = False
			thread = gdb.inferiors()[0].threads()[0]

			#file = open('experiment_output.csv', 'a')
			
			while thread.is_valid():
				#time.sleep(1)
				try:
					frame = gdb.selected_frame()
					sal = frame.find_sal()
					symtab = sal.symtab
					if symtab:
						path = symtab.fullname()
						line = sal.line
					else:
						path = None
						line = None
						#break
					pc = frame.pc()
					op = frame.architecture().disassemble(pc)[0]['asm'].split()[0]

					#if path is not None:
					#	file.write(str(line) + ' - ' + os.path.basename(path) + '\n')
					#	file.write("{} {} {}".format(hex(pc), frame.architecture().disassemble(pc)[0]['asm'], os.linesep))
					
					if op not in occurances:
						occurances[op] = 1
					else:
						occurances[op] += 1

					if path is not None and os.path.basename(path) == fileName and line == logTerminatesAtLine:
						break

					gdb.execute('si', to_string=True)
					#gdb.execute('si', from_tty=True, to_string=False)
					count += 1
				except Exception as e:
					exc_type, exc_obj, exc_tb = sys.exc_info()
					fname = os.path.split(exc_tb.tb_frame.f_code.co_filename)[1]
					print('ERROR', exc_type, 'file', fname, 'line', exc_tb.tb_lineno)
					break

			occurances = dict(sorted(occurances.items(), key=lambda item: item[1], reverse=True))

			file = open('experiment_output.csv', 'a')
			line = ''
			line += str(algorithm) + ','
			line += str(operation) + ','
			line += '"' + str(occurances) + '",'
			file.write(line + '\n')
			file.close()
			
			print('Success!')
TraceAsm()