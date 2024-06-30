import socket
import pygame
import sys

client_socket = None
snakeBodyLocationTuples = []
fruitsListLocationTuples = []

def redrawWindow(surface):
    surface.fill((0,0,0))  # Fills the screen with black
    drawGrid(width, rows, surface)

    
    # draw snake
    for snakeBodyLocation in snakeBodyLocationTuples:
        snakeX = snakeBodyLocation[0]
        snakeY = snakeBodyLocation[1]
        pygame.draw.rect(surface, (255, 0, 0), (snakeX * sizeBtwn, snakeY * sizeBtwn, sizeBtwn, sizeBtwn))

    for fruitLocation in fruitsListLocationTuples:
        fruitX = fruitLocation[0]
        fruitY = fruitLocation[1]
        pygame.draw.rect(surface, (0, 255, 0), (fruitX * sizeBtwn, fruitY * sizeBtwn, sizeBtwn, sizeBtwn))

    snakeEyeX = snakeBodyLocationTuples[0][0]
    snakeEyeY = snakeBodyLocationTuples[0][1]

    eyeSize = sizeBtwn/8
    eyeOffset = 8
    eyecolor = (0, 0, 0)
    pygame.draw.circle(surface, eyecolor, (snakeEyeX * sizeBtwn + eyeOffset, snakeEyeY * sizeBtwn + eyeOffset), eyeSize)
    pygame.draw.circle(surface, eyecolor, ((snakeEyeX + 1) * sizeBtwn - eyeOffset, snakeEyeY * sizeBtwn + eyeOffset), eyeSize)

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

def sendInput(events):
    for event in events:
        if event.type == pygame.QUIT:
            client_socket.send("quit".encode())
            pygame.quit()
            return "quit"
    
        keys = pygame.key.get_pressed()

        if (keys[pygame.K_UP]):
            client_socket.send("up".encode())
            return
        elif (keys[pygame.K_DOWN]):
            client_socket.send("down".encode())
            return
        elif (keys[pygame.K_LEFT]):
            client_socket.send("left".encode())
            return
        elif (keys[pygame.K_RIGHT]):
            client_socket.send("right".encode())
            return
        elif (keys[pygame.K_r]):
            client_socket.send("reset".encode())
            return

    client_socket.send("get".encode())

def run():
    pygame.init()
    
    client_socket.send("get".encode())

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
        try: 
            events = pygame.event.get()
            message = client_socket.recv(500).decode()
            # The game state contains the position of the snake (the locations of all its body cubes) and the locations of the snacks.
            # print(f"Message received is: {message}")

            snakeBodyList = message.split("|")[0].split("*")
            fruitsList = message.split("|")[1].split("**") # in the form "(9, 5)"

            global snakeBodyLocationTuples
            snakeBodyLocationTuples = [] # list of tuples in the form (x,y)
            global fruitsListLocationTuples
            fruitsListLocationTuples = []
            
            # print(snakeBodyList)
            # print(fruitsList)

            for snakeBodyPart in snakeBodyList:
                oneBodyPart = eval(snakeBodyPart)
                snakeBodyLocationTuples.append(oneBodyPart)

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
        except Exception as error:
            print("The following error occurred in main:", error)
            sys.exit()

def main():
    server_addr = "localhost"
    server_port = 5555

    global client_socket
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM) #TCP
    client_socket.connect((server_addr, server_port))

    run()

if __name__ == "__main__" : 
    main()