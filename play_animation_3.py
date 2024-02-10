import sys
import serial
import time
import struct
from bitarray import bitarray

def send_over_serial(file_path, serial_port='/dev/ttyACM0', baud_rate=115200):
    try:
        # Initialize the serial connection
        ser = serial.Serial(serial_port, baud_rate, timeout=1)
        time.sleep(1)

        # Open the text file
        with open(file_path, 'r') as file:
            remaining_lines = file.readlines()

            while remaining_lines:
                # Find the sleep duration (first line with a whole number)
                sleep_duration_line = ''
                if remaining_lines[0].strip().isdigit():
                    sleep_duration_line = remaining_lines[0].strip()

                if sleep_duration_line != '':
                    sleep_duration = int(sleep_duration_line.strip())
                    print(f"Sleeping for {sleep_duration} milliseconds...")
                    time.sleep(sleep_duration / 1000.0)
                    remaining_lines = remaining_lines[1:]

                # Discard lines that were read for sleep duration and lines containing '#'
                
                character_data = ''
                line_count = 0
                for line in remaining_lines:
                    if not line.strip().isdigit():
                        character_data += line.strip("\r\n")
                        line_count += 1
                    else:
                        break
                
                #print(character_data)

                remaining_lines = remaining_lines[line_count:]

                if len(character_data) > 215:
                    character_data = character_data[:216]

                pattern = '# '

                # Create the array with the repeating pattern
                #character_data = pattern * 120
                print(character_data)

                # Process consecutive lines, treating '#' as True and other characters as False
                boolean_values = []
                boolean_values.extend([c == '#' for c in character_data])

                #print(boolean_values)

                # Pad the boolean values to fill a 216-byte buffer
                boolean_values += [False] * (216 - len(boolean_values))

                # Pack boolean values into bytes
                packed_data = bitarray(boolean_values).tobytes()

                # Send the packed data over serial
                ser.write(packed_data)

        # Close the serial connection
        ser.close()
        print(f"Data sent over serial successfully!")

    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    # Check if a file path is provided as a command-line argument
    if len(sys.argv) != 2:
        print("Usage: python send_over_serial.py <file_path>")
        sys.exit(1)

    # Get the file path from the command-line argument
    file_path = sys.argv[1]

    # Specify the serial port and baud rate (modify as needed)
    serial_port = '/dev/ttyACM0'
    baud_rate = 115200

    # Send the file content over serial in a loop
    send_over_serial(file_path, serial_port, baud_rate)
