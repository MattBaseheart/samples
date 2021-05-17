import time
import board
import digitalio
from adafruit_motor import stepper
from adafruit_motorkit import MotorKit
kit = MotorKit(i2c=board.I2C())

#           BOARD LAYOUT
#       --------------------- x motor,  --> is forwards, <-- is backwards
#          1     2     3  
#       -------------------      |    
#       |     |     |     |   1  |   /|\                |
#       -------------------      |    |   is backwards, | is forwards
#       |     |     |     |   2  |    |                \|/
#       -------------------      |
#       |     |     |     |   3  |  y motor
#       -------------------

def move(x_dest, X_CURR, y_dest, Y_CURR, half):
    #MOVES FIRST ALONG Y AXIS
    #MOVES SECOND ALONG X AXIS
    
    #Use current and destination Y position do decide to move forward or backward
    #The half parameter determines if it is moving to the center of a square or not
    if Y_CURR >y_dest:
        customStep("y", "BACKWARDS", Y_CURR-y_dest, half)
        Y_CURR = y_dest
    else:
        customStep("y", "FORWARDS", y_dest-Y_CURR, half)
        Y_CURR = y_dest
    
    if X_CURR >x_dest:
        customStep("x", "BACKWARDS", X_CURR-x_dest, half)
        X_CURR = x_dest
    else:
        customStep("x", "FORWARDS", x_dest-X_CURR, half)
        X_CURR = x_dest

def customStep(motor, direction, dist, half):
    DELAY = 0.01
    SQUARE_LEN = 50

    #Given the number of onestep operations needed to traverse a single square, SQUARE_LEN:
    #Move dist number of squares in the provided direction.
    #The last movement will be half a square based on the parameter "half"
    
    #Xmotor movement
    if (motor == "x"):
        if(direction == "FORWARDS"):
            for step in range(dist):
                if(step == dist and half == 1):
                    for step in range(SQUARE_LEN/2):
                        kit.stepper1.onestep(direction=stepper.FORWARD)
                        time.sleep(DELAY)
                else:
                    for step in range(SQUARE_LEN):
                        kit.stepper1.onestep(direction=stepper.FORWARD)
                        time.sleep(DELAY)
        else:
            for step in range(dist):
                if(step == dist and half == 1):
                    for step in range(SQUARE_LEN/2):
                        kit.stepper1.onestep(direction=stepper.BACKWARD)
                        time.sleep(DELAY)
                else:
                    for step in range(SQUARE_LEN):
                        kit.stepper1.onestep(direction=stepper.BACKWARD)
                        time.sleep(DELAY)
    #Ymotor movement
    else:
        if(direction == "FORWARDS"):
            for step in range(dist):
                if(step == dist and half == 1):
                    for step in range(SQUARE_LEN/2):
                        kit.stepper2.onestep(direction=stepper.FORWARD)
                        time.sleep(DELAY)
                else:
                    for step in range(SQUARE_LEN):
                        kit.stepper2.onestep(direction=stepper.FORWARD)
                        time.sleep(DELAY)
        else:
            for step in range(dist):
                if(step == dist and half == 1):
                    for step in range(SQUARE_LEN/2):
                        kit.stepper2.onestep(direction=stepper.BACKWARD)
                        time.sleep(DELAY)
                else:
                    for step in range(SQUARE_LEN):
                        kit.stepper2.onestep(direction=stepper.BACKWARD)
                        time.sleep(DELAY)


if __name__ == '__main__':

    #Os are below board, Xs are on top
    #          X  X  X  X  X
    #       -------------------         
    #       |     |     |     |  
    #       -------------------   
    #       |     |     |     |  Beginning state of Board
    #       -------------------  
    #       |     |     |     |  
    #       ----MAGNET---------
    #          O  O  O  O  O

    #PLAYER: 0 is Os turn, 1 is Xs turn
    #Start game with O going first
    player = 0

    #CURRENTLY THIS FOR LOOP ONLY RUNS 5 TURNS
    #EDIT THIS TO RUN BASED ON GAME STATE
    for i in range(1,5):
        if(player == 0): #O starting point
            X_CURR = 1
            Y_CURR = 3
            #       -------------------         
            #       |     |     |     |  
            #       -------------------   
            #       |     |     |     |  Starting point when it is O's turn
            #       -------------------  Treat this coordinate like (1,3) 
            #       |     |     |     |  
            #       -----HERE---------- 
        else: #X starting point, use -1 for Y_CURR instead of 0 to make math in move() easier
            X_CURR = 1
            Y_CURR = -1
            #       -----HERE----------         
            #       |     |     |     |  
            #       -------------------   
            #       |     |     |     |  Starting point when it is X's turn
            #       -------------------  Treat this coordinate like (1,-1) 
            #       |     |     |     |  
            #       ------------------- 

        #REPLACE THIS WITH INPUT FROM API?
        x_n = int(input("Set X position to: "))
        y_n = int(input("Set Y position to: "))
        
        #HANDLE SPECIAL CASES OF X_N VALUES
        if(x_n == 1):
            X_CURR = 0
        elif(x_n == 2):
            move(1, 0, 0, 0, 0)
        
        #MOVE TO PLACEMENT LOCATION, MIDDLE OF THE SQUARE
        move(x_n, X_CURR, y_n, Y_CURR, 1)

        #LET GO OF THE PIECE
        #-----------------ADD THIS-----------------#

        #MOVE TO A KNOWN COORDINATE (-1/2, -1/2 will get us to the real x_n, y_n coordinate)
        move(0, 1, 0, 1, 1)

        #NOW, X_CURR = x_n, Y_CURR  = y_n
        X_CURR = x_n
        Y_CURR = y_n

        #STORE PLACEMENT AND DECIDE IF WIN STATE REACHED
        #-----------------ADD THIS-----------------#

        #MOVE TO THE LEFT CORNER OF THE BOARD CLOSEST TO THE NEXT PIECE TO BE PICKED UP
        #Just placed an O, X is next (top left corner)
        if(player == 0):
            move(0, X_CURR, 0, Y_CURR, 0)
            player = 1
        #Just placed an X, O is next (bottom left corner)
        else:
            move(0, X_CURR, 3, Y_CURR, 0)
            player = 0

        #MOVE TO NEW PIECE
        #-----------------ADD THIS-----------------#

        #PICK UP PIECE
        #-----------------ADD THIS-----------------#

        #MOVE TO CORRECT BEGINNING SQUARE FOR NEXT TURN
        if(player == 0):
            #Move to O starting position based on X_CURR and Y_CURR from picking up piece
            move(1, X_CURR, 3, Y_CURR, 0)
        else:
            #Move to X starting position based on X_CURR and Y_CURR from picking up piece
            move(1, X_CURR, 0, Y_CURR, 0)

        kit.stepper1.release()
        kit.stepper2.release()