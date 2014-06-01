# This is a UPD server program from KR "Computer Networking" book 6th edition
# 01-27-2013
from socket import *
serverPort = 12000
serverSocket = socket( AF_INET, SOCK_DGRAM )
serverSocket.bind( ('', serverPort) )
print( 'The server is ready to receive' )
while 1:
    message, clientAddress = serverSocket.recvfrom( 2048 )
    modifiedMessage = message.upper()
    serverSocket.sendto(modifiedMessage, clientAddress)
