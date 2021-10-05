import serial
import time


def serial_capture(com_port: str, speed: int, timeout_s: float):
    input_buffer = []
    ser = serial.Serial(com_port, baudrate=speed if speed else 115200)
    try:
        last_read = time.time()
        read_started = False
        print("Listening on \"{}\" @ {} Bauds ".format(com_port, speed))
        while True:
            if ser.in_waiting > 0:
                read_started = True
                last_read = time.time()
                input_buffer.append(ser.read(1))
            elif read_started and time.time() - last_read > timeout_s:
                print("Received {} byte{}".format(len(input_buffer), "s" if len(input_buffer) > 1 else ""))
                ser.close()
                return input_buffer.copy()
    except KeyboardInterrupt as k_int:
        print("Keyboard Interrupt! Closing Serial Port")
        ser.close()
        raise k_int


if __name__ == "__main__":
    raise NotImplemented("Library_file")
