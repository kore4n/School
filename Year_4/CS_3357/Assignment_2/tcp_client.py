# Assignment: TCP Simple Chat Room - TCP Client Code Implementation

# **Libraries and Imports**: 
#    - Import the required libraries and modules. 
#    You may need sys, socket, argparse, select, threading (or _thread) libraries for the client implementation.
#    Feel free to use any libraries as well.

# **Global Variables**:
#    - IF NEEDED, Define any global variables that will be used throughout the code.

# **Function Definitions**:
#    - In this section, you will implement the functions you will use in the client side.
#    - Feel free to add more other functions, and more variables.
#    - Make sure that names of functions and variables are meaningful.
#    - Take into consideration error handling, interrupts,and client shutdown.

import socket
import threading
import argparse
import sys

client_name = None
client_socket = None

event = threading.Event()

def receive():
    while True and not event.is_set():
        try:
            message = client_socket.recv(1024).decode('ascii')
            print(message)
        except Exception as error:
            # print("The following error occurred:", error)
            client_socket.close()

def write():
    while True and not event.is_set():
        messageContent = input("")
        message = f'{client_name}: {messageContent}'

        if (messageContent == "exit"):
            client_socket.close()
            event.set()
        else:
            client_socket.send(message.encode('ascii')) 

def run(clientSocket, username):
    # Cannot put prints here
    clientSocket.send(username.encode('ascii'))
    
    receiveThread = threading.Thread(target=receive)
    receiveThread.start()

    writeThread = threading.Thread(target=write)
    writeThread.start()

    receiveThread.join()
    writeThread.join()

    # Close down everything
    client_socket.close()
    sys.exit()

# **Main Code**:  
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Argument Parser')
    parser.add_argument('name')  # to use: python tcp_client.py username
    args = parser.parse_args()
    client_name = args.name
    server_addr = '127.0.0.1'
    server_port = 9301

    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM) #TCP
    client_socket.connect((server_addr, server_port))

    run(client_socket, client_name)