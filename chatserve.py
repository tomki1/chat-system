# Author: Kimberly Tom
# Project 1: TCP Chat Program
# Description: server side code for chat program written in Python
# This is a chat system using TCP sockets that works for a pair of users, one being the client and one being the server.
# Once chatserve is running on a port number, chatclient can connect to it by entering the same server and port number as chatserve is running on.  After that, the two are connected and the chat can begin.
# It allows a client and a server to alternate sending and receiving messages to each other (up to 500 characters each message, inclusive of chat handle, "> " formatting, and spaces).
# The testing machine I used was a Lenovo Z51 operating with Windows 10.
# Program will work using two instances of PuTTY on one computer on flip1.engr.oregonstate.edu, 
# flip2.engr.oregonstate.edu, and flip3.engr.oregonstate.edu.
# CS372 Intro To Computer Networks
# Last Modified: 11/3/19

import sys
from socket import *


serverPort = int(sys.argv[1]) # https://stackoverflow.com/questions/17383470/argv-string-into-integer
serverSocket = socket(AF_INET,SOCK_STREAM)
serverSocket.bind(('',serverPort))
serverSocket.listen(1)


print("The server is ready to receive a client to chat with")


# with help from 2.7 from Computer Networking - A Top-down Approach by James F. Kurose, Keith W. Ross

# while SIGINT has not been received, keep looping
while True:
	connectionSocket, addr = serverSocket.accept()
	
	# while the client nor server typed \quit, keep looping
	while 1:
		messageOK = False
		# receive messages up to 500 characters from client
		messageFrom = connectionSocket.recv(500).decode()
		
		if "\quit" in messageFrom:
			print("Client is quitting the chat program. Goodbye")
			print("The server is ready to receive client to chat with")
			break
			
		print(messageFrom)
		
		# while the message has not been sent yet/not OK
		while not messageOK:
			# obtain message from command line
			messageTo = input('HostA> ')
			# combine the username string with the message to be sent
			# with help from https://stackoverflow.com/questions/4435169/how-do-i-append-one-string-to-another-in-python
			usernameAndMessage = 'hostA> ' + messageTo
			# print(len(usernameAndMessage))
		
			# if the username and message is greater than 500, show error and prompt server to try again
			if len(usernameAndMessage) > 500:
				print("Your message to the client is over 500 characters and is too long. try again")
				messageOK = False
			else:
				messageOK = True
		
		# send the username and message to the client
		connectionSocket.send(usernameAndMessage.encode())
		
		# if server types \quit, then close the connection and exit loop
		if "\quit" in messageTo:
			print("Server is quitting the chat program. Goodbye")
			print("The server is ready to receive a client to chat with")
			break
	

	# close the connection
	connectionSocket.close()