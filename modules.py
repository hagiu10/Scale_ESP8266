import pip
import importlib.util as importlib

modules_list = [
    "pyserial>=3.5",

]


def install_modules():
    """Install required modules."""
    for module in modules_list:
        try:
            module_name = module.split('>=')[0]
            # Check if the module is already installed
            if importlib.find_spec(module_name) is not None:
                continue
            # Install the module
            pip.main(['install', module])
        except Exception as e:
            print(f"Failed to install {module}: {e}")

if __name__ == "__main__":
    install_modules()