from cli.build_prog import build_prog
from cli.config_core import config_core
from cli.tools import install_arduino_cli
from cli.flash_prog import flash_prog
from cli.tools import copyHtmlToProgMem
from cli.flash_littleFS import flash_littleFS
from cli.read_flash import read_flash
from cli.tools import open_terminal
from cli.erase_flash import erase_flash_littlefs
import sys


def print_help():
        help_text = """
        Usage: rc.py [OPTIONS]

        Options:
          h, help                        Show this help message and exit
          b, build_program               Build the project
          c, config_core                Configure the arduino-cli for the project
          f, flashing                   Flash the project onto the ESP8266
          o, open-serial-terminal        Open serial terminal with ESP8266
          i, install-arduino-cli          Install the arduino-cli executable
          cp, copy-html-to-progmem       Copy the HTML file to a C++ header file with PROGMEM format
          ffs, flash-littlefs           Flash the littleFS image onto the ESP8266
        """
        print(help_text)

PATH_HTML_FILE = 'arduino/libraries/webServer/utils/webPage.html'
PATH_OUTPUT_FILE = 'arduino/libraries/webServer/utils/webPage.h'

def run():
    """Command line interface for the Scale_ESP8266 project."""
    if 'help' in sys.argv or 'h' in sys.argv:
        print_help()
    if 'build_program' in sys.argv or 'b' in sys.argv:
        build_prog.build_prog_cmd()
    if 'config_core' in sys.argv or 'c' in sys.argv:
        config_core.config_core_cmd()
    if 'flash_program' in sys.argv or 'f' in sys.argv:
        flash_prog.flash_prog_cmd()
    if 'open-serial-terminal' in sys.argv or 'o' in sys.argv:
        open_terminal.open_serial_terminal_cmd()
    if 'install-arduino-cli' in sys.argv or 'i' in sys.argv:
        install_arduino_cli.install_arduino_cli()
    if 'copy-html-to-progmem' in sys.argv or 'cp' in sys.argv: 
        copyHtmlToProgMem.copy_html_to_progmem(PATH_HTML_FILE, PATH_OUTPUT_FILE) 
    if 'flash-littlefs' in sys.argv or 'ffs' in sys.argv:
        flash_littleFS.flash_littleFS_cmd()
    if 'read_flash' in sys.argv or 'rf' in sys.argv:
        read_flash.read_flash_cmd()
    if 'erase-flash-littlefs' in sys.argv or 'efl' in sys.argv:
        erase_flash_littlefs.erase_flash_littlefs()

if __name__ == "__main__":
    run()
    