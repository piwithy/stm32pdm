import argparse
import pathlib

import serial_capture


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
