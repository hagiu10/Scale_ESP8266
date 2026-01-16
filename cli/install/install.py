from pathlib import Path
import subprocess


def install_ard_cli():
    """Install the executable."""
     # Find bash.exe
    bash_path = find_bash_exe()
    if not bash_path:
        print("bash.exe not found in common paths.")
        return
    
    # Command to install the arduino-cli
    command = [f"{bash_path}", "-c", 
               "curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh"
               ]
   
    result = subprocess.run(command,
                            stdout = subprocess.PIPE,
                            text=True, 
                            check=True,
                            encoding="utf-8")
    
    print(f"Stdout:\n{result.stdout}")
    if result.returncode == 0:
        print("Succesfully installed arduino-cli\n")
    else:
        print("Core installation failed for arduino-cli")
        print(result.stderr)
        return
    

def find_bash_exe():
    """Find the path to bash.exe."""
    common_paths = [
        "C:\\Program Files\\Git\\bin\\bash.exe",
        "C:\\Program Files (x86)\\Git\\bin\\bash.exe",
        "C:\\Windows\\System32\\bash.exe",  # WSL
        "C:\\cygwin64\\bin\\bash.exe",
        "C:\\cygwin\\bin\\bash.exe"
    ]

    for path in common_paths:
        if Path(path).exists():
            return path
    return None
