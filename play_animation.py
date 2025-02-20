#!/bin/python3

import sys
import serial
import time
import struct
from bitarray import bitarray

stream = sys.stdin
last_frame = time.time()
sleep_duration = 0.0
packed_data = None

def run_state():
    return stream and not stream.closed and stream.readable()

def next_frame(blocking=True):
    global last_frame, sleep_duration, stream, packed_data
    character_data = ''
    while True:
        if not run_state():
            print("Run state false")
            return False

        elapsed_time = (time.time() - last_frame)
        if elapsed_time < sleep_duration:
            if not blocking:
                print("Not blocking")
                return True
            else:
                sleep_period = sleep_duration - elapsed_time
                #print("Elapsed time:", str(elapsed_time) + "s")
                #print("Sleep for:", str(sleep_period) + "s")
                time.sleep(sleep_period)
            
        line = stream.readline()
        
        if not line or len(line) == 0:
            print("No line")
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
        print("Character data too short")
        return False

    #print("Next bit frame")

    boolean_values = []
    boolean_values.extend([c == '#' for c in character_data])
    
    boolean_values += [False] * (216 - len(boolean_values))

    last_frame = time.time()

    packed_data = bitarray(boolean_values).tobytes()

    return True

def send_over_serial(serial_port='/dev/ttyACM0', baud_rate=115200):
    try:
        print("Create port")
        ser = serial.Serial(serial_port, baud_rate, timeout=0)
        time.sleep(1)
        ser.timeout = 0.1
        ser.write_timeout = 0.1
        print("Wait")
        print(ser.readall())

        print("Ready")
        while next_frame():
            ser.write(packed_data)

        ser.close()
        print(f"Data sent over serial successfully!")
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    if (sys.stdin.isatty()):
        if len(sys.argv) < 2:
            print("Usage: python send_over_serial.py <file_path>")
            sys.exit(1)
        stream = open(sys.argv[1], 'r')
    else:
        stream = sys.stdin

    serial_port = '/dev/ttyACM0'
    baud_rate = 115200

    send_over_serial(serial_port, baud_rate)

    if stream and not stream.closed:
        stream.close()

    exit(0)
