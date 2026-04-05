import serial.tools.list_ports as serial


def get_com_port_ESP8266():
    """Find the port of the ESP8266 based on PID and VID."""
    target_pid = 0x7523  # PID of the ESP8266
    target_vid = 0x1A86  # VID of the ESP8266

    ports = serial.comports()
    for port in ports:
        if port.vid == target_vid and port.pid == target_pid:
            return port.device

    raise Exception("ESP8266 not found. Please check the connection.")