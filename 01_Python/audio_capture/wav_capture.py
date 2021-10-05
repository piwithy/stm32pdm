import argparse
import pathlib
import wave

import numpy as np

import serial_capture


def main():
    argument_parser = argparse.ArgumentParser("pcm_capture.py",
                                              description="Python Script to capture WAV data From Serial Port")
    argument_parser.add_argument("output", metavar="<PCM OUTPUT FILE>",
                                 help="PCM File where the captured data will be stored", nargs=1)
    argument_parser.add_argument("serial_port", metavar="<COM PORT>", help="COM PORT to capture from", nargs=1)
    argument_parser.add_argument("-b", metavar="baud", type=int,
                                 help="Serial Port Speed in Baud (defaults @115200 Baud)", required=False)
    argument_parser.add_argument("-s", '--sampling', type=int, help="Sampling Frequency", required=False)

    args = argument_parser.parse_args()

    pcm_bytes = serial_capture.serial_capture(args.serial_port[0], args.b, .5)
    output_path = pathlib.Path(args.output[0])
    print("Writing to \"{}\"".format(output_path))
    # fp = open(output_path, "wb")
    pcm_16_bits = []
    wr_bytes = 0
    for i in range(0, len(pcm_bytes), 2):
        print(i)
        j_b = pcm_bytes[i] + pcm_bytes[i + 1]
        pcm_16_bits.append(int.from_bytes(j_b, 'little', signed=True))
        wr_bytes += 2
        # print("j_b={:4x}, 0x{:4x}".format(j_b, pcm_16_bits[-1]))

    if args.sampling:
        fs = args.sampling
    else:
        fs = 32000
    wav = wave.open(str(output_path.absolute()), "wb")
    wav.setnchannels(1)
    wav.setsampwidth(2)
    wav.setframerate(fs)
    wav.writeframes(np.array(pcm_16_bits, dtype="int16"))
    wav.close()
    print("Wrote {} byte{} to \"{}\"".format(wr_bytes, "s" if wr_bytes > 1 else "", output_path))


if __name__ == "__main__":
    exit(main())
