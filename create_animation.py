#!/bin/python3

import sys
import time
import numpy as np

frame = []
period = 50

def get_char(v=False):
    if v:
        return '#'
    return '-'

def set_led(x:int,y:int,z:int,v=bool):
    global frame
    frame[(z * 36) + (y * 6) + x] = v

def get_led(x:int,y:int,z:int):
    global frame
    return frame[(z * 36) + (y * 6) + x]

def print_frame(periodp=period):
    global frame
    #line = ''.join([get_char(i) for i in frame])
    line = ''.join(get_char(x) for x in frame)

    print(line)
    #print(line, file=sys.stderr)
    print(periodp)
    sys.stdout.flush()

def clear_frame(ch=False):
    global frame
    frame = [ch] * 216

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

def square(x:float, y:float, z:float):
    return (x * x) + (y * y) + (z * z)


def make_fireworks2():
    global frame
    seed = int((time.clock_gettime_ns(0)) % 10000)
    #print(seed)
    np.random.seed(seed)
    start_x = np.random.randint(0, 6)
    start_y = np.random.randint(0, 6)
    end_x = np.random.randint(2, 4)
    end_y = np.random.randint(2, 4)
    end_z = 2.5
    start_z = 0.0

    print (start_x, start_y, end_x, end_y, file=sys.stderr)
    #set_led(start_x, start_y, 5, True)
    #set_led(end_x, end_y, 1, True)
    #print_frame(1000)

    steps = 20
    per = 1000/60
    xiter = np.linspace(start_x, end_x, steps)
    yiter = np.linspace(start_y, end_y, steps)

    height = 5

    clear_frame()

    #Trail .4s
    for i in range(steps):
        clear_frame()
        set_led(int(xiter[i]), int(yiter[i]), int(height - (i * (height / steps))), True)
        print_frame(int(400 / steps))

    #Fall to center .7s
    for i in range(steps):
        clear_frame()
        z = int(end_z - ((steps - i) * (end_z / steps)))
        #print(z, file=sys.stderr)
        set_led(end_x, end_y, z, True)
        print_frame(int(700 / steps))


    #Explosion 1s
    explosion = 60
    explosion_t = 1000
    radius = np.random.randint(6, 10) #radius
    print("Radius:", radius, file=sys.stderr)
    cubed_dist = 0.0
    for i in range(explosion):
        clear_frame()
        e_i = i / explosion
        #Expand
        for x in range(0,6):
            for y in range(0,6):
                for z in range(0,6):
                    z_fall = 1 - (e_i * 1)
                    cubed_dist = square(x - end_x, y - end_y, z - end_z + z_fall)
                    if cubed_dist <= ((radius * 0.7) + (e_i * radius * 0.3)):
                        set_led(int(x), int(y), int(z), True)
        print_frame(int(explosion_t / explosion))

    #Dwell .3s
    print_frame(300)

    #Flicker 40 times in 1.5s
    flicker = 40
    flicker_t = 1500
    decay_f = 1.5
    for i in range(flicker):
        clear_frame()
        n_i = i / flicker
        #Decay
        for x in range(0,6):
            for y in range(0,6):
                for z in range(0,6):
                    z_fall = 0 - (n_i * 2)
                    cubed_dist = square(x - end_x, y - end_y, z - end_z + z_fall)
                    if cubed_dist <= radius:
                        set_led(int(x), int(y), int(z), np.random.randint(0, i + flicker) > n_i * flicker * decay_f)
        print_frame(int(flicker_t / flicker))

    print_frame(100)

    #Fall
    for f in range(7):
        for _i in range(len(frame)):
            i = len(frame) - _i - 37
            if frame[i] and i > 0:
                frame[i] = False
                frame[i + 36] = True

        print_frame(100)
            


clear_frame(False)

#clear_frame(True)

#while True:
    #print_frame(4000)
    #make_anim(make_fireworks)
#make_anim(make_standingwave)
#print_frame(1000)
#print_frame(1000)
#print_frame(1000)
#print_frame(1000)
while True:
    #for x in range(6):
    #    for y in range(6):
    #        set_led(x, 0, y, True)
    #print_frame(1000)
    make_fireworks2()
    sys.stdout.flush()

#for i in range(1000):
#    make_fireworks2()

#print()
sys.stdout.close()