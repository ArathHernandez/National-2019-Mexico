import time, sensor, image, pyb
from image import SEARCH_EX, SEARCH_DS
from pyb import Pin, Timer
# Reset sensor
sensor.reset()

# Set sensor settings
sensor.set_contrast(1)
sensor.set_gainceiling(16)
# Max resolution for template matching with SEARCH_EX is QQVGA
sensor.set_framesize(sensor.QQVGA)
# You can set windowing to reduce the search image.
#sensor.set_windowing(((640-80)//2, (480-60)//2, 80, 60))
sensor.set_pixformat(sensor.GRAYSCALE) # Configuramos escala de grises

# Load template.
# Template should be a small (eg. 32x32 pixels) grayscale image.
templateH = image.Image("/exampleH1.pgm") # Abrimos archivo H
templateS = image.Image("/exampleS1.pgm") # Abrimos archivo S
templateU = image.Image("/exampleU1.pgm") # Abrimos archivo U
tim = Timer(4, freq=1000) # Frequency in Hz
clock = time.clock()

# Run template matching
while (True):
    clock.tick()
    img = sensor.snapshot()

    # find_template(template, threshold, [roi, step, search])
    # ROI: The region of interest tuple (x, y, w, h).
    # Step: The loop step used (y+=step, x+=step) use a bigger step to make it faster.
    # Search is either image.SEARCH_EX for exhaustive search or image.SEARCH_DS for diamond search
    #
    # Note1: ROI has to be smaller than the image and bigger than the template.
    # Note2: In diamond search, step and ROI are both ignored.
    r = img.find_template(templateH, 0.70, step=4, search=SEARCH_EX) #, roi=(10, 0, 60, 60))
    p = img.find_template(templateS, 0.70, step=4, search=SEARCH_EX) #, roi=(10, 0, 60, 60))
    q = img.find_template(templateU, 0.70, step=4, search=SEARCH_EX) #, roi=(10, 0, 60, 60))
    if r:
        img.draw_rectangle(r)
        print('H')
        ch1 = tim.channel(1, Timer.PWM, pin=Pin("P7"), pulse_width_percent=100)
    elif p:
        img.draw_rectangle(p)
        print('S')
        ch2 = tim.channel(2, Timer.PWM, pin=Pin("P8"), pulse_width_percent=100)
    elif q:
        img.draw_rectangle(q)
        print('U')
        ch3 = tim.channel(3, Timer.PWM, pin=Pin("P9"), pulse_width_percent=100)
    else:
        print("No se reconoce nada")
        ch1 = tim.channel(1, Timer.PWM, pin=Pin("P7"), pulse_width_percent=0)
        ch2 = tim.channel(2, Timer.PWM, pin=Pin("P8"), pulse_width_percent=0)
        ch3 = tim.channel(3, Timer.PWM, pin=Pin("P9"), pulse_width_percent=0)

