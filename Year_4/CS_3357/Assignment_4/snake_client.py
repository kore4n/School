import socket
import pygame
import sys
import rsa

client_socket = None

sizeBtwn = 0

# A list of strings in the form (255, 0, 0)-(6, 16)*(6, 16)*(6, 16)
snakes_bodies_info = []

snakeBodyLocationTuples = []
fruitsListLocationTuples = []
snakeColor = (255, 255, 255)

globalMessageFormat = "chatmessage|"

client_public_key = None
client_private_key = None
server_public_key = None

def redrawWindow(surface):
    surface.fill((0,0,0))  # Fills the screen with black
    drawGrid(width, rows, surface)

    # A list of strings in the form 
    # e.x. (255, 0, 0)-(6, 16)*(6, 16)*(6, 16)

    for snake in snakes_bodies_info:
        snakeColorString: str = snake.split("-")[0]
        snakeBodyPositionsList: list = snake.split("-")[1].split("*")
        # print(f"snake color: {snakeColorString}")
        # print(f"snakeBodyPositionsList: {snakeBodyPositionsList}")

        snakeColor = eval(snakeColorString)
        # print(f"snake color tuple: {snakeColor}")

        for snakeBodySegmentPositionString in snakeBodyPositionsList:
            snakeBodySegmentPositionTuple = eval(snakeBodySegmentPositionString)

            snakeX = snakeBodySegmentPositionTuple[0]
            snakeY = snakeBodySegmentPositionTuple[1]
            pygame.draw.rect(surface, snakeColor, (snakeX * sizeBtwn, snakeY * sizeBtwn, sizeBtwn, sizeBtwn))

        snakeEyeX = eval(snakeBodyPositionsList[0])[0]
        snakeEyeY = eval(snakeBodyPositionsList[0])[1]

        eyeSize = sizeBtwn/8
        eyeOffset = 8
        eyecolor = (0, 0, 0)
        pygame.draw.circle(surface, eyecolor, (snakeEyeX * sizeBtwn + eyeOffset, snakeEyeY * sizeBtwn + eyeOffset), eyeSize)
        pygame.draw.circle(surface, eyecolor, ((snakeEyeX + 1) * sizeBtwn - eyeOffset, snakeEyeY * sizeBtwn + eyeOffset), eyeSize)

    for fruitLocation in fruitsListLocationTuples:
        fruitX = fruitLocation[0]
        fruitY = fruitLocation[1]
        pygame.draw.rect(surface, (0, 255, 0), (fruitX * sizeBtwn, fruitY * sizeBtwn, sizeBtwn, sizeBtwn))

    # draw fruits
    pygame.display.update()

def drawGrid(w, rows, surface):
    x = 0  # Keeps track of the current x
    y = 0  # Keeps track of the current y
    for l in range(rows):  # We will draw one vertical and one horizontal line each loop
        x = x + sizeBtwn
        y = y + sizeBtwn

        pygame.draw.line(surface, (255,255,255), (x,0),(x,w))
        pygame.draw.line(surface, (255,255,255), (0,y),(w,y))

def encryptMessage(messageToEncrypt) -> bytes:
    return rsa.encrypt(messageToEncrypt.encode(), server_public_key)

def sendInput(events):
    for event in events:
        if event.type == pygame.QUIT:
            # client_socket.send("quit".encode())

            pygame.quit()
            client_socket.send(encryptMessage("quit"))
            return "quit"
    
        keys = pygame.key.get_pressed()

        if (keys[pygame.K_UP]):
            client_socket.send(encryptMessage("up"))
            return
        elif (keys[pygame.K_DOWN]):
            client_socket.send(encryptMessage("down"))
            return
        elif (keys[pygame.K_LEFT]):
            client_socket.send(encryptMessage("left"))
            return
        elif (keys[pygame.K_RIGHT]):
            client_socket.send(encryptMessage("right"))
            return
        elif (keys[pygame.K_r]):
            client_socket.send(encryptMessage("reset"))
            return
        elif (keys[pygame.K_z]):
            client_socket.send(encryptMessage(globalMessageFormat + "Congratulations!"))
            return
        elif (keys[pygame.K_x]):
            client_socket.send(encryptMessage(globalMessageFormat + "It Works!"))
            return
        elif (keys[pygame.K_c]):
            client_socket.send(encryptMessage(globalMessageFormat + "Ready?"))
            return

    # client_socket.send("get".encode())
    client_socket.send(encryptMessage("get"))

