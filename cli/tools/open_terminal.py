import serial as pyserial
from cli.tools.get_com_port_ESP8266 import get_com_port_ESP8266
  

def open_serial_terminal_cmd():
    """Open a serial terminal with the ESP8266."""
    com_port = get_com_port_ESP8266()
    baud_rate = 115200

    try:
        with pyserial.Serial(com_port, baud_rate, timeout=1) as ser:
            print(f"Connected to {com_port} at {baud_rate} baud.")
            print("Press Ctrl+C to exit.")
            while True:
                if ser.in_waiting > 0:
                    data = ser.read(ser.in_waiting).decode("utf-8", errors="replace")
                    print(data, end="")
    except KeyboardInterrupt:
        print("\nSerial terminal closed.")
    except Exception as e:
        print(f"Error: {e}")