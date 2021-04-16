/**************
Author: Kimberly Tom
Project 1: TCP Chat Program
Description: client side code for chat program written in C
This is a chat system using TCP sockets that works for a pair of users, one being the client and one being the server.
Once chatserve is running on a port number, chatclient can connect to it by entering the same server and port number as chatserve is running on.  After that, the two are connected and the chat can begin.
It allows a client and a server to alternate sending and receiving messages to each other (up to 500 characters each message, inclusive of chat handle, "> " formatting, and spaces).
The testing machine I used was a Lenovo Z51 operating with Windows 10.
Program will work using two instances of PuTTY on one computer on flip1.engr.oregonstate.edu, 
flip2.engr.oregonstate.edu, and flip3.engr.oregonstate.edu.
CS372 Intro To Computer Networks
Last Modified: 11/3/19
**************/

#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h>  
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

// with help from https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/


// function getHandle obtains client's chat handle that is a one-word name and maximum of 10 characters
// Precondition: none 
// Postcondition: returns character array called chatHandle which holds the user's chosen chat handle that is no more than 10 characters
char * getHandle() {
	int handleTooLong = 1;
	int handleHasSpace = 1;
	int handleMax = 10; // handle can be a maximum of 10 characters
	static char chatHandle[12];
	
	// while the handle doesn't satisfy the conditions of one word or 10 characters, keep looping
	while (handleTooLong == 1 || handleHasSpace == 1) {
		// reset flags
		handleTooLong = 1;
		handleHasSpace = 1;
	
		printf("Enter your chat handle (one-word name, max: 10 characters):\n");
		fgets(chatHandle, 12, stdin);
		// remove newline character from chatHandle
		strtok(chatHandle, "\n"); // https://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input	
		
	
		int handleLength = strlen(chatHandle);
		int handleSize = sizeof(chatHandle);
	

	
		// if the chat handle length is shorter than or equal to the handleMax, handle satisfies size requirement
		if (handleLength <= handleMax) {
			handleTooLong = 0;
		
		
			// check if user's handle has any spaces
			int i;
			int spacePresent = 0;
			// for each character in the chatHandle, see if that character is a space
			for (i = 0; i < handleLength; i++) { 
				int ch = chatHandle[i]; 
				if (isspace(ch)) {
					spacePresent = 1;
				}
			}
			// if space present, then tell user to try again
			if (spacePresent == 1) {
				printf("There is a space in your handle,  Press Enter, then try again.\n");
				handleHasSpace = 1;
		
		
				// clear the fgets buffer
				// with help from https://stackoverflow.com/questions/38767967/clear-input-buffer-after-fgets-in-c
				char *check2;
				// see if a newline character exists
				if (check2=strchr(chatHandle, '\n')){
					*check2 = 0;
					} 
				// discard everything up to and including the newline character
				else {
					scanf("%*[^\n]");
					scanf("%*c");
				}
		}
	
			// else there are no spaces, exit while loop
			else {
				handleHasSpace = 0;
			}	
		
		}
		// else handle is too long, have user try again
		else {
			handleTooLong = 1;
		
			printf("Your chat handle is too long.\n");
		
			// clear the fgets buffer
			// with help from https://stackoverflow.com/questions/38767967/clear-input-buffer-after-fgets-in-c
			char *check;
			// see if a newline character exists
			if (check=strchr(chatHandle, '\n')){
				*check = 0;
				} 
			// discard everything up to and including the newline character
			else {
					scanf("%*[^\n]");
					scanf("%*c");
				}
		}	
	}

		printf("Your chat handle has been set to %s\n", chatHandle);
		int handleSize2 = sizeof(chatHandle);
		// printf("Your handle size is %d.\n", handleSize2);
		return chatHandle;
}


