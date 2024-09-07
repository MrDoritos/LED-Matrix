#!/bin/python3

import sys
import serial
import time
import struct
from bitarray import bitarray

def send_over_serial(stream, serial_port='/dev/ttyACM0', baud_rate=115200):
    try:
        ser = serial.Serial(serial_port, baud_rate, timeout=1)
        time.sleep(1)

        current_line = stream.readline()

        while stream.readable():
            sleep_duration_line = ''

            if current_line.strip().isdigit():
                sleep_duration_line = current_line.strip()
                current_line = stream.readline()

            if sleep_duration_line != '':
                sleep_duration = int(sleep_duration_line.strip())
                print(f"Sleeping for {sleep_duration} milliseconds...")
                time.sleep(sleep_duration / 1000.0)

            character_data = ''
            line_count = 0
            while stream.readable():
                if current_line.strip().isdigit():
                    break
                character_data += current_line.strip("\r\n")
                current_line = stream.readline()

            if len(character_data) > 215:
                character_data = character_data[:216]

            print(character_data)
            boolean_values = []
            boolean_values.extend([c == '#' for c in character_data])
            boolean_values += [False] * (216 - len(boolean_values))
            packed_data = bitarray(boolean_values).tobytes()
            ser.write(packed_data)

        ser.close()
        print(f"Data sent over serial successfully!")
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    stream = None
    if (sys.stdin.isatty()):
        if len(sys.argv) < 2:
            print("Usage: python send_over_serial.py <file_path>")
            sys.exit(1)
        stream = open(sys.argv[1], 'r')
    else:
        stream = sys.stdin


    serial_port = '/dev/ttyACM0'
    baud_rate = 115200

    send_over_serial(stream, serial_port, baud_rate)
