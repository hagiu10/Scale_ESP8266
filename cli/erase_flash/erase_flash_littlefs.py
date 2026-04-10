
from glob import glob
import subprocess
from pathlib import Path

from cli.tools import get_com_port_ESP8266


def erase_flash_littlefs():
    """ Flash the littleFS image onto the ESP8266."""
    # Get root directory of the project
    ROOT = Path(__file__).parent.parent.parent.resolve()
    # Path to the esptool.py executable

    matches = glob(str(ROOT / "workspace/arduino/packages/esp8266/hardware/esp8266/*/tools/esptool/esptool.py"))

    if not matches:
        raise RuntimeError("esptool.py not found in the expected directory.")

    path_esp_tool_py = Path(matches[-1])
    start_address = "0x100000"  # Starting address of the flash region to erase
    size = "0x300000"  # Size of the flash region to erase (

    com_port = get_com_port_ESP8266.get_com_port_ESP8266()
    
    command = ["python", path_esp_tool_py,
                "--port", com_port,
                "--baud", "115200",
                "erase_region", start_address, size
               ]
    result = subprocess.run(command, shell=True, 
                            stdout = subprocess.PIPE,
                            text=True,
                            check=True, 
                            encoding="utf-8",
                            errors="replace")
    print(f"\nStdout:\n{result.stdout}")
    success_message = "Erase completed successfully"
    if success_message in result.stdout:
        print("Flash memory erased successfully.")
    else:
        print("Failed to erase flash memory.")