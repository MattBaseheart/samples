import time
import board
import digitalio
from adafruit_motor import stepper
from adafruit_motorkit import MotorKit
kit = MotorKit(i2c=board.I2C())
#Connect to M1 and M2 terminals

def move(x_dest, X_CURR):
    #this is assuming x_dest and y_dest are the coordinates of the center of a square if you are moving to a square
    if X_CURR >x_dest:
        customStep("x", "FORWARDS", X_CURR-x_dest)
        X_CURR = x_dest
    else:
        customStep("x", "BACKWARDS", x_dest-X_CURR)
        X_CURR = x_dest

    return X_CURR

def customStep(motor, direction, dist):
    DELAY = 0.01

    #this can be scaled to perform the standard steps multiplied by a larger constant
    if (motor == "x"):
        if(direction == "FORWARDS"):
            for step in range(dist):
                kit.stepper1.onestep(direction=stepper.FORWARD)
                time.sleep(DELAY)
        else:
            for step in range(dist):
                kit.stepper1.onestep(direction=stepper.BACKWARD)
                time.sleep(DELAY)

if __name__ == '__main__':
    X_CURR = 0
    for i in range(1,5):
        x_n = int(input("Set position (enter 0 to end control) to: "))
        X_CURR = move(x_n, X_CURR)
        kit.stepper1.release()
