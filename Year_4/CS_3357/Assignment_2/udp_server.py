# Assignment: UDP Simple Chat Room - UDP Server Code Implementation

# **Libraries and Imports**: 
#    - Import the required libraries and modules. 
#    You may need socket, select, time libraries for the client.
#    Feel free to use any libraries as well.

# **Global Variables**:
#    - IF NEEDED, Define any global variables that will be used throughout the code.

# **Function Definitions**:
#    - In this section, you will implement the functions you will use in the server side.
#    - Feel free to add more other functions, and more variables.
#    - Make sure that names of functions and variables are meaningful.

import socket
import threading
import queue
import sys

messages = queue.Queue()   # Queue of messages
clients = [] #list to add the connected client sockets , feel free to adjust it to other place

event = threading.Event()

def receive():
    try:
        while True and not event.is_set():
            try:
                message, addr = serverSocket.recvfrom(1024)
                messages.put((message, addr))
            except:
                pass
    except:
        print("Server closed.")

def broadcast():
    try:
        while True and not event.is_set():
            while not messages.empty():
                message, addr = messages.get()

                print(f"Message received from {addr}: {message.decode()}")

                if message.decode().startswith("leaving:"):
                    name = message.decode()[message.decode().index(":") + 1:]
                    print(f"User {name} left from address:{addr[0]}:{addr[1]}")
                    continue
                    
                if addr not in clients:
                    clients.append(addr)
                for client in clients:
                    try:
                        if message.decode().startswith("joining:"):
                            name = message.decode()[message.decode().index(":") + 1:]
                            # serverSocket.sendto(f"{name} joined.".encode(), client)

                            print(f"User {name} joined from address:{addr[0]}:{addr[1]}")
                        else:
                            print(f"sending message to user {client}")
                            serverSocket.sendto(message, client)
                    except Exception as error:
                        print("Removing from clients list with error:", error)
                        clients.remove(client)
    except:
        print("An error occurred.")

                    

def run(serverSocket, serverPort):
    serverAddr = "127.0.0.1"
    serverSocket.bind((serverAddr, serverPort))

    print(f"\nCHATROOM\n\nThis is the server side.\nI am ready to receive connections on port {serverPort}")

    receiveThread = threading.Thread(target=receive)
    receiveThread.start()
    broadcastThread = threading.Thread(target=broadcast)
    broadcastThread.start()

    try:
        while True:
            pass
    except KeyboardInterrupt:
        print("\ninterrupt received: shutting down\nserver shut down\n")
        event.set()
        serverSocket.close()
        sys.exit()

    # print("test")

# **Main Code**:  
if __name__ == "__main__":
    
    serverPort = 9301  # Set the `serverPort` to the desired port number (e.g., 9301).
    serverSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # Creating a UDP socket.
    run(serverSocket, serverPort)  # Calling the function to start the server.