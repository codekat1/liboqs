import datetime
import os 
import psutil
import time

process_name = 'Signatures_power.o'

# Send a command to the linux terminal and return its response
def terminal(cmd):
	return os.popen(cmd).read()

def header():
	line = 'Timestamp,'
	line += 'Timestamp (Seconds),'
	line += 'CPU %,'
	line += 'CPU Frequency,'
	line += 'Virtual Memory (%),'
	line += 'Virtual Memory (B),'
	line += 'Swap Memory (%),'
	line += 'Swap Memory (B),'
	line += 'Disk Usage (%),'
	line += 'Disk Usage (B),'
	line += 'Process Name,'
	line += 'Process ID,'
	line += 'Process Priority,'
	line += 'Process Nice Value,'
	line += 'Process Virtual Memory (B),'
	line += 'Process Memory (B),'
	line += 'Process Shared Memory (B),'
	line += 'Process Memory (%),'
	line += 'Process CPU State,'
	line += 'Process CPU (%),'
	return line

def log(file):
	process_data = log_specific_process(process_name)
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

	now = datetime.datetime.now()
	time_end = (now - datetime.datetime(1970, 1, 1)).total_seconds()

	line = f'{now},{time_end},{cpu},{cpu_f},{v_mem_p},{v_mem},{s_mem_p},{s_mem},{disk_usage_p},{disk_usage},'

	line += f'{process_data["process_name"]},{process_data["process_ID"]},{process_data["priority"]},{process_data["nice_value"]},{process_data["virtual_memory"]},{process_data["memory"]},{process_data["shared_memory"]},{process_data["memory_percent"]},{process_data["state"]},{process_data["cpu_percent"]}'

	file.write(line + '\n')

# Given a raw memory string from the linux "top" command, return the number of bytes
# 1 EiB = 1024 * 1024 * 1024 * 1024 * 1024 * 1024 bytes
# 1 PiB = 1024 * 1024 * 1024 * 1024 * 1024 bytes
# 1 GiB = 1024 * 1024 * 1024 * 1024 bytes
# 1 MiB = 1024 * 1024 * 1024 bytes
# 1 KiB = 1024 * 1024 bytes
def top_mem_to_bytes(mem):
	if mem.endswith('e'): return float(mem[:-1]) * 1024 * 1024 * 1024 * 1024 * 1024 * 1024 # exbibytes to bytes
	elif mem.endswith('p'): return float(mem[:-1]) * 1024 * 1024 * 1024 * 1024 * 1024 # gibibytes to bytes
	elif mem.endswith('t'): return float(mem[:-1]) * 1024 * 1024 * 1024 * 1024 # tebibytes to bytes
	elif mem.endswith('g'): return float(mem[:-1]) * 1024 * 1024 * 1024 # gibabytes to bytes
	elif mem.endswith('m'): return float(mem[:-1]) * 1024 * 1024 # mebibytes to bytes
	else: return float(mem) * 1024 # kibibytes to bytes

def log_specific_process(process_id):
	raw = terminal('top -b -n 1 |grep ' + process_id).strip().split()
	while len(raw) < 12: raw.append('0') # Fill in with zeros for any missing values
	output = {
		'process_ID': raw[0],
		'user': raw[1],
		'priority': raw[2],
		'nice_value': raw[3],
		'virtual_memory': str(top_mem_to_bytes(raw[4])),
		'memory': str(top_mem_to_bytes(raw[5])),
		'shared_memory': str(top_mem_to_bytes(raw[6])),
		'state': raw[7],
		'cpu_percent': raw[8],
		'memory_percent': raw[9],
		'time': raw[10],
		'process_name': raw[11],

	}
	return output

def run(file):
	count = 0
	try:
		while True:
			count += 1
			log(file)
			if count % 10 == 0:
				print(f'Logged {count / 2} seconds')
			time.sleep(0.5)
	except KeyboardInterrupt:
		pass

if not os.path.exists('fullyAutomatedLogs'):
	os.makedirs('fullyAutomatedLogs')

fileName = os.path.join('fullyAutomatedLogs', f'LOGGED_CPU_{process_name}.csv')
file = open(fileName, 'w+')
file.write(header() + '\n')
run(file)