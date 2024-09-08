#!/bin/python3
import sys
import time
import numpy
import os
import threading
from OpenGL import *
from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *

blue_led = [0.0, 0.0, 1.0, 1.0]
black_led = [0.0, 0.0, 0.0, 0.1]

thread = threading.Thread()
renderobjs = []
run = True
wind = None

stream = sys.stdin
last_frame = time.time()
objs_lock = threading.Lock()
sleep_duration = 0.0

def run_state():
    return run and stream and not stream.closed and stream.readable()

def cube(x,y,z,color): 
    glPushMatrix()
    glTranslatef(x, y, z)
    glColor4f(color[0],color[1],color[2],color[3])
    glutSolidCube(0.25)
    glPopMatrix()

def next_frame(blocking=True):
    global last_frame, sleep_duration, run, stream
    character_data = ''
    while True:
        if not run_state():
            return False

        elapsed_time = (time.time() - last_frame)
        if elapsed_time < sleep_duration:
            if not blocking:
                return True
            else:
                sleep_period = sleep_duration - elapsed_time
                print("Elapsed time:", str(elapsed_time) + "s")
                print("Sleep for:", str(sleep_period) + "s")
                time.sleep(sleep_period)
            
        line = stream.readline()
        
        if not line:
            return False

        if line.strip().isdigit():
            sleep_dur = int(line.strip()) / 1000.0
            if sleep_dur != sleep_duration:
                sleep_duration = sleep_dur
                print("Sleep duration set to " + str(sleep_duration) + "s")
            continue
        
        character_data += line.strip("\r\n")

        if len(character_data) > 215:
            character_data = character_data[:216]
            break
    

    if len(character_data) < 216:
        return False

    print("Next frame")

    boolean_values = []
    boolean_values.extend([c == '#' for c in character_data])
    
    boolean_values += [False] * (216 - len(boolean_values))

    last_frame = time.time()

    objs_lock.acquire()

    renderobjs.clear()

    for x in range(6):
        for y in range(6):
            for z in range(6):
                value = boolean_values[(z * 36) + (y * 6) + x]
                color = black_led
                if value:
                    color = blue_led
                renderobjs.append([x, y, z, color])

    objs_lock.release()

    return True

def safe_close():
    run = False
    if stream and not stream.closed:
        print("Closing stream")
        stream.close()

    if thread.is_alive() and thread != threading.current_thread():
        print("Waiting for input to stop")
        thread.join()
    
    if wind and thread == threading.main_thread():
        print("Closing window")
        glutDestroyWindow(wind)
        glutMainLoopEvent()
        exit(0)

def view_front():
    gluPerspective(90, 1, 0.1, 100)
    z = 2.5
    x = 8
    y = 2.5
    gluLookAt(x, y, z, x-1, y, z, 0.0, 0.0, -1.0)
    

def view_ortho():
    gluPerspective(45, 1, 0.1, 100)
    z = 2.5
    x = 13
    y = 2.5
    gluLookAt(x, y, z, x-1, y, z, 0.0, 0.0, -1.0)

def set_view():
    #glViewport(0, 0, 700, 700)
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    view_front()
    glMatrixMode(GL_MODELVIEW)

def glut_close():
    safe_close()

def render_loop():
    glClearColor(1.0, 1.0, 1.0, 1.0)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    set_view()
    glLoadIdentity()
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
    glEnable(GL_BLEND)

    for obj in renderobjs:
        cube(obj[0], obj[1], obj[2], obj[3])
    
    glutSwapBuffers()

    time.sleep(0.01)

    if not run_state():
        glutLeaveMainLoop()

    return True

def stream_loop():
    print("Running stream loop")
    try:
        while next_frame():
            continue
    except Exception as e:
        print("Error: " + str(e))
    print("Stream loop done")
    safe_close()

try:
    glutInit()
    glutInitDisplayMode(GLUT_RGBA)
    glutInitWindowSize(700,700)
    glutInitWindowPosition(0,0)
    wind = glutCreateWindow(b'LED Matrix')
    glutDisplayFunc(render_loop)
    glutIdleFunc(render_loop)
    glutCloseFunc(glut_close)
    thread = threading.Thread(target=stream_loop)
    thread.start()
    glutMainLoop()


except Exception as e:
    print("Error: " + str(e))

safe_close()