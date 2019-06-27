# PWM Control Example
#
# This example shows how to do PWM with your OpenMV Cam.

import time
from pyb import Pin, Timer
import pyb


tim = Timer(4, freq=1000) # Frequency in Hz
# Generate a 1KHz square wave on TIM4 with 50% and 75% duty cycles on channels 1 and 2, respectively.


while (True):

    ch1 = tim.channel(1, Timer.PWM, pin=Pin("P7"), pulse_width_percent=100)
    ch2 = tim.channel(2, Timer.PWM, pin=Pin("P8"), pulse_width_percent=0)
    ch3 = tim.channel(3, Timer.PWM, pin=Pin("P9"), pulse_width_percent=0)
    pyb.delay(200)

    ch1 = tim.channel(1, Timer.PWM, pin=Pin("P7"), pulse_width_percent=0)
    ch2 = tim.channel(2, Timer.PWM, pin=Pin("P8"), pulse_width_percent=100)
    ch3 = tim.channel(3, Timer.PWM, pin=Pin("P9"), pulse_width_percent=0)
    pyb.delay(200)

    ch1 = tim.channel(1, Timer.PWM, pin=Pin("P7"), pulse_width_percent=0)
    ch2 = tim.channel(2, Timer.PWM, pin=Pin("P8"), pulse_width_percent=0)
    ch3 = tim.channel(3, Timer.PWM, pin=Pin("P9"), pulse_width_percent=100)
    pyb.delay(200)

    ch1 = tim.channel(1, Timer.PWM, pin=Pin("P7"), pulse_width_percent=0)
    ch2 = tim.channel(2, Timer.PWM, pin=Pin("P8"), pulse_width_percent=100)
    ch3 = tim.channel(3, Timer.PWM, pin=Pin("P9"), pulse_width_percent=0)
    pyb.delay(200)

    ch1 = tim.channel(1, Timer.PWM, pin=Pin("P7"), pulse_width_percent=100)
    ch2 = tim.channel(2, Timer.PWM, pin=Pin("P8"), pulse_width_percent=0)
    ch3 = tim.channel(3, Timer.PWM, pin=Pin("P9"), pulse_width_percent=0)
    pyb.delay(200)
