# Assignment: TCP Simple Chat Room - TCP Server Code Implementation

# **Libraries and Imports**: 
#    - Import the required libraries and modules. 
#    You may need socket, threading, select, time libraries for the client.
#    Feel free to use any libraries as well.

# **Global Variables**:
#    - IF NEEDED, Define any global variables that will be used throughout the code.

# **Function Definitions**:
#    - In this section, you will implement the functions you will use in the server side.
#    - Feel free to add more other functions, and more variables.
#    - Make sure that names of functions and variables are meaningful.

import socket
import threading

clients = [] #list to add the connected client sockets , feel free to adjust it to other place
usernames = []

event = threading.Event()

def broadcast(message):
    for client in clients:
        client.send(message)

def handle(serverSocket, addr):
    while True and not event.is_set():
        try: 
            message = serverSocket.recv(1024)

            print(f"Message received from {addr}: {message}")
            broadcast(message)

        except:
            print("Handling client disconnect/server error.")

            index = clients.index(serverSocket)
            clients.remove(serverSocket)
            serverSocket.close()
            username = usernames[index]
            # broadcast(f'{username} has left the chat room.'.encode('ascii'))
            usernames.remove(username)
            break

def run(serverSocket, serverPort):
    print(f"\nCHATROOM\n\nThis is the server side.\nI am ready to receive connections on port {serverPort}")

    try:
        while True:
            serverSocket, addr = server_socket.accept()  

            username = serverSocket.recv(1024).decode('ascii')
            usernames.append(username)
            clients.append(serverSocket)

            print(f"User {username} joined from address:{addr[0]}:{addr[1]}")
            # broadcast(f'{username} has joined the chat!'.encode('ascii'))
            # serverSocket.send('Connected to the server!'.encode('ascii'))

            thread = threading.Thread(target=handle, args=(serverSocket, addr))
            thread.start()
    except:
        print("An error occurred")
        

# **Main Code**:  
if __name__ == "__main__":
    server_port = 9301
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)# Creating a TCP socket.
    server_socket.bind(('127.0.0.1', server_port))
    server_socket.listen(3) # size of the waiting_queue that stores the incoming requests to connect.
    
    run(server_socket, server_port)# Calling the function to start the server.

