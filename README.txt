Kimberly Tom
11/3/19
CS 372

Readme file for Project 1, TCP Chat Program:

chatserve.py was written in python.
chatclient.c was written in C.


The testing machine I used was a Lenovo Z51 operating with Windows 10.
Testing was done using two instances of PuTTY on one computer  on flip1.engr.oregonstate.edu, 
flip2.engr.oregonstate.edu, and flip3.engr.oregonstate.edu.

Unzip the folder containing the two programs, chatclient.c and chatserve.py

Open an instance of PuTTY, log in, and navigate to the folder containing chatserve.py
Make note of which flip server you are on. You can do this by typing "hostname" into the command line. 

To compile and run chatserve.py, type:
python3 chatserve.py [PORTNUM]

Enter your desired port number where it says [PORTNUM].
For example [port number] can be replaced with "35698", hit enter. If that port is not in use,
the chat server should now be running and it should say "The server is ready to receive a client to chat with".
If the port is in use, try another port number until you see "The server is ready to receive a client to chat with".

Open a second instance of PuTTY on the same computer, log in, and navigate to the folder containing chatclient.c

To compile chatclient.c, type: 
gcc -o chatclient chatclient.c

Make sure chatserve.py is still running and waiting to receive a client in your first instance of PuTTY.

If it is, you can start the chatclient program in the second instance of PuTTY by typing:
./chatclient [SERVER] [PORTNUM]
For [SERVER], type the server that your first instance of PuTTY where chatserve.py is running (you should've gotten this by typing "hostname" into first instance of PuTTY as mentioned earlier)
For [PORTNUM], use the same port number as you did in the first instance of PuTTY when you started chatserve.py

Now, the chatclient program should tell you that the socket has been successfully created and that you are now
connected to [SERVER] at [PORTNUM].  The chat program is now running and you can follow the prompt by entering your handle.

Now the client can send a message up to 500 characters to the server (inclusive of handle).  
Switch back and forth between the two PuTTY terminal windows to chat.  Client and server can alternate sending each other messages.
After client has sent a message, the server can respond with a message up to 500 characters (inclusive of handle, formatting "> " and spaces).
After client sends a message, it cannot send another message to the server until server has sent a message back.
After the server has sent a message, it cannot send another message to the client until the client has sent a message back.

If client types "\quit" and sends it to server, client quits the chat and the connection is closed. The server is now ready to receive a client to chat with. Client's program has closed.
If the server types "\quit" and sends it to the client, server quits the chat and the connection is closed.  The server is now ready to receive a client to chat with. Client's program has closed.

The client program can be started up again by typing "./chatclient [SERVER] [PORTNUM]" in the second PuTTY terminal
if chatserve.py is still running where the [SERVER] and the [PORTNUM] match chatserve's server and port number.

chatserve.py will be ready to receive clients until a SIGINT is received. SIGINT (control + C) will terminate the program. 

