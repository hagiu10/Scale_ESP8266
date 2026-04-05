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
    path_data_dir = ROOT/ "arduino"/ "libraries"/ "webServer"/ "webPage"/ "src"
    # Path to the output littleFS image
    path_littlefs_image = ROOT/ "bin"/ "littleFS.bin"

    command = [path_mklittlefs_exe, 
               "-c", path_data_dir,
               "-p", "256",
               "-b", "4096",
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