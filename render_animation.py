#!/bin/python3
import sys
import time
import vedo
from vedo import *
import numpy

on_led = Cube(side=0.25).c('blue')
off_led = Cube(side=0.25,alpha=0.25).c('black')

plt = Plotter(title='LED Matrix')

#stream = open("anim_rain", "r")
stream = sys.stdin
last_frame = time.time()

def next_frame():
    #print("Next frame called")
    character_data = ''
    while stream.readable():
        line = stream.readline()
        if line.strip().isdigit():
            sleep_duration = int(line.strip())

            elapsed_time = time.time() - last_frame
            if elapsed_time < sleep_duration / 1000.0:
                time.sleep((sleep_duration / 1000.0) - elapsed_time)
            #print("Sleep for " + str(sleep_duration) + "ms")
            #time.sleep(sleep_duration / 1000.0)
            #print("Sleep done")
        else:
            character_data += line.strip("\r\n")

        if len(character_data) > 215:
            character_data = character_data[:216]
            break
    
    if len(character_data) < 216:
        return False

    pattern = '# '
    plotter = plt

    boolean_values = []
    boolean_values.extend([c == '#' for c in character_data])
    
    boolean_values += [False] * (216 - len(boolean_values))

    for x in range(6):
        for y in range(6):
            for z in range(6):
                value = boolean_values[(z * 36) + (y * 6) + x]
                mesh = off_led
                if value:
                    mesh = on_led
                plotter += mesh.clone().pos(x, y, z)
    
    return True

def loop_func(evt):
    plt.clear()
    if not next_frame():
        print("End")
        plt.timer_callback("stop")
        plt.close()
        exit(0)
        return False
    plt.render()
    return True

plt.add_callback("timer", loop_func)
plt.timer_callback("start", dt=0)
#plt.add_callback("timer", loop_func)
plt.show(rate=-1,viewup=[0,0,-1],interactive=True).close()

#plt.show(interactive=False,viewup=[0,0,-1])
#while loop_func(None): pass
#plt.interactive().close()