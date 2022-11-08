import csv
import os
import re
import json
import sys

if __name__ == '__main__':
	# Construct the assembly --> category mapping
	categoryMap = {}
	readerFile = open(os.path.join('AssemblyDatabase', 'extracted_instructions.csv'), 'r')
	reader = csv.reader(x.replace('\0', '') for x in readerFile)
	header = next(reader)
	for row in reader:
		categoryMap[row[0]] = row[1].upper()
	readerFile.close()

	outputFile = open('experiment_output_categorized.csv', 'w')
	header = ''
	header += 'Algorithm,'
	header += 'Operation,'
	header += 'Total instructions,'
	header += 'Control Flow instructions,'
	header += 'Arithmetic and Logic instructions,'
	header += 'Data Movement instructions,'
	header += 'Operations,'
	outputFile.write(header + '\n')

	readerFile = open('experiment_output.csv', 'r')
	reader = csv.reader(x.replace('\0', '') for x in readerFile)
	header = next(reader)
	for row in reader:
		algorithm = row[0]
		operation = row[1]
		print(row[2].replace("'", '"'))
		opcodes = json.loads(row[2].replace("'", '"'))

		totalOccurances = 0
		categorizedOutputs = {}
		for op in opcodes:
			totalOccurances += opcodes[op]
			category = None
			try:
				category = categoryMap[op.upper()]
			except:
				
				for opToCheck in categoryMap:
					if opToCheck.startswith(op.upper()):
						category = categoryMap[opToCheck]
						break
				if category is None:
					if op in ['movzbl', 'movswl', 'movw', 'movsbl', 'movabs', 'movzwl', 'cmove', 'movslq', 'cmova', 'cmovne']: category = 'DATAXFER'
					elif op in ['cmpl', 'cmpq', 'testb', 'cmpw', 'testl', 'andl', 'orl', 'andq']: category = 'LOGICAL'
					elif op in ['ja', 'jne', 'je', 'jae', 'jmpq', 'jg']: category = 'COND_BR'
					elif op in ['incl', 'addq', 'subl', 'orq', 'addl']: category = 'BINARY'
					elif op in ['callq']: category = 'CALL'
					elif op in ['retq']: category = 'RET'
					elif op in ['shrl']: category = 'SHIFT'
					elif op in ['pushq']: category = 'PUSH'
					elif op in ['nopw', 'data16', 'nopl']: category = 'NOP'
					elif op in ['sete', 'setne']: category = 'SETCC'
					elif op in ['cltq']: category = 'CONVERT'
					elif op in ['leaveq', 'notrack']: category = 'MISC'
					else:
						print(f'No category for " {op} " ({opcodes[op]})!')
						sys.exit()

			if category in ['COND_BR', 'UNCOND_BR', 'NOP', 'WIDENOP', 'MISC', 'CALL', 'RET', 'CET', 'SEMAPHORE']:
				category = 'Control Flow Instructions'
			elif category in ['LOGICAL', 'BINARY', 'AVX512', 'AVX2', 'AVX', 'SHIFT', 'SSE', 'MPX', 'BITBYTE', 'SYSCALL', 'ROTATE', 'BMI1', 'CONVERT']:
				category = 'Arithmetic and Logic Instructions'
			elif category in ['DATAXFER', 'PUSH', 'POP', 'BROADCAST', 'SETCC', 'XSAVE', 'CMOV']:
				category = 'Data Movement Instructions'
			else:
				print('No category for', op, f'({opcodes[op]})', '->', category)
				sys.exit()

			# After mapping the op to a category, apply that category
			if category not in categorizedOutputs:
				categorizedOutputs[category] = opcodes[op]
			else:
				categorizedOutputs[category] += opcodes[op]

		categorizedOutputs = dict(sorted(categorizedOutputs.items(), key=lambda item: item[1], reverse=True))
		line = ''
		line += algorithm + ','
		line += operation + ','
		line += str(totalOccurances) + ','
		line += str(categorizedOutputs['Control Flow Instructions']) + ','
		line += str(categorizedOutputs['Arithmetic and Logic Instructions']) + ','
		line += str(categorizedOutputs['Data Movement Instructions']) + ','
		line += '"' + str(opcodes) + '",'
		outputFile.write(line + '\n')


	readerFile.close()
	outputFile.close()

	print('Done!')




	# # Begin looping through the files
	# for file in files:
	# 	print(f'Processing {file}')
	# 	readerFile = open(file, 'r')
	# 	# Remove NUL bytes to prevent errors
	# 	reader = csv.reader(x.replace('\0', '') for x in readerFile)

	# 	i = 0
	# 	for row in reader:
	# 		j = 0
	# 		for cell in row:
	# 			if re.match(r'^-?[0-9]+(\.[0-9]*)?$', cell.strip()) is not None:
	# 				num = float(cell)
	# 				data_sum[i][j] += num
	# 				data_counter[i][j] += 1
	# 			elif data_sum[i][j] == 0:
	# 				data_sum[i][j] = cell
	# 			j += 1
	# 		i += 1

	# 	readerFile.close()

	# # Convert the sum into averages
	# for i in range(len(data_sum)):
	# 	for j in range(len(data_sum[i])):
	# 		if (type(data_sum[i][j]) is int or type(data_sum[i][j]) is float) and data_counter[i][j] != 0:
	# 			data_sum[i][j] /= data_counter[i][j]

	# # Convert the array into CSV format
	# for row in data_sum:
	# 	line = listToCSV(row)
	# 	outputFile.write(line)

	# outputFile.close()
	# print('Done.')
