
'''
*****************************************************************************************
*
*        		=============================================
*           		Rapid Rescuer (RR) Theme (eYRC 2019-20)
*        		=============================================
*
*  This script is intended to check the output of the Task 1A
*  of Rapid Rescuer (RR) Theme (eYRC 2019-20).
*
*  Filename:			task_1a_cardinal.py
*  Created:				04/11/2019
*  Last Modified:		04/11/2019
*  Author:				e-Yantra Team
*  
*  This software is made available on an "AS IS WHERE IS BASIS".
*  Licensee/end user indemnifies and will keep e-Yantra indemnified from
*  any and all claim(s) that emanate from the use of the Software or 
*  breach of the terms of this agreement.
*  
*  e-Yantra - An MHRD project under National Mission on Education using ICT (NMEICT)
*
*****************************************************************************************
'''


# Import modules

import os
import sys
import string
import random
import base64
from datetime import datetime

import socket
from general import *
from networking.ethernet import Ethernet
from networking.ipv4 import IPv4
from networking.tcp import TCP
from networking.pcap import Pcap


from stat import S_IRWXU, S_IRWXG, S_IRWXO


# Random string generator function
def random_string(length = 10, char = string.ascii_lowercase + string.ascii_uppercase + string.digits + string.punctuation):

	return ''.join(random.choice(char) for x in range(length))


# Encoding string of characters
def encode(str_input):

	str_output = base64.b64encode(str_input.encode('utf-8')).decode('utf-8')
	
	return str_output


# Print any string in specific colour
def printColour(str, colour='', end=''):
    if colour == 'red':
        print ('\033[1;31m %s \033[0m' %str, end = end)
    elif colour == 'green':
        print ('\033[1;32m %s \033[0m' %str, end = end)
    elif colour == 'yellow':
        print ('\033[1;33m %s \033[0m' %str, end = end)
    elif colour == 'blue':
        print ('\033[1;34m %s \033[0m' %str, end = end)
    elif colour == 'magenta':
        print ('\033[1;35m %s \033[0m' %str, end = end)
    elif colour == 'cyan':
        print ('\033[1;36m %s \033[0m' %str, end = end)
    else:
        print ('\033[1;37m %s \033[0m' %str, end = end)


def main():

	# output txt file name
	output_file_name = 'task_1b_output.txt'

	# remove the previously generated output txt file if exists
	if os.path.exists(output_file_name):
		os.remove(output_file_name)

	pcap = Pcap('capture.pcap')
	conn = socket.socket(socket.AF_PACKET, socket.SOCK_RAW, socket.ntohs(3))

	data_from_server = ''
	data_from_client = ''

	try:

		output_file = open(output_file_name, 'w')
		
		while True:

			raw_data, addr = conn.recvfrom(65535)

			pcap.write(raw_data)
			eth = Ethernet(raw_data)

			now = datetime.now()
			current_time = now.strftime("%H:%M:%S")

			# IPv4
			if eth.proto == 8:
				ipv4 = IPv4(eth.data)

				# TCP
				if ipv4.proto == 6:

					tcp = TCP(ipv4.data)

					if len(tcp.data) > 0:

						if tcp.src_port == 3333:
							# print("[BOT to PC] [" + ipv4.src + "]" + " to " + "[" + ipv4.target + "]" + " > " + tcp.data.decode("utf-8"))
							print("-------------------------")
							printColour(current_time + " [BOT to PC] [" + ipv4.src + "]" + " to " + "[" + ipv4.target + "]" + " > ", "green", end='')
							printColour(tcp.data.decode("utf-8"), "red", end='\n')
							data_from_server = tcp.data.decode("utf-8")
							# print(type(data_from_server))

							if (data_from_server.count('@') == 2):
								output_file.write(encode(random_string(6) + str(data_from_server) + random_string(6)) + '\n')

						if tcp.dest_port == 3333:
							# print("[PC to BOT] [" + ipv4.src + "]" + " to " + "[" + ipv4.target + "]" + " > " + tcp.data.decode("utf-8"))
							print("-------------------------")
							printColour(current_time + " [PC to BOT] [" + ipv4.src + "]" + " to " + "[" + ipv4.target + "]" + " > ", "green", end='')
							printColour(tcp.data.decode("utf-8"), "cyan", end='\n')
							data_from_client = tcp.data.decode("utf-8")
							# print(type(data_from_client))

							if (data_from_client.count('#') == 2):
								output_file.write(encode(random_string(6) + str(data_from_client) + random_string(6)) + '\n')

						# if (data_from_server.count('@') == 2) and (data_from_client.count('#') == 2):
						# 	output_file.write(encode(random_string(6) + str(data_from_server) + random_string(6)) + '\n')
						# 	output_file.write(encode(random_string(6) + str(data_from_client) + random_string(6)) + '\n')

		pcap.close()
		output_file.close()

	except KeyboardInterrupt as e:
		print('\n\nSee ya !')
		os.chmod(output_file_name, (S_IRWXG | S_IRWXO | S_IRWXU))
		exit()

if __name__ == "__main__": 
	main()