// function that creates TCP socket and connects to server
// Precondition: argv[1] and argv[2] must have some data to pass to the function
// Postcondition: returns socketFD that isn't equal to -1
int socketFunct(char *addressServ, char *numberPort) {
    int socketfd; 
    struct sockaddr_in serverAddress; 
  
    // create TCP socket
    socketfd = socket(AF_INET, SOCK_STREAM, 0); 
	
	// if socket wasn't created show error and exit
    if (socketfd == -1) { 
        printf("Failed to create socket.\n"); 
        exit(0); 
    } 
    else {
        printf("Socket has been successfully created.\n"); 
    }
	
	
	bzero(&serverAddress, sizeof(serverAddress)); 
  
    // obtain the IP address and the Port number from the command line
	// https://cboard.cprogramming.com/c-programming/134232-converting-command-line-char-*argv[]-int.html
    	
		
	// with help from http://beej.us/guide/bgnet/html/#getaddrinfoprepare-to-launch
	
	struct addrinfo hints; // provides hints regarding socket type
	struct addrinfo *result; // this will point to the linked list of addrinfo result
	struct addrinfo *p;
	int status;
	char ipstr[INET6_ADDRSTRLEN];


	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // either IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP stream
	
	
	if ((status = getaddrinfo(addressServ, NULL, &hints, &result)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return 2;
	}

	// obtain the pointer to the IP address
	for(p = result;p != NULL; p = p->ai_next) {
		void *addr;
		
		// if IPv4
		if (p->ai_family == AF_INET) { 
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
			addr = &(ipv4->sin_addr);

		// else if IPv6
		} else { 
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
			addr = &(ipv6->sin6_addr);

		}

		// convert the IP address to a string
		inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
	}		
	
	// connect the client socket to server socket 
	// with help from https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
	serverAddress.sin_family = AF_INET; 
	serverAddress.sin_addr.s_addr = inet_addr(ipstr); // set the server address to the IP address string
	int portNumber = atoi(numberPort); // set the port number to the third argument
	serverAddress.sin_port = htons(portNumber); 
	freeaddrinfo(result); // free the linked list
	
    
    // if failed to connect to server, show error and exit
    if (connect(socketfd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) != 0) { 
        printf("Connection to the server was unsuccessful.\n"); 
        exit(0); 
    } 
	// else show successfully connected message
    else {
        printf("You are now connected to server %s at port %s\n", addressServ, numberPort); 
    }
	
	return socketfd;
}


// function that lets client send messages to the server and receive messages from the server
// Precondition: socketFD and chatHandle must be sucessfully created
// Postcondition: shows messages sent to server and messages received from server on the screen
void chatFunct(int socketfd, char * username) { 
	// printf("your username length is %d\n", strlen(username));
	// printf("your username size is %d\n", sizeof(username));
    char buff[500]; // buffer to be received from server
	int sendMax = 502 - strlen(username) - 2; // this is the maximum our message can be to account for null chars, username length and "> " characters
	char msg[sendMax];  // the message the user wants to send to the server
	char usernameAndMessage[500]; // the username, message and "> " formatting that will be sent to the server
    int sizeMsg;
	int lengthMsg;
	int sizeUsernameAndMessage;
	int lengthUsernameAndMessage;
	const int sizeBuff = 500; // maximum size of what we can receive from the server
	int bytes_sent;
	int bytes_recv;
	int msgTooLong;
	
	// keep repeating until client or server quits 	
	for (;;) {  
        
		do {
		bzero(msg, sizeof(msg));
		bzero(usernameAndMessage, sizeof(usernameAndMessage));
		bzero(buff, sizeof(buff));
		printf("%s> ", username); 
		fgets(msg, sendMax, stdin);
		// remove newline character from msg
		strtok(msg, "\n");
		sizeMsg = sizeof(msg);
		lengthMsg = strlen(msg);
		// printf("size of just the message is %d\n", sizeMsg);
		// printf("length of just the message is %d\n", lengthMsg);
		
	
		// https://www.geeksforgeeks.org/how-to-append-a-character-to-a-string-in-c/
		strncat(usernameAndMessage, username, sizeof(username));
		strncat(usernameAndMessage, "> ", 2);
		strncat(usernameAndMessage, msg, sendMax);
		sizeUsernameAndMessage = sizeof(usernameAndMessage);
		lengthUsernameAndMessage = strlen(usernameAndMessage);
		// printf("size of usernameAndMessage is %d\n", sizeUsernameAndMessage);
		// printf("length of usernameAndMessage is %d\n", lengthUsernameAndMessage);
		// with help from https://beej.us/guide/bgnet/html/?#sendrecv
		// with help from https://stackoverflow.com/questions/16374847/socket-c-bytes-received-but-i-cant-print-the-string
		// with help from https://stackoverflow.com/questions/44744247/first-recv-cannot-read-message-sent-from-server
		
		// if the username and message is over 500 characters, inform user and have user try again
		if (lengthUsernameAndMessage > 500) {
			printf("Your message to the server is over 500 characters and is too long. try again\n\n");
			msgTooLong = 1;
			// clear the fgets buffer
			// with help from https://stackoverflow.com/questions/38767967/clear-input-buffer-after-fgets-in-c
			char *check3;
			// see if a newline character exists
			if (check3=strchr(msg, '\n')){
				*check3 = 0;
				} 
			// discard everything up to and including the newline character
			else {
					scanf("%*[^\n]");
					scanf("%*c");
				}
		}
		// else username and message combined length is 500 or less
		else {
			msgTooLong = 0;
		}
		
		// keep looping until the username and message combined  length is 500 or less
		} while (msgTooLong == 1);
		
		// send the bytes to the server
		bytes_sent = send(socketfd, usernameAndMessage, sizeUsernameAndMessage, 0);
		
		
		// if client enters \quit, exit out of infinite loop and close the connection
		if ((strncmp(msg, "\\quit", 4)) == 0) { 
			printf("Client is quitting the chat program. Goodbye\n"); 
			break; 
		} 
		
		
		memset(buff, 0, sizeBuff);
		
		// receive bytes from the server
		bytes_recv = recv(socketfd, buff, sizeBuff, 0);
		
		// quit the chat program if server quit
		// with help from https://stackoverflow.com/questions/12784766/check-substring-exists-in-a-string-in-c
		char *serverQuit = "\\quit";
		if(strstr(buff, serverQuit) != NULL) {
			printf("Server is quitting the chat program. Goodbye\n"); 
			break; 
		}
		
		
		printf("%s\n", buff);
		
    } 
} 


  
int main(int argc, char *argv[]) 
{ 
	// if the user did not enter three arguments including the program name, show error and exit
	if (argc != 3) {
	    fprintf(stderr,"error: you didn't enter the correct number of arguments\n");
	    return 1;
	}
	
	char *serverAdd = argv[1];
	char *portNum = argv[2];
	
	// pass the server addreses and port number arguemnts to create socket and TCP connection to server
	int sockFD = socketFunct(serverAdd, portNum);
	
	// obtain host B's (client's) chat handle
	// https://stackoverflow.com/questions/1496313/returning-c-string-from-a-function
	char * hostBHandle = getHandle();

    // function for chat 
    chatFunct(sockFD, hostBHandle); 
  
    // close the socket 
    close(sockFD); 
} 