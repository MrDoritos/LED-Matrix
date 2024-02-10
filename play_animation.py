import sys
import serial
import time

def send_over_serial(file_path, serial_port='/dev/ttyACM0', baud_rate=9600):
    try:
        # Open the text file

        ser = serial.Serial(serial_port, baud_rate, timeout=1, rtscts=True)
        time.sleep(5)

        with open(file_path, 'r') as file:
            lines = file.readlines()

            # Find the sleep duration (first line with a whole number)
            #sleep_duration = None
            #for line in lines:
            #    try:
            #        sleep_duration = int(line.strip())
            #        break
            #    except ValueError:
            #        pass


            #if sleep_duration is not None:
            #    print(f"Sleeping for {sleep_duration} milliseconds...")
            #    time.sleep(sleep_duration / 1000.0)

            # Process consecutive lines, treating '#' as True and other characters as False
            boolean_values = []
            for line in lines:
                boolean_values.extend([c == '#' for c in line.strip()])

            print(boolean_values)
            # Pad the boolean values to fill a 256-byte buffer
            boolean_values += [False] * (256 - len(boolean_values))

            # Convert boolean values to bytes
            boolean_bytes = bytes(boolean_values)

            print(boolean_bytes)

            # Initialize the serial connection

            #ser.write('#####'.encode())
            # Send the boolean values over serial
            ser.write(boolean_bytes)
            #ser.write('\r\n'.encode())
            #ser.flush()

            time.sleep(5)
            # Close the serial connection

        print(f"Data sent over serial successfully!")
        ser.close()

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
    baud_rate = 9600

    # Send the file content over serial
    send_over_serial(file_path, serial_port, baud_rate)
