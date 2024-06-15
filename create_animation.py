#!/bin/python3

import sys
import time
import numpy as np

frame = []
period = 50

def get_char(v):
    if v:
        return '#'
    return '-'

def set_led(x,y,z,v):
    frame[(z * 36) + (y * 6) + x] = v

def get_led(x,y,z):
    return frame[(z * 36) + (y * 6) + x]

def print_frame(periodp=period):
    line = ''.join([get_char(i) for i in frame])

    print(line)
    print(line, file=sys.stderr)
    print(periodp)

def clear_frame():
    global frame
    frame = [0] * 216

frame = [0] * 216

def make_xywave(framenum, x, y, z):
    xval = np.sin((framenum / 3) + (x * (3.14 / 6))) * 3 + 2
    yval = np.sin((framenum / 3) + (y * (3.14 / 6))) * 3 + 2
    val = (xval + yval) * 0.5

    if z > val and z < val + 1:
        set_led(x,y,z, True)

def make_standingwave(framenum, x, y, z):
    xval = np.sin(((x + 0.5) * (3.14 / 6)))
    yval = np.sin(((y + 0.5) * (3.14 / 6)))
    #amplitude = ((framenum % 40) - 20) / 5
    amplitude = np.sin(framenum * (3.14 / 40)) * 5
    xval = xval * amplitude - (amplitude * 0.5) + 2
    yval = yval * amplitude - (amplitude * 0.5) + 2
    val = (xval + yval) * 0.5

    if z > val and z < val + 1:
        set_led(x,y,z, True)

def make_fireworks(framenum, x, y, z):
    anim_length = 100
    anim_prog = framenum % anim_length
    seed = np.round(framenum / anim_length, decimals=0)
    np.random.seed(seed)
    xval = np.random.randint(0, 6)
    yval = np.random.randint(0, 6)

    if (anim_prog < 60):
        if (x == xval and y == yval and z == anim_prog / 20):
            set_led(x,y,z, True)

def supply_3d(framenum, func):
    global frame
    for x in range(6):
        for y in range(6):
            for z in range(6):
                func(framenum, x, y, z)

def make_anim(func):
    global frame
    for i in range(500):
        frame = [0] * 216
        for x in range(6):
            for y in range(6):
                for z in range(6):
                    func(i, x, y, z)
        print_frame()

#make_anim(make_fireworks)

def make_fireworks2():
    global frame
    frame = [0] * 216
    seed = int((time.clock_gettime_ns(0)) % 10000)
    #print(seed)
    np.random.seed(seed)
    start_x = np.random.randint(0, 6)
    start_y = np.random.randint(0, 6)
    end_x = np.random.randint(2, 4)
    end_y = np.random.randint(2, 4)
    #set_led(start_x, start_y, 5, True)
    #set_led(end_x, end_y, 1, True)
    #print_frame(1000)

    steps = 10
    xiter = np.linspace(start_x, end_x, steps)
    yiter = np.linspace(start_y, end_y, steps)

    for i in range(steps):
        frame = [0] * 216
        set_led(int(xiter[i]), int(yiter[i]), 5 - int(i * (4 / steps)), True)
        print_frame(50)

    for i in range(steps):
        for x in range(6):
            for y in range(6):
                for z in range(6):
                    cubed_dist = (x - xiter[i]) ** 2 + (y - yiter[i]) ** 2 + (z - (5 - i * (4 / steps))) ** 2
                    if cubed_dist < 2:
                        set_led(x, y, z, True)
        print_frame(50)


#for i in range(1000):
#    make_fireworks2()

#print()
#sys.stdout.close()