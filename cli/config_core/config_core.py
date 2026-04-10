from pathlib import Path
import subprocess

# Get root directory of the project
ROOT = Path(__file__).parent.parent.parent.resolve()
# Path to the arduino-cli executable
path_arduino_exe = ROOT/ "bin"/ "arduino-cli.exe"
# Path to the arduino-cli configuration file
path_config_file = ROOT/ "workspace"/ "arduino"/ "arduino-cli.yaml"

def config_core_cmd():
    """Config the arduino-cli for the project."""
    create_project_path()
    install_core_version()
    install_libraries()


def create_project_path():
    """Config the project path."""
    # Create the configuration file if it does not exist
    if not path_config_file.exists():
        path_config_file.parent.mkdir(parents=True, exist_ok=True)
        path_config_file.touch()

    print("Config parameters:")
    set_configs = [
        ["board_manager.additional_urls", "http://arduino.esp8266.com/stable/package_esp8266com_index.json"],
        ["directories.builtin.libraries", f"{ROOT}\\workspace\\arduino\\libraries"],
        ["directories.data", f"{ROOT}\\workspace\\arduino"],
        ["directories.downloads", f"{ROOT}\\workspace\\arduino\\staging"],
        ["directories.user", f"{ROOT}\\arduino"],
    ]
   
    for config in set_configs:
        command = [path_arduino_exe, 
                    "config",
                    "--config-file", path_config_file,
                    "set",
                    config[0],
                    config[1]
                    ]
        result = subprocess.run(command, shell=True, 
                            stdout = subprocess.PIPE,
                            text=True, 
                            check=True,
                            encoding="utf-8")
        if result.returncode == 0:
            print(f"Setting {config[0]} to {config[1]}\n{result.stdout}")
        else:
            print(f"Result stdout:\n{result.stdout}")
            print("Config failed")
            print(result.stderr)
            break
    
  
def install_core_version():
    """Install the core version for the project."""
    # Define the last version of the esp8266 core
    board_core = "esp8266:esp8266"
    # Command to install the core
    command = [path_arduino_exe,
               "core",
               "--config-file", path_config_file,
                "install",
                board_core
                ]
    result = subprocess.run(command, shell=True, 
                            stdout = subprocess.PIPE,
                            text=True,
                            check=True, 
                            encoding="utf-8")
    print(f"Stdout:\n{result.stdout}")
    if result.returncode == 0:
        print(f"Succesfully installed core for {board_core}\n")
    else:
        print(f"Core installation failed for {board_core}")
        print(result.stderr)  

def install_libraries():
    """Install the libraries for the project."""
    # Command to install the libraries
    libraries = ["ESPAsyncWebServer",
                 "ESPasycnTCP", 
                 "ArduinoJson",
                 "HX711 Arduino Library",
                 "RTClib"]
    for lib in libraries:
        command = [path_arduino_exe,
                   "lib",
                   "--config-file", path_config_file,
                    "install",
                    lib
                    ]
        result = subprocess.run(command, shell=True, 
                                stdout = subprocess.PIPE,
                                text=True,
                                check=True, 
                                encoding="utf-8")
        print(f"Stdout:\n{result.stdout}")
        if result.returncode == 0:
            print(f"Succesfully installed library {lib}\n")
        else:
            print(f"Library installation failed for {lib}")
            print(result.stderr)  