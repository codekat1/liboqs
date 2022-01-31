import csv
import datetime
import os
import re
import sys
import time

def isSudo():
	return os.geteuid() == 0

if not isSudo():
	print(f'\nPlease run the script as sudo:\n\tsudo python3 {os.path.basename(__file__)}\n')
	sys.exit()

# Send a command to the linux terminal
def terminal(cmd):
	return os.popen(cmd).read()

# def stopPowertops():
# 	terminal('sudo pkill powertop')

def startPowerTop():
	if isPowerTopRunning(): return
	
	exists = terminal('which powertop') != ''

	if not exists:
		print('\nPowerTOP does not exist. Installing...')
		terminal('sudo apt-get install powertop')
		print('\nPlease run "sudo powertop --calibrate" before proceeding.')
		sys.exit()

	terminal('sudo gnome-terminal -- powertop')

def isPowerTopRunning():
	process = terminal('ps -A | grep powertop')
	return process != ''

def getPowerTopInfo():
	powertopVersion = ''
	kernelVersion = ''
	systemName = ''
	cpuInformation = ''
	osInformation = ''
	powerUsageBaseline = '0'
	powerUsageBaselineUnit = ''
	powerUsage = '0'
	powerUsageUnit = ''
	signaturesPowerUsage = '0'
	signaturesPowerUnit = ''


	if not isPowerTopRunning(): return {
		'powertopVersion': powertopVersion,
		'kernelVersion': kernelVersion,
		'systemName': systemName,
		'cpuInformation': cpuInformation,
		'osInformation': osInformation,
		'powerUsage': powerUsage,
		'powerUsageUnit': powerUsageUnit,
		'powerUsageBaseline': powerUsageBaseline,
		'powerUsageBaselineUnit': powerUsageBaselineUnit,
		'signaturesPowerUsage': signaturesPowerUsage,
		'signaturesPowerUnit': signaturesPowerUnit
	}

	powertopPath = os.path.expanduser('~/powertop.csv')
	print(f'Creating {powertopPath}')
	temp = open(powertopPath, 'w+')
	temp.close()
	output = terminal('sudo powertop --time=1 --csv=' + powertopPath)
	print('\n\n\nPowertop output: "' + output + '"')

	_powertopVersion = terminal(f'cat {powertopPath} | grep "PowerTOP Version;"')
	match = re.match('PowerTOP Version;([^;\n]+)', _powertopVersion)
	if match is not None:
		powertopVersion = match.group(1)
	
	_kernelVersion = terminal(f'cat {powertopPath} | grep "Kernel Version;"')
	match = re.match('Kernel Version;([^;\n]+)', _kernelVersion)
	if match is not None:
		kernelVersion = match.group(1)
	
	_systemName = terminal(f'cat {powertopPath} | grep "System Name;"')
	match = re.match('System Name;([^;\n]+)', _systemName)
	if match is not None:
		systemName = match.group(1)
	
	_cpuInformation = terminal(f'cat {powertopPath} | grep "CPU Information;"')
	match = re.match('CPU Information;([^;\n]+)', _cpuInformation)
	if match is not None:
		cpuInformation = match.group(1)
	
	_osInformation = terminal(f'cat {powertopPath} | grep "OS Information;"')
	match = re.match('OS Information;([^;\n]+)', _osInformation)
	if match is not None:
		osInformation = match.group(1)

	_powerUsage = terminal(f'cat {powertopPath} | grep "The battery reports a discharge rate of:"')
	match = re.match('The battery reports a discharge rate of:\s*([0-9\.]+)([^;\n]+)', _powerUsage)
	if match is not None:
		powerUsage = match.group(1)
		powerUsageUnit = match.group(2)

	_powerUsageBaseline = terminal(f'cat {powertopPath} | grep "The system baseline power is estimated at:"')
	match = re.match('The system baseline power is estimated at:\s*([0-9\.]+)([^;\n]+)', _powerUsageBaseline)
	if match is not None:
		powerUsageBaseline = match.group(1)
		powerUsageBaselineUnit = match.group(2)

	_signaturesUsage = terminal(f'cat {powertopPath} | grep "] ./Signatures.o ; "').strip()
	match = re.match(r'[^\]]+\] \./Signatures.o ; +([0-9\.]+) ([a-zA-Z]+)', _signaturesUsage)
	if match is not None:
		signaturesPowerUsage = match.group(1)
		signaturesPowerUnit = match.group(2)

	return {
		'powertopVersion': powertopVersion,
		'kernelVersion': kernelVersion,
		'systemName': systemName,
		'cpuInformation': cpuInformation,
		'osInformation': osInformation,
		'powerUsage': powerUsage,
		'powerUsageUnit': powerUsageUnit.replace(' ', ''),
		'powerUsageBaseline': powerUsageBaseline,
		'powerUsageBaselineUnit': powerUsageBaselineUnit.replace(' ', ''),
		'signaturesPowerUsage': signaturesPowerUsage,
		'signaturesPowerUnit': signaturesPowerUnit
	}

