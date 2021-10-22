import argparse
import pathlib

import serial
import time


def serial_capture(com_port: str, speed: int, timeout_s: float):
    """
    Function to capture Data from the serial Port

    Parameters
    ----------
    com_port : str
        Serial Port to listen
    speed: int
        Serial Communication Speed in Bauds
    timeout_s: float
        Time in seconds after last byte received to end the communication

    Returns
    -------
    list
        list of the bytes received
    """
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


def main():
    argument_parser = argparse.ArgumentParser("raw_capture.py", description="Python Script to capture RAW data From Serial Port")
    argument_parser.add_argument("output", metavar="<OUTPUT FILE>",
                                 help="File where the captured data will be stored", nargs=1)
    argument_parser.add_argument("serial_port", metavar="<COM PORT>", help="COM PORT to capture from", nargs=1)
    argument_parser.add_argument("-b", metavar="baud", type=int,
                                 help="Serial Port Speed in Baud (defaults @115200 Baud)", required=False)
    args = argument_parser.parse_args()

    pcm_bytes = serial_capture.serial_capture(args.serial_port[0], args.b, .5)
    output_path = pathlib.Path(args.output[0])
    print("Writing to \"{}\"".format(output_path))
    fp = open(output_path, "wb")
    wr_bytes = 0
    for byt in pcm_bytes:
        fp.write(byt)
        wr_bytes += 1
    fp.close()
    print("Wrote {} byte{} to \"{}\"".format(wr_bytes, "s" if wr_bytes > 1 else "", output_path))


if __name__ == "__main__":
    exit(main())
