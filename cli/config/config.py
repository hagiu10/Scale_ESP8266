from pathlib import Path
import subprocess



def config_cmd():
    """Config the project."""
    # Get root directory of the project
    ROOT = Path(__file__).parent.parent.parent.resolve()
    # Path to the arduino-cli executable
    path_arduino_exe = ROOT/ "bin"/ "arduino-cli.exe"
    # Path to the arduino-cli configuration file
    path_config_file = ROOT/ "workspace"/ "arduino"/ "arduino-cli.yaml"
    
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
    
  
    