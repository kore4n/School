# By Jason Kwan
# Student ID: 251173792
# Run with: "Python server.py <port #> <max # of clients>" 
# Ex. "Python server.py 9200 3"

import sys
import threading
from socket import socket, AF_INET, SOCK_STREAM

# ipAddress = "localhost"
# ipAddress = "0.0.0.0"
ipAddress = "127.0.0.1"

def activeConnection(connectionSocket, addr):
    print(f"New connection at {addr} connected!")

    canAcceptNewConnection = threadCount.acquire(blocking=False)
    if (canAcceptNewConnection is False):
        print("Handling client disconnect. Server is full!")
        connectionSocket.close()
        return

    while True:
        try: 
            message = connectionSocket.recv(1024).decode()

            # Close client connections gracefully
            if (message is None or message == ''):
                print("Disconnecting client!")
                threadCount.release()
                connectionSocket.close()
                return

            if (message.startswith("GET") and "HTTP" in message):
                # Search and retrieve html file 
                # In the form "<filename> HTTP"
                fileNameAndProtocol = message.split('/')[1]
                # print("File name and protocol", fileNameAndProtocol)

                fileName = fileNameAndProtocol[0:-5]
                # print("File name is", fileName)
                # File exists -> Read + send
                f = open(fileName)
                content = f.read()
                f.close()
                response = "HTTP/1.1 200 OK\r\n\r\n" + content
                connectionSocket.send(response.encode())

            if message is not None:
                # print('Received message:', message)
                # print("Received a message!")
                pass

        except:
            # File doesn't exist -> Send 404 HTTP message to the client
            print("Handling client disconnect/server error.")

            connectionSocket.send("HTTP/1.1 404 Not Found\r\n\r\n".encode())
            connectionSocket.close()
            threadCount.release()
            return
            # break

def main():
    try:
        serverSocket = socket(AF_INET, SOCK_STREAM)

        serverPort = int(sys.argv[1])        # First argument
        max_connections = int(sys.argv[2])   # Second argument

        global threadCount
        threadCount = threading.Semaphore(max_connections)

        serverSocket.bind((ipAddress, serverPort))
        serverSocket.listen(max_connections)
        print("Server started on port", serverPort)

        while True:
            connectionSocket, addr = serverSocket.accept()  
            print("Connection accepted")

            thread = threading.Thread(target=activeConnection, args=(connectionSocket, addr))
            thread.start()
    except:
        print("Error creating server.")
if __name__ == "__main__":
    main()