startPowerTop()

outputFile = open('loggedPowerOutput.csv', 'w+')
line = ''
line += 'Timestamp,'
line += 'Timestamp (s),'
	line += 'CPU %,'
	line += 'CPU Frequency,'
	line += 'Virtual Memory %,'
	line += 'Virtual Memory,'
	line += 'Swap Memory %,'
	line += 'Swap Memory,'
	line += 'Disk Usage %,'
	line += 'Disk Usage,'
line += 'Powertop Version,'
line += 'Kernel Version,'
line += 'System Name,'
line += 'CPU Stats,'
line += 'OS Stats,'
line += 'Power Usage,'
line += 'Unit,'
line += 'Baseline Power,'
line += 'Unit,'
line += 'Signatures Power,'
line += 'Signatures Unit,'
outputFile.write(line + '\n')

print(getPowerTopInfo())

sampleCounter = 1
while True:
	time.sleep(1)
	try:
		now = datetime.datetime.now()
		seconds = (now - datetime.datetime(1970, 1, 1)).total_seconds()

		powerStats = getPowerTopInfo()
		line = ''
		line += str(now) + ','
		line += str(seconds) + ','


		# CPU, memory, disk usage logger
		cpu = psutil.cpu_percent()
		cpu_f = 0
		try:
			cpu_f = psutil.cpu_freq().current
		except: pass
		v_mem_p = psutil.virtual_memory().percent
		v_mem = psutil.virtual_memory().used
		s_mem_p = psutil.swap_memory().percent
		s_mem = psutil.swap_memory().used
		disk_usage_p = psutil.disk_usage('/').percent
		disk_usage = psutil.disk_usage('/').used

		line += str(cpu) + ','
		line += str(cpu_f) + ','
		line += str(v_mem_p) + ','
		line += str(v_mem) + ','
		line += str(s_mem_p) + ','
		line += str(s_mem) + ','
		line += str(disk_usage_p) + ','
		line += str(disk_usage) + ','

		# Power logger
		line += str(powerStats['powertopVersion']) + ','
		line += str(powerStats['kernelVersion']) + ','
		line += str(powerStats['systemName']) + ','
		line += str(powerStats['cpuInformation']) + ','
		line += str(powerStats['osInformation']) + ','
		line += str(powerStats['powerUsage']) + ','
		line += str(powerStats['powerUsageUnit']) + ','
		line += str(powerStats['powerUsageBaseline']) + ','
		line += str(powerStats['powerUsageBaselineUnit']) + ','
		line += str(powerStats['signaturesPowerUsage']) + ','
		line += str(powerStats['signaturesPowerUnit']) + ','
		outputFile.write(line + '\n')

		print(f'Logged power sample #{sampleCounter}')
		sampleCounter += 1
	except KeyboardInterrupt:
		break