from pathlib import Path
import subprocess
from cli.tools import get_com_port_ESP8266
from glob import glob


def read_flash_cmd():
    """Read the flash memory of the ESP8266."""
    # Get root directory of the project
    ROOT = Path(__file__).parent.parent.parent.resolve()    
    # Path to the esptool.py executable
    matches = glob(str(ROOT / "workspace/arduino/packages/esp8266/hardware/esp8266/*/tools/esptool/esptool.py"))
    if not matches:
        raise RuntimeError("esptool.py not found in the expected directory.")
    path_esp_tool_py = Path(matches[-1])
    # Path flash memory dump output file
    path_flash_dump = ROOT / "bin" / "flash_dump.bin"
    # Address to read the flash memory from
    address = "0x100000"
    # Size of the flash memory to read
    size = "0x3000" # Read 12KB of flash memory

    com_port = get_com_port_ESP8266.get_com_port_ESP8266()

    command = ["python", path_esp_tool_py,
                "--port", com_port,
                "--baud", "115200",
                "read_flash", address, size, path_flash_dump
               ]
    result = subprocess.run(command, shell=True, 
                            stdout = subprocess.PIPE,
                            text=True,
                            check=True, 
                            encoding="utf-8",
                            errors="replace")
    print(f"\nStdout:\n{result.stdout}")
    