import csv
import os
import re
import json
import sys

if __name__ == '__main__':
	# Construct the assembly --> category mapping using the dataset from https://uops.info/xml.html
	categoryMap = {}
	readerFile = open(os.path.join('AssemblyDatabase', 'extracted_instructions.csv'), 'r')
	reader = csv.reader(x.replace('\0', '') for x in readerFile)
	header = next(reader)
	for row in reader:
		categoryMap[row[0]] = row[1].upper()
	readerFile.close()

	outputFile = open('experiment_output_categorized.csv', 'w')
	header = 'Algorithm,'
	header += 'Experiment,'
	header += 'Total Instructions,'
	header += 'Data Movement Instructions,'
	header += 'Arithmetic and Logic Instructions,'
	header += 'Control Flow Instructions,'
	header += 'Miscellaneous Instructions,'
	header += 'Operations,'
	outputFile.write(header + '\n')

	experimentData = {'key generation': '', 'signing': '', 'verifying': ''}

	readerFile = open('experiment_output.csv', 'r')
	reader = csv.reader(x.replace('\0', '') for x in readerFile)
	header = next(reader)
	baselineRow = next(reader)
	assert baselineRow[1] == 'N/A', 'The initial baseline row was not found'
	baselineJSON = json.loads(baselineRow[2].replace("'", '"'))
	for row in reader:
		algorithm = row[0]
		experiment = row[1]
		print(row[2].replace("'", '"'))
		opcodes = json.loads(row[2].replace("'", '"'))
		# First, we remove any baseline data, since this contains the assembly instructions executed when no code is present
		for key in baselineJSON:
			if key in opcodes:
				opcodes[key] -= baselineJSON[key]

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

				# Data dataset was not complete... so any remaining opcodes can be placed here to make the dataset complete
				if category is None:
					if op in ['cmova', 'cmove', 'cmovne', 'movabs', 'movsbl', 'movslq', 'movswl', 'movw', 'movzbl', 'movzwl']: category = 'DATAXFER'
					elif op in ['andl', 'andq', 'cmpl', 'cmpq', 'cmpw', 'orl', 'testb', 'testl']: category = 'LOGICAL'
					elif op in ['ja', 'jae', 'je', 'jg', 'jmpq', 'jne']: category = 'COND_BR'
					elif op in ['addl', 'addq', 'incl', 'orq', 'subl']: category = 'BINARY'
					elif op in ['data16', 'nopl', 'nopw']: category = 'NOP'
					elif op in ['leaveq', 'notrack']: category = 'MISC'
					elif op in ['sete', 'setne']: category = 'SETCC'
					elif op in ['cltq']: category = 'CONVERT'
					elif op in ['callq']: category = 'CALL'
					elif op in ['pushq']: category = 'PUSH'
					elif op in ['shrl']: category = 'SHIFT'
					elif op in ['retq']: category = 'RET'
					else:
						print(f'No category for " {op} " ({opcodes[op]})!')
						sys.exit()

			# Optionally, we categorize the categories into four groups
			if category in ['BITBYTE', 'BROADCAST', 'CMOV', 'CONVERT', 'DATAXFER', 'POP', 'PUSH', 'SETCC', 'XSAVE']:
				category = 'Data Movement Instructions'
			elif category in ['AVX', 'AVX2', 'AVX512', 'BINARY', 'BMI1', 'LOGICAL', 'MPX', 'ROTATE', 'SHIFT', 'SSE']:
				category = 'Arithmetic and Logic Instructions'
			elif category in ['CALL', 'CET', 'COND_BR', 'NOP', 'RET', 'SEMAPHORE', 'UNCOND_BR', 'WIDENOP']:
				category = 'Control Flow Instructions'
			elif category in ['MISC', 'SYSCALL']:
				category = 'Miscellaneous Instructions'
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
		line += experiment + ','
		line += str(totalOccurances) + ','
		line += str(categorizedOutputs['Data Movement Instructions']) + ','
		line += str(categorizedOutputs['Arithmetic and Logic Instructions']) + ','
		line += str(categorizedOutputs['Control Flow Instructions']) + ','
		line += str(categorizedOutputs['Miscellaneous Instructions']) + ','
		line += '"' + str(opcodes) + '",'
		experimentData[experiment.lower()] += line + '\n'

	outputFile.write(experimentData['key generation'])
	outputFile.write(experimentData['signing'])
	outputFile.write(experimentData['verifying'])
	readerFile.close()
	outputFile.close()
	print('Done!')
