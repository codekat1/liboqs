import os
import sys
import time

# Variables are set within gdb using -ex 'py fileName="FILE NAME"'
assert len(fileName) > 0
#print(f'Algorithm = {algorithm}, Operation = {operation}')

startLine = None
endLine = None
startAddress = None
endAddress = None

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
			return

		occurances = {}
		thread = gdb.inferiors()[0].threads()[0]
		while thread.is_valid():
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
				pc = frame.pc()
				op = frame.architecture().disassemble(pc)[0]['asm'].split()[0]
				if op not in occurances:
					occurances[op] = 1
				else:
					occurances[op] += 1
				if path is not None and os.path.basename(path) == fileName and (line == endLine or pc == endAddress):
					break
				gdb.execute('stepi', to_string=True)

			except Exception as e:
				exc_type, exc_obj, exc_tb = sys.exc_info()
				fname = os.path.split(exc_tb.tb_frame.f_code.co_filename)[1]
				print('ERROR', exc_type, 'file', fname, 'line', exc_tb.tb_lineno)
				break

		occurances = dict(sorted(occurances.items(), key=lambda item: item[1], reverse=True))
		file = open('experiment_output.csv', 'a')
		line = str(algorithm) + ','
		line += str(operation) + ','
		line += str(startLine) + ','
		line += str(endLine) + ','
		line += str(startAddress) + ','
		line += str(endAddress) + ','
		line += '"' + str(occurances) + '",'
		file.write(line + '\n')
		file.close()
		# Done

# Used to quickly retrieve the return line of the function
class BreakReturn(gdb.Command):
	def __init__(self):
		super().__init__(
			'break-return',
			gdb.COMMAND_RUNNING,
			gdb.COMPLETE_NONE,
			False
		)

	def invoke(self, arg, from_tty):
		global startLine, endLine, startAddress, endAddress
		startLine = gdb.selected_frame().find_sal().line
		startAddress = gdb.selected_frame().block().start
		endLine = None

		# Go up a stack to get the program counter location
		try:
			gdb.execute('up', to_string=True)
		except:
			pass # Already at the main function

		frame = gdb.selected_frame()
		block = frame.block()
		while block:
			if block.function:
				break
			block = block.superblock
		blockStart = block.start
		blockEnd = block.end
		pc = gdb.selected_frame().pc()
		instructions = frame.architecture().disassemble(blockStart, blockEnd - 1)
		for instruction in instructions:
			if instruction['asm'].startswith('retq ') or instruction['asm'].startswith('ret '):
				endAddress = instruction['addr']
				gdb.Breakpoint('*{}'.format(endAddress))
		endLine = frame.find_sal().line
		if endAddress is None: endAddress = blockEnd

BreakReturn()
TraceAsm()