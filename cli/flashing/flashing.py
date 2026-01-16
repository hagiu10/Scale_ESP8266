from pathlib import Path
import subprocess
import serial as pyserial
import serial.tools.list_ports as serial


def flashing_cmd():
    """ Flash the project onto the ESP8266."""
    # Get root directory of the project
    ROOT = Path(__file__).parent.parent.parent.resolve()
    # Path to the arduino-cli executable
    path_arduino_exe = ROOT/ "bin"/ "arduino-cli.exe"
    # Path to the main arduino sketch
    path_arduini_sketch = ROOT/ "arduino"/ "main"/ "main.ino"
    # Define the core to build for
    board_core = "esp8266:esp8266"
    # Command to build the project
    com_port = get_com_port_ESp8266()
    # Path to the configuration file
    config_file = ROOT / "workspace" / "arduino" / "arduino-cli.yaml"
    command = [path_arduino_exe, 
               "upload",
               "-p", com_port,
               "--fqbn",f"{board_core}:generic", 
               path_arduini_sketch,
               "--config-file",config_file
               ]

    result = subprocess.run(command, shell=True, 
                            stdout = subprocess.PIPE,
                            text=True,
                            check=True, 
                            encoding="utf-8",
                            errors="replace")
    print(f"\nStdout:\n{result.stdout}")
    success_string = "Hash of data verified."
    if  success_string in result.stdout:
        print("Flashing successful")
    else:
        print("Flashing failed")
        print(result.stderr)


def get_com_port_ESp8266():
    """Find the port of the ESP8266 based on PID and VID."""
    target_pid = 0x7523  # PID of the ESP8266
    target_vid = 0x1A86  # VID of the ESP8266

    ports = serial.comports()
    for port in ports:
        if port.vid == target_vid and port.pid == target_pid:
            return port.device

    raise Exception("ESP8266 not found. Please check the connection.")


def open_serial_terminal_cmd():
    """Open a serial terminal with the ESP8266."""
    com_port = get_com_port_ESp8266()
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