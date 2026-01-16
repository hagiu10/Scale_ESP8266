from cli.build import build
from cli.config import config
from cli.install import install
from cli.flashing import flashing
import sys


def print_help():
        help_text = """
        Usage: rc.py [OPTIONS]

        Options:
          -c, --config  Configure the project
          -b, --build  Build the project
          -f, --flashing  Flash the project onto the ESP8266
          -i, --install-arduino-cli  Install the executable
          -h, --help   Show this message and exit.
          -o, --open-serial-terminal Open serial terminal with ESP8266
        """
        print(help_text)


def run():
    """Command line interface for the Scale_ESP8266 project."""
    if '--help' in sys.argv or '-h' in sys.argv:
        print_help()
    if '--build' in sys.argv or '-b' in sys.argv:
        build.build_cmd()
    if '--config' in sys.argv or '-c' in sys.argv:
        config.config_cmd()
    if '--flashing' in sys.argv or '-f' in sys.argv:
        flashing.flashing_cmd()
    if '--open-serial-terminal' in sys.argv or '-o' in sys.argv:
        flashing.open_serial_terminal_cmd()
    if '--install-arduino-cli' in sys.argv or '-i' in sys.argv:
        install.install_ard_cli()


if __name__ == "__main__":
    run()
    