from pathlib import Path
import subprocess



def build_prog_cmd():
    """Build the project."""
    # Get root directory of the project
    ROOT = Path(__file__).parent.parent.parent.resolve()
    # Path to the arduino-cli executable
    path_arduino_exe = ROOT/ "bin"/ "arduino-cli.exe"
    # Path to the main arduino sketch
    path_arduini_sketch = ROOT/ "arduino"/ "main"/ "main.ino"
    # Path to the arduino-cli configuration file
    path_config_file = ROOT/ "workspace"/ "arduino"/ "arduino-cli.yaml"
    
    # Define the core to build for
    board_core = "esp8266:esp8266:generic:eesz=4M3M"

    # Command to build the project
    command = [path_arduino_exe, 
               "compile",
               "--config-file", path_config_file ,
               "--fqbn",board_core, 
               path_arduini_sketch
               ]

    result = subprocess.run(command, shell=True, 
                            stdout = subprocess.PIPE,
                            text=True,
                            check=True, 
                            encoding="utf-8")
    print(f"Stdout:\n{result.stdout}")

    if result.returncode == 0:
        print("Build successful")
    else:
        print("Build failed")
        print(result.stderr)