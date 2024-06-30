import numpy as np
import socket
from _thread import *
import pickle
from snake import SnakeGame
import uuid
import time 
import rsa

# server = "10.11.250.207"
server = "localhost"
port = 5555
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

gameHasStartedYet = False
counter = 0 
rows = 20 

try:
    s.bind((server, port))
except socket.error as e:
    str(e)

s.listen(2)
# s.settimeout(0.5)
print("Waiting for a connection, Server Started")


game = SnakeGame(rows)
game_state = "" 
last_move_timestamp = time.time()
interval = 0.2
moves_queue = set()

# tuple in the form (uuid, message)
messages_queue_tuples = []

clients = [] #list to add the connected client sockets , feel free to adjust it to other place

def broadcast(message_tuple):
    message_prefix = "|chatmessage|"
    # Message prefix in bytestring
    # \x7c\x63\x68\x61\x74\x6d\x65\x73\x73\x61\x67\x65\x7c
    # message = message_prefix + message_tuple[0] + ": " + message_tuple[1]
    message = message_tuple[0] + ": " + message_tuple[1]
    # message_separator = b'\x7c\x63\x68\x61\x74\x6d\x65\x73\x73\x61\x67\x65\x7c'
    message_separator = message_prefix.encode()

    # client is tuple in the form (clientSocket, client_public_key)
    for client in clients:
        clientSocket = client[0]
        client_public_key = client[1]
        # print(f"Sending with public key {client_public_key}")
        encryptedMessage = rsa.encrypt(message.encode(), client_public_key)
        
        clientSocket.send(message_separator)
        clientSocket.send(encryptedMessage)

def game_thread() : 
    global game, moves_queue, game_state 
    while True :
        last_move_timestamp = time.time()
        game.move(moves_queue)
        moves_queue = set()
        game_state = game.get_state()
        while time.time() - last_move_timestamp < interval : 
            time.sleep(0.1) 


def player_thread(clientSocket, client_unique_id):
    global clients
    server_public_key, server_private_key = rsa.newkeys(1024)

    # print(client_public_key)
    # print(client_private_key)

    # Server: Use client_public_key to encode.
    # Server: Use server_private_key to decode

    # Exchange 1: Receive client_public_key from client
    client_public_key = rsa.PublicKey.load_pkcs1(clientSocket.recv(1024))

    # Exchange 2: Send server_public_key to client
    clientSocket.send(server_public_key.save_pkcs1("PEM"))

    # print(f"Client public key: {client_public_key}")

    clients.append((clientSocket, client_public_key))

    while True :
        encryptedData = clientSocket.recv(1024)
        # print(game_state)
        # print(f"Received encrypted data: {encryptedData}")
        data = rsa.decrypt(encryptedData, server_private_key).decode()
        
        # print(f"Data is {data}")
        # clientSocket.send(game_state.encode())

        move = None 
        if not data :
            print("no data received from client")
            break 
        elif data == "get" : 
            clientSocket.send(game_state.encode())
            # print("received get")
            pass 
        elif data == "quit" :
            print("received quit")
            game.remove_player(client_unique_id)

            break
        elif data == "reset" : 
            game.reset_player(client_unique_id)

        elif data in ["up", "down", "left", "right"] : 
            move = data
            moves_queue.add((client_unique_id, move))
            clientSocket.send(game_state.encode())
        elif data.startswith("chatmessage|"):
            # Data is in the form
            # chatmessage|puremessage
            # e.x. chatmessage|It works!
            pure_messsage = data.split("|")[1]
            message_tuple = (client_unique_id, pure_messsage)
            # print(f"pure message is: {pure_messsage}")
            # print(f"message tuple is: {message_tuple}")

            broadcast(message_tuple)
            # Must continue to avoid combining chat message with game state
            continue
        else :
            print("Invalid data received from client:", data)

    # print("Closing current player thread and socket.")
    clientSocket.close()

rgb_colors = {
    "red" : (255, 0, 0),
    "green" : (0, 255, 0),
    "blue" : (0, 0, 255),
    "yellow" : (255, 255, 0),
    "orange" : (255, 165, 0),
} 
rgb_colors_list = list(rgb_colors.values())


def main() : 
    global counter, game
    global gameHasStartedYet

    while True:
        conn, addr = s.accept()
        print("Connected to:", addr)

        global clients
        # clients.append(conn)
        unique_id = str(uuid.uuid4())
        color = rgb_colors_list[np.random.randint(0, len(rgb_colors_list))]
        game.add_player(unique_id, color = color)

        print("Starting new thread!")

        if (gameHasStartedYet == False):
            start_new_thread(game_thread, ())
            gameHasStartedYet = True
        start_new_thread(player_thread, (conn, unique_id))
        

if __name__ == "__main__" : 
    main()