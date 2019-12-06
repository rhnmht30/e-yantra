import socket
import os
import subprocess
import random

s = socket.socket()
host = '127.0.0.1'
port = 3333

s.connect((host, port))
data = ''
file_num = [0, 1, 2, 3, 4]
for i in file_num:
    while '$' not in data:
        str1 = str(i) + random.choice('abcdefghijklomnoqrt')
        s.sendall(str1.encode("utf-8"))
        print('Data Sent: ' + str1)
        data = s.recv(1024)
        data = data.decode("utf-8")
        print('Data Mila: ' + data)
    data = ''
s.close()
# output_byte = str1.read()
# output_str = str(output_byte, "utf-8")
# s.send(str.encode(output_str)
