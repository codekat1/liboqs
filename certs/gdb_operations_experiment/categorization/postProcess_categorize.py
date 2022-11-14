import csv
import os
import re
import json
import sys

addInstructions = ['ADC', 'ADCX', 'ADD', 'ADDPD', 'ADDPS', 'ADDSD', 'ADDSS', 'ADDSUBPD', 'ADDSUBPS', 'ADOX', 'BNDLDX', 'BNDSTX', 'FADD', 'FADDP', 'FIADD', 'HADDPD', 'HADDPS', 'KADDB', 'KADDD', 'KADDQ', 'KADDW', 'LEA', 'LOCK XADD', 'MONITOR', 'PADDB', 'PADDD', 'PADDQ', 'PADDSB', 'PADDSW', 'PADDUSB', 'PADDUSW', 'PADDW', 'PFADD', 'PHADDD', 'PHADDSW', 'PHADDW', 'PMADDUBSW', 'PMADDWD', 'TILELOADD', 'TILELOADDT1', 'UMONITOR', 'V4FMADDPS', 'V4FMADDSS', 'V4FNMADDPS', 'V4FNMADDSS', 'VADDPD', 'VADDPH', 'VADDPS', 'VADDSD', 'VADDSH', 'VADDSS', 'VADDSUBPD', 'VADDSUBPS', 'VFCMADDCPH', 'VFCMADDCSH', 'VFMADD132PD', 'VFMADD132PH', 'VFMADD132PS', 'VFMADD132SD', 'VFMADD132SH', 'VFMADD132SS', 'VFMADD213PD', 'VFMADD213PH', 'VFMADD213PS', 'VFMADD213SD', 'VFMADD213SH', 'VFMADD213SS', 'VFMADD231PD', 'VFMADD231PH', 'VFMADD231PS', 'VFMADD231SD', 'VFMADD231SH', 'VFMADD231SS', 'VFMADDCPH', 'VFMADDCSH', 'VFMADDPD', 'VFMADDPS', 'VFMADDSD', 'VFMADDSS', 'VFMADDSUB132PD', 'VFMADDSUB132PH', 'VFMADDSUB132PS', 'VFMADDSUB213PD', 'VFMADDSUB213PH', 'VFMADDSUB213PS', 'VFMADDSUB231PD', 'VFMADDSUB231PH', 'VFMADDSUB231PS', 'VFMADDSUBPD', 'VFMADDSUBPS', 'VFMSUBADD132PD', 'VFMSUBADD132PH', 'VFMSUBADD132PS', 'VFMSUBADD213PD', 'VFMSUBADD213PH', 'VFMSUBADD213PS', 'VFMSUBADD231PD', 'VFMSUBADD231PH', 'VFMSUBADD231PS', 'VFMSUBADDPD', 'VFMSUBADDPS', 'VFNMADD132PD', 'VFNMADD132PH', 'VFNMADD132PS', 'VFNMADD132SD', 'VFNMADD132SH', 'VFNMADD132SS', 'VFNMADD213PD', 'VFNMADD213PH', 'VFNMADD213PS', 'VFNMADD213SD', 'VFNMADD213SH', 'VFNMADD213SS', 'VFNMADD231PD', 'VFNMADD231PH', 'VFNMADD231PS', 'VFNMADD231SD', 'VFNMADD231SH', 'VFNMADD231SS', 'VFNMADDPD', 'VFNMADDPS', 'VFNMADDSD', 'VFNMADDSS', 'VHADDPD', 'VHADDPS', 'VPADDB', 'VPADDD', 'VPADDQ', 'VPADDSB', 'VPADDSW', 'VPADDUSB', 'VPADDUSW', 'VPADDW', 'VPHADDBD', 'VPHADDBQ', 'VPHADDBW', 'VPHADDD', 'VPHADDDQ', 'VPHADDSW', 'VPHADDUBD', 'VPHADDUBQ', 'VPHADDUBW', 'VPHADDUDQ', 'VPHADDUWD', 'VPHADDUWQ', 'VPHADDW', 'VPHADDWD', 'VPHADDWQ', 'VPMADD52HUQ', 'VPMADD52LUQ', 'VPMADDUBSW', 'VPMADDWD', 'XADD']

subtractInstructions = ['ADDSUBPD', 'ADDSUBPS', 'FISUB', 'FISUBR', 'FSUB', 'FSUBP', 'FSUBR', 'FSUBRP', 'HSUBPD', 'HSUBPS', 'PHSUBD', 'PHSUBSW', 'PHSUBW', 'PSUBB', 'PSUBD', 'PSUBQ', 'PSUBSB', 'PSUBSW', 'PSUBUSB', 'PSUBUSW', 'PSUBW', 'SBB', 'SUB', 'SUBPD', 'SUBPS', 'SUBSD', 'SUBSS', 'VADDSUBPD', 'VADDSUBPS', 'VFMADDSUB132PD', 'VFMADDSUB132PS', 'VFMADDSUB213PD', 'VFMADDSUB213PS', 'VFMADDSUB231PD', 'VFMADDSUB231PS', 'VFMSUB132PD', 'VFMSUB132PS', 'VFMSUB132SD', 'VFMSUB132SS', 'VFMSUB213PD', 'VFMSUB213PS', 'VFMSUB213SD', 'VFMSUB213SS', 'VFMSUB231PD', 'VFMSUB231PS', 'VFMSUB231SD', 'VFMSUB231SS', 'VFMSUBADD132PD', 'VFMSUBADD132PS', 'VFMSUBADD213PD', 'VFMSUBADD213PS', 'VFMSUBADD231PD', 'VFMSUBADD231PS', 'VFNMSUB132PD', 'VFNMSUB132PS', 'VFNMSUB132SD', 'VFNMSUB132SS', 'VFNMSUB213PD', 'VFNMSUB213PS', 'VFNMSUB213SD', 'VFNMSUB213SS', 'VFNMSUB231PD', 'VFNMSUB231PS', 'VFNMSUB231SD', 'VFNMSUB231SS', 'VHSUBPD', 'VHSUBPS', 'VPHSUBD', 'VPHSUBSW', 'VPHSUBW', 'VPSUBB', 'VPSUBD', 'VPSUBQ', 'VPSUBSB', 'VPSUBSW', 'VPSUBUSB', 'VPSUBUSW', 'VPSUBW', 'VSUBPD', 'VSUBPS', 'VSUBSD', 'VSUBSS']

multiplyInstructions = ['FIMUL', 'FMUL', 'FMULP', 'GF2P8MULB', 'IMUL', 'MUL', 'MULPD', 'MULPS', 'MULSD', 'MULSS', 'MULX', 'PMADDUBSW', 'PMADDWD', 'PMULDQ', 'PMULHRSW', 'PMULHUW', 'PMULHW', 'PMULLD', 'PMULLW', 'PMULUDQ', 'V4FMADDPS', 'V4FMADDSS', 'V4FNMADDPS', 'V4FNMADDSS', 'VFMADD132PD', 'VFMADD132PS', 'VFMADD132SD', 'VFMADD132SS', 'VFMADD213PD', 'VFMADD213PS', 'VFMADD213SD', 'VFMADD213SS', 'VFMADD231PD', 'VFMADD231PS', 'VFMADD231SD', 'VFMADD231SS', 'VFMADDSUB132PD', 'VFMADDSUB132PS', 'VFMADDSUB213PD', 'VFMADDSUB213PS', 'VFMADDSUB231PD', 'VFMADDSUB231PS', 'VFMSUB132PD', 'VFMSUB132PS', 'VFMSUB132SD', 'VFMSUB132SS', 'VFMSUB213PD', 'VFMSUB213PS', 'VFMSUB213SD', 'VFMSUB213SS', 'VFMSUB231PD', 'VFMSUB231PS', 'VFMSUB231SD', 'VFMSUB231SS', 'VFMSUBADD132PD', 'VFMSUBADD132PS', 'VFMSUBADD213PD', 'VFMSUBADD213PS', 'VFMSUBADD231PD', 'VFMSUBADD231PS', 'VFNMADD132PD', 'VFNMADD132PS', 'VFNMADD132SD', 'VFNMADD132SS', 'VFNMADD213PD', 'VFNMADD213PS', 'VFNMADD213SD', 'VFNMADD213SS', 'VFNMADD231PD', 'VFNMADD231PS', 'VFNMADD231SD', 'VFNMADD231SS', 'VFNMSUB132PD', 'VFNMSUB132PS', 'VFNMSUB132SD', 'VFNMSUB132SS', 'VFNMSUB213PD', 'VFNMSUB213PS', 'VFNMSUB213SD', 'VFNMSUB213SS', 'VFNMSUB231PD', 'VFNMSUB231PS', 'VFNMSUB231SD', 'VFNMSUB231SS', 'VGF2P8MULB', 'VMULPD', 'VMULPS', 'VMULSD', 'VMULSS', 'VPMADD52HUQ', 'VPMADD52LUQ', 'VPMADDUBSW', 'VPMADDWD', 'VPMULDQ', 'VPMULHRSW', 'VPMULHUW', 'VPMULHW', 'VPMULLD', 'VPMULLQ', 'VPMULLW', 'VPMULUDQ']

divideInstructions = ['DIV', 'DIVPD', 'DIVPS', 'DIVSD', 'DIVSS', 'FDIV', 'FDIVP', 'FDIVR', 'FDIVRP', 'FIDIV', 'FIDIVR', 'IDIV', 'VDIVPD', 'VDIVPS', 'VDIVSD', 'VDIVSS']

jumpInstructions = ['JB', 'JBE', 'JL', 'JLE', 'JMP', 'JNB', 'JNBE', 'JNL', 'JNLE', 'JNO', 'JNP', 'JNS', 'JNZ', 'JO', 'JP', 'JRCXZ', 'JS', 'JZ']

nopInstructions = ['FNOP', 'NOP', 'NOPL', 'NOPW']

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

	header += 'Addition Instructions,'
	header += 'Subtraction Instructions,'
	header += 'Multiply Instructions,'
	header += 'Divide Instructions,'
	header += 'Jump Instructions,'
	header += 'No Operation (NOP) Instructions,'

	header += 'Operations,'
	outputFile.write(header + '\n')

	experimentData = {'key generation': '', 'signing': '', 'verifying': ''}

	readerFile = open('experiment_output.csv', 'r')
	reader = csv.reader(x.replace('\0', '') for x in readerFile)
	header = next(reader)
	baselineRow = next(reader)
	assert baselineRow[1] == 'N/A', 'The initial baseline row was not found'
	baselineJSON = json.loads(baselineRow[6].replace("'", '"'))
	for row in reader:
		algorithm = row[0]
		experiment = row[1]
		print('Processing', algorithm, experiment, '...')
		lineStart = row[2]
		lineEnd = row[3]
		addressStart = row[4]
		addressEnd = row[5]
		opcodes = json.loads(row[6].replace("'", '"'))
		# First, we remove any baseline data, since this contains the assembly instructions executed when no code is present
		for key in baselineJSON:
			if key in opcodes:
				opcodes[key] -= baselineJSON[key]

		totalOccurances = 0
		categorizedOutputs = {}
		categorizedIndividualInstructions = {'add': 0, 'subtract': 0, 'multiply': 0, 'divide': 0, 'jump': 0, 'nop': 0}
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

			if op.upper() in addInstructions:
				categorizedIndividualInstructions['add'] += opcodes[op]
			elif op.upper() in subtractInstructions:
				categorizedIndividualInstructions['subtract'] += opcodes[op]
			elif op.upper() in multiplyInstructions:
				categorizedIndividualInstructions['multiply'] += opcodes[op]
			elif op.upper() in divideInstructions:
				categorizedIndividualInstructions['divide'] += opcodes[op]
			elif op.upper() in jumpInstructions:
				categorizedIndividualInstructions['jump'] += opcodes[op]
			elif op.upper() in nopInstructions:
				categorizedIndividualInstructions['nop'] += opcodes[op]

		categorizedOutputs = dict(sorted(categorizedOutputs.items(), key=lambda item: item[1], reverse=True))
		line = ''
		line += algorithm + ','
		line += experiment + ','
		line += str(totalOccurances) + ','
		line += str(categorizedOutputs['Data Movement Instructions']) + ','
		line += str(categorizedOutputs['Arithmetic and Logic Instructions']) + ','
		line += str(categorizedOutputs['Control Flow Instructions']) + ','
		line += str(categorizedOutputs['Miscellaneous Instructions']) + ','

		line += str(categorizedIndividualInstructions['add']) + ','
		line += str(categorizedIndividualInstructions['subtract']) + ','
		line += str(categorizedIndividualInstructions['multiply']) + ','
		line += str(categorizedIndividualInstructions['divide']) + ','
		line += str(categorizedIndividualInstructions['jump']) + ','
		line += str(categorizedIndividualInstructions['nop']) + ','

		line += '"' + str(opcodes) + '",'
		experimentData[experiment.lower()] += line + '\n'
		print('   ', categorizedOutputs)

	outputFile.write(experimentData['key generation'])
	outputFile.write(experimentData['signing'])
	outputFile.write(experimentData['verifying'])
	readerFile.close()
	outputFile.close()
	print('Done!')
