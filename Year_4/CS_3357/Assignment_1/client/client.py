# By Jason Kwan
# Student ID: 251173792
# Run with: "Python client.py <port #> <file name>" 
# Ex. "Python client.py 9200 hello.html"

import sys
from socket import socket, AF_INET, SOCK_STREAM

def main():
    serverIP = '127.0.0.1'
    # serverIP = 'localhost'
    # serverIP = '0.0.0.0'

    serverPort = int(sys.argv[1])
    fileName = sys.argv[2]

    clientSocket = socket(AF_INET, SOCK_STREAM)

    try:
        clientSocket.connect((serverIP, serverPort))
        print('Connnection established')
    except:
        print('Connnection could not be established')
        return

    clientSocket.send(f'GET /{fileName} HTTP/1.1\r\nHost: {serverIP}\r\n\r\n'.encode())

    # while True:
    try:
        # Parse & save the received file
        output = clientSocket.recv(1024)
        message = output.decode()
        print("Received message: ", message)

        if ("200 OK" in message):
            # print("200 OK received")
            fileDataList = message.split("\n")[2:]
            fileDataString = "".join(fileDataList)

            with open(fileName, 'w') as f:
                f.write(fileDataString)

        while True:
            pass                    

    except:
        clientSocket.close()
        print('Connection closed')
        print("Client process ended")
        # break
    
if __name__ == "__main__":
    main()