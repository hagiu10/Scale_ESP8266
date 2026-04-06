from pathlib import Path
import subprocess
import serial.tools.list_ports as serial
from cli.tools import get_com_port_ESP8266


def flash_prog_cmd():
    """ Flash the project onto the ESP8266."""
    # Get root directory of the project
    ROOT = Path(__file__).parent.parent.parent.resolve()
    # Path to the arduino-cli executable
    path_arduino_exe = ROOT/ "bin"/ "arduino-cli.exe"
    # Define the core to build for
    board_core = "esp8266:esp8266:generic:eesz=4M3M"
    # Schech to use for flashing
    path_arduino_sketch = ROOT/ "arduino" / "main" / "main.ino"


    com_port = get_com_port_ESP8266.get_com_port_ESP8266()

    command = [path_arduino_exe, 
               "upload",
               "-p", com_port,
               "--fqbn",board_core,
                path_arduino_sketch
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


