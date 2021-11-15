import csv
import os
import re

# List the files with a regular expression
def listFiles(regex, directory):
	path = os.path.join(os.curdir, directory)
	return [os.path.join(path, file) for file in os.listdir(path) if os.path.isfile(os.path.join(path, file)) and bool(re.match(regex, file))]

def listToCSV(data):
	line = ''
	for item in data:
		line += '"' + str(item).replace('"', '""') + '",'
	return line + '\n'

if __name__ == '__main__':

	directory = 'ScalabilitySamples'
	files = listFiles(r'SAMPLE_[0-9]+_[A-Za-z0-9]+\.csv', directory)
	outputFile = open(f'COMBINED_{len(files)}_SCALABILITY_SAMPLES.csv', 'w', newline='')

	data_sum = []
	data_counter = []
	
	# Initialize the data structure by reading one of the files, and mimicking the number of rows and columns with arrays of zero
	_readerFile = open(files[0], 'r')
	# Remove NUL bytes to prevent errors
	_reader = csv.reader(x.replace('\0', '') for x in _readerFile)
	for row in _reader:
		data_sum.append([0] * len(row))
		data_counter.append([0] * len(row))
	_readerFile.close()

	# Begin looping through the files
	for file in files:
		print(f'Processing {file}')
		readerFile = open(file, 'r')
		# Remove NUL bytes to prevent errors
		reader = csv.reader(x.replace('\0', '') for x in readerFile)

		i = 0
		for row in reader:
			j = 0
			for cell in row:
				if re.match(r'^-?[0-9]+(\.[0-9]*)?$', cell.strip()) is not None:
					num = float(cell)
					data_sum[i][j] += num
					data_counter[i][j] += 1
				elif data_sum[i][j] == 0:
					data_sum[i][j] = cell
				j += 1
			i += 1

		readerFile.close()

	# Convert the sum into averages
	for i in range(len(data_sum)):
		for j in range(len(data_sum[i])):
			if (type(data_sum[i][j]) is int or type(data_sum[i][j]) is float) and data_counter[i][j] != 0:
				data_sum[i][j] /= data_counter[i][j]

	# Convert the array into CSV format
	for row in data_sum:
		line = listToCSV(row)
		outputFile.write(line)

	outputFile.close()
	print('Done.')