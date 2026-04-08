import subprocess
from pathlib import Path
from glob import glob

def create_littleFS_image():
    """ Create the littleFS image from the data directory."""
    # Get root directory of the project
    ROOT = Path(__file__).parent.parent.parent.resolve()
    # Path to the mklittlefs executable
    matches = glob(str(ROOT / "workspace/arduino/packages/esp8266/tools/mklittlefs/*/mklittlefs.exe"))
    if not matches:
        raise RuntimeError("mklittlefs.exe not found in the expected directory.")
    path_mklittlefs_exe = Path(matches[-1])
    # Path to the data directory
    path_data_dir = ROOT/ "arduino"/ "libraries"/ "webServer"/ "webPage"/ "data"
    # Path to the output littleFS image
    path_littlefs_image = ROOT/ "bin"/ "littleFS.bin"

    command = [path_mklittlefs_exe, 
               "-c", path_data_dir,
               "-p", "256",
               "-b", "8192",
               "-s", "1048576",
               path_littlefs_image
               ]

    result = subprocess.run(command, shell=True, 
                            stdout = subprocess.PIPE,
                            text=True,
                            check=True, 
                            encoding="utf-8",
                            errors="replace")
    print(f"\nStdout:\n{result.stdout}")


# Nex code is an around work because the mklittlefs.exe does not work on my system. 
def create_binary_files():
    """ Create binary files from the data directory."""
    # Get root directory of the project
    ROOT = Path(__file__).parent.parent.parent.resolve()
    # Path to the data directory
    path_data_dir = ROOT/ "arduino"/ "libraries"/ "webServer"/ "webPage"/ "data"
    # Path to the output littleFS image
    path_binary_dir = ROOT/ "bin" / "littleFS"
    # Create the binary directory if it does not exist
    path_binary_dir.mkdir(parents=True, exist_ok=True)

    # For each file in src directory create for each one a binary file
    for file_path in path_data_dir.glob("*"):
        if file_path.is_file():
            with open(file_path, "rb") as f:
                data = f.read()
            with open(path_binary_dir / f"{file_path.stem}.bin", "wb") as f:
                f.write(data)

