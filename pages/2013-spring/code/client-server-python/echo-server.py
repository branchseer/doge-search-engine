############################################################################
# Server side: open a TCP/IP socket on a port, listen for a message from
# a client, and send an echo reply; this is a simple one-shot listen/reply
# conversation per client, but it goes into an infinite loop to listen for
# more clients as long as this server script runs; the client may run on 
# a remote machine, or on same computer if it uses 'localhost' for server
############################################################################

# revised by X. Meng 01-19-2013 for CSCI 363, converted it to Python 3.x and
# removed extra elements, using only the minimum setup.

import sys
import string
from socket import *                    # get socket constructor and constants
myHost = ''                             # server machine, '' means local host
myPort = 50007                          # listen on a non-reserved port number

sockobj = socket(AF_INET, SOCK_STREAM)       # make a TCP socket object
sockobj.bind((myHost, myPort))               # bind it to server port number 
sockobj.listen(5)                            # listen, allow 5 pending connects
     
while True:                                  # listen until process killed
    connection, address = sockobj.accept()   # wait for next client connect
    print('Server connected by', address)    # connection is a new socket
    while True:
        data = connection.recv(1024)         # read next line on client socket
        if not data: break                   # send a reply line to the client
        echoMsg = 'Echo => ' + bytes.decode(data)
        connection.send(str.encode(echoMsg))     
                                             # until eof when socket closed
    connection.close() 