def generate_rsa_keys():
    global client_public_key
    global client_private_key

    client_public_key, client_private_key = rsa.newkeys(1024)

    # print(client_public_key)
    # print(client_private_key)

def run():
    pygame.init()

    generate_rsa_keys()

    # Client: Use server_public_key to encode.
    # Client: Use client_private_key to decode

    # Exchange 1: Send client_public_key to server
    client_socket.send(client_public_key.save_pkcs1("PEM"))


    # Exchange 2: Receive server_public_key from server
    
    global server_public_key
    server_public_key = rsa.PublicKey.load_pkcs1(client_socket.recv(1024))
    # print(f"Server public key: {server_public_key}")


    firstget = encryptMessage("get")
    client_socket.send(firstget)
    # print(f"Sending {firstget}")

    global width, rows, s
    width = 500  # Width of our screen
    height = 500  # Height of our screen
    rows = 20  # Amount of rows

    global sizeBtwn
    sizeBtwn = width // rows  # Gives us the distance between the lines

    win = pygame.display.set_mode((width, height))  # Creates our screen object

    clock = pygame.time.Clock() # creating a clock object

    # STARTING MAIN LOOP
    while True:
        # try: 
            events = pygame.event.get()
            encryptedData = client_socket.recv(4096)
            # print(message)
            message_prefix = "|chatmessage|"
            # message_separator = b'\x7c\x63\x68\x61\x74\x6d\x65\x73\x73\x61\x67\x65\x7c'
            message_separator = message_prefix.encode()
            split_message_list = encryptedData.split(message_separator)

            message = split_message_list[0].decode()

            if (len(split_message_list) > 1):
                rsaEncryptedData = split_message_list[1]
                pure_message = rsa.decrypt(rsaEncryptedData, client_private_key).decode()
                # print(f"Message received: {pure_message}")
                print(pure_message)
                sendInput(events)
                pygame.time.delay(50)  # This will delay the game so it doesn't run too quickly
                clock.tick(10)  # Will ensure our game runs at 10 FPS
                redrawWindow(win)  # This will refresh our screen  
                continue

            fruitsList = message.split("|")[1].split("**") # in the form "(9, 5)"

            # A string of tuples in the form 
            # E.x. (255, 0, 0)-(6, 16)*(6, 16)*(6, 16)
            allPlayersInfo = message.split("|")[0].split("~")

            global snakes_bodies_info
            snakes_bodies_info = []
            for currentPlayerInfo in allPlayersInfo:
                # current player info looks like
                # E.x. (255, 0, 0)-(6, 16)*(3, 17)*(4, 17)
                snakes_bodies_info.append(currentPlayerInfo)

            global snakeBodyLocationTuples
            snakeBodyLocationTuples = [] # list of tuples in the form (x,y)
            global fruitsListLocationTuples
            fruitsListLocationTuples = []
            
            # print(snakeBodyList)
            # print(fruitsList)

            for fruitTupleString in fruitsList:
                oneFruitLocationTuple = eval(fruitTupleString)
                fruitsListLocationTuples.append(oneFruitLocationTuple)

            # print(snakeBodyLocationTuples)
            # print(fruitsListLocationTuples)

            result = sendInput(events)

            if (result == "quit"):
                print("Quitting game.")
                sys.exit()
            pygame.time.delay(50)  # This will delay the game so it doesn't run too quickly
            clock.tick(10)  # Will ensure our game runs at 10 FPS
            redrawWindow(win)  # This will refresh our screen  
        # except Exception as error:
        #     print("The following error occurred in main:", error)
        #     sys.exit()

def main():
    server_addr = "localhost"
    server_port = 5555

    global client_socket
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM) #TCP
    client_socket.connect((server_addr, server_port))

    run()

if __name__ == "__main__" : 
    main()