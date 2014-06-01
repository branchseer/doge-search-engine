# This is a UPD client program from KR "Computer Networking" book 6th edition
# 01-27-2013
from socket import *
serverName = 'polaris'
serverPort = 12000
clientSocket = socket( AF_INET, SOCK_DGRAM )
message = input( 'Input lowercase sentence: ' )
clientSocket.sendto( str.encode(message), (serverName, serverPort) )
modifiedMessage, serverAddress = clientSocket.recvfrom( 2048 )
print( bytes.decode( modifiedMessage ) )
clientSocket.close()
