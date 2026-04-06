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

    # Path to the SPIFFS image
    path_spiffs_image = ROOT/ "bin"/ "littleFS.bin"
    # Address to flash the SPIFFS image on the ESP8266
    address = "0x106000"

    create_littleFS_image() # Create the littleFS image before flashing it

    com_port = get_com_port_ESP8266()
    
    command = ["python", path_esp_tool_py,
                "--port", com_port,
                "--baud", "115200",
                "write_flash", address, 
                path_spiffs_image
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
        print("Flashing SPIFFS files successful!")
    else:
        print("Flashing SPIFFS files failed!")
        print(result.stderr)


