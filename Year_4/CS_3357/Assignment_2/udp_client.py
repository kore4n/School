# Assignment: UDP Simple Chat Room - UDP Client Code Implementation

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
import argparse
import threading
import time
import random

clientSocket = None
serverAddr = None
event = threading.Event()

def receive():
    while True and not event.is_set():
        try:
            message, addr = clientSocket.recvfrom(1024)
            print(message.decode(), flush=True)
        except:
            pass


def run(clientSocket, clientname, serverAddr, serverPort):
    clientSocket.bind((serverAddr, random.randint(8000, 10000)))

    receiveThread = threading.Thread(target=receive)
    receiveThread.start()

    clientSocket.sendto(f"joining:{clientname}".encode(), (serverAddr, serverPort))

    while True:
        time.sleep(0.5)
        # message = input(f"{clientname}: ")
        message = input()
        if (message == "exit"):
            print("Client Closing.........")
            print("socket of thread will terminate\n")

            clientSocket.sendto(f"leaving:{clientname}".encode(), (serverAddr, serverPort))
            clientSocket.close()
            event.set()
            exit()
        clientSocket.sendto(f"{clientname}: {message}".encode(), (serverAddr, serverPort))
    
# **Main Code**:  
if __name__ == "__main__":
    
    # Arguments: name address
    parser = argparse.ArgumentParser(description='argument parser')
    parser.add_argument('name')  # to use: python udp_client.py username
    args = parser.parse_args()
    clientname = args.name
    serverAddr = '127.0.0.1'
    serverPort = 9301
    clientSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP

    run(clientSocket, clientname, serverAddr, serverPort)  # Calling the function to start the client.