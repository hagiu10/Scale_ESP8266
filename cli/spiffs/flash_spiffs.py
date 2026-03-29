from pathlib import Path
import subprocess
import serial.tools.list_ports as serial


def flash_spiffs_cmd():
    """ Flash the SPIFFS image onto the ESP8266."""
    # Get root directory of the project
    ROOT = Path(__file__).parent.parent.parent.resolve()
    # Path to the esptool.py executable
    path_esp_tool_py = ROOT/ "workspace" / "arduino" / "packages" / "esp8266" / "hardware" / "esp8266" / "3.1.2" / "tools" / "esptool" / "esptool.py"
    # Path to the SPIFFS image
    path_spiffs_image = ROOT/ "arduino"/ "main" / "data" / "spiffs.bin"
    
    create_spiffs_image_cmd() # Create the SPIFFS image before flashing it

    com_port = get_com_port_ESp8266()
    
    command = ["python", path_esp_tool_py,
                "--port", com_port,
                "--baud", "115200",
                "write_flash", "0x300000", path_spiffs_image
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

def create_spiffs_image_cmd():
    """ Create the SPIFFS image from the data directory."""
    # Get root directory of the project
    ROOT = Path(__file__).parent.parent.parent.resolve()
    # Path to the mkspiffs executable
    path_mkspiffs_exe = ROOT/ "workspace" / "arduino" / "packages" / "esp8266" / "tools" / "mkspiffs" / "3.1.0-gcc10.3-e5f9fec" / "mkspiffs.exe"
    # Path to the data directory
    path_data_dir = ROOT/ "arduino"/ "main" / "data"
    # Path to the output SPIFFS image
    path_spiffs_image = ROOT/ "arduino"/ "main" / "data" / "spiffs.bin"
    
    command = [path_mkspiffs_exe, 
               "-c", path_data_dir,
               "-p", "256",
               "-b", "4096",
               "-s", "1048576",
               path_spiffs_image
               ]

    result = subprocess.run(command, shell=True, 
                            stdout = subprocess.PIPE,
                            text=True,
                            check=True, 
                            encoding="utf-8",
                            errors="replace")
    print(f"\nStdout:\n{result.stdout}")

def get_com_port_ESp8266():
    """Find the port of the ESP8266 based on PID and VID."""
    target_pid = 0x7523  # PID of the ESP8266
    target_vid = 0x1A86  # VID of the ESP8266

    ports = serial.comports()
    for port in ports:
        if port.vid == target_vid and port.pid == target_pid:
            return port.device

    raise Exception("ESP8266 not found. Please check the connection.")
