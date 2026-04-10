from glob import glob
from pathlib import Path
import subprocess
import serial.tools.list_ports as serial
from cli.tools.create_littlleFS_image import create_littleFS_image
from cli.tools.get_com_port_ESP8266 import get_com_port_ESP8266


def flash_littleFS_cmd():
    """ Flash the littleFS image onto the ESP8266."""
    # Get root directory of the project
    ROOT = Path(__file__).parent.parent.parent.resolve()
    # Path to the esptool.py executable

    matches = glob(str(ROOT / "workspace/arduino/packages/esp8266/hardware/esp8266/*/tools/esptool/esptool.py"))

    if not matches:
        raise RuntimeError("esptool.py not found in the expected directory.")

    path_esp_tool_py = Path(matches[-1])

    # Path to the binary files created from the data directory
    path_binary_dir = ROOT/ "bin" / "littleFS"
    
    create_littleFS_image() # Create the littleFS image before flashing it
    
    com_port = get_com_port_ESP8266()

    # Write each binary file to the flash memory of the ESP8266 start at 0x300000
    start_address = 0x100000
    max_address = 0x400000

    for binary_file in path_binary_dir.glob("*.bin"):
        if start_address > max_address:
            print("Error: Not enough flash memory to write all binary files.")
            break

        size = binary_file.stat().st_size
        end_address = start_address + size

        if end_address > max_address:
            print(f"Error: Not enough flash memory to write {binary_file.name}.")
            break

        print(f"Flashing {binary_file} between address {hex(start_address)} and {hex(end_address)}")

        command = ["python", str(path_esp_tool_py),
                    "--port", com_port,
                    "--baud", "115200",
                    "write_flash", hex(start_address), str(binary_file)
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
            print("Flashing binary files successful!") 
        else:
            print("Flashing binary files failed!")
            print(result.stderr)
        start_address = start_address + size