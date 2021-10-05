import argparse
import numpy as np
import array
import wave


def main():
    arg_parser = argparse.ArgumentParser("pcm2wav", description="Python script to convert PCM Files to WAV")
    arg_parser.add_argument('input', metavar="<Input file>", help="PCM file to convert", nargs=1)
    arg_parser.add_argument('output', metavar="<Output file>", help="WAV output file", nargs=1)
    arg_parser.add_argument('-s', '--sampling', help="Audio Sampling Frequency")

    args = arg_parser.parse_args()

    print("Reading \"{}\"".format(args.input[0]))
    data = np.fromfile(args.input[0], dtype='int16')
    fragments = array.array('h', data).tobytes()

    bytes_per_sample = 2
    if args.sampling:
        fs = args.sampling
    else:
        fs = 32000

    print("Writing \"{}\"".format(args.output[0]))
    wav = wave.open(args.output[0], 'wb')
    wav.setnchannels(1)
    wav.setsampwidth(bytes_per_sample)
    wav.setframerate(fs)
    wav.writeframes(fragments)


if __name__ == "__main__":
    exit(main())
