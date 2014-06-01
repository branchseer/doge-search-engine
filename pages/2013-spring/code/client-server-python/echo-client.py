############################################################################
# Client side: use sockets to send data to the server, and print server's 
# reply to each message line; 'localhost' means that the server is running
# on the same machine as the client, which lets us test client and server 
# on one machine;  to test over the Internet, run a server on a remote
# machine, and set serverHost or argv[1] to machine's domain name or IP addr;
# Python sockets are a portable BSD socket interface, with object methods 
# for the standard socket calls available in the sytstem's C library;
############################################################################

# revised by X. Meng 01-19-2013 for CSCI 363, converted it to Python 3.x and
# removed extra elements, using only the minimum setup.
     
import sys
import string
from socket import *              # portable socket interface plus constants
serverHost = 'localhost'          # server name, or: 'starship.python.net'
serverPort = 50007                # non-reserved port used by the server
     
message = 'Hello network world'          # default text to send to server
     
sockobj = socket(AF_INET, SOCK_STREAM)     # make a TCP/IP socket object
sockobj.connect((serverHost, serverPort))  # connect to server machine and port

sockobj.send(str.encode(message))     # send line to server over socket
data = sockobj.recv(1024)             # receive line from server: up to 1k
print('Client received:', bytes.decode(data))
                                      # make sure it is quoted, was `x`
     
sockobj.close()                       # close socket to send eof to server
