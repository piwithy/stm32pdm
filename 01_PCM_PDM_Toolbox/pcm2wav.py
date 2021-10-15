import argparse
import numpy as np
import matplotlib.pyplot as plt
import array
import wave


def main():
    arg_parser = argparse.ArgumentParser("pcm2wav", description="Python script to convert PCM Files to WAV")
    arg_parser.add_argument('input', metavar="<Input file>", help="PCM file to convert", nargs=1)
    arg_parser.add_argument('output', metavar="<Output file>", help="WAV output file", nargs=1)
    arg_parser.add_argument('-s', '--sampling', type=int, help="Audio Sampling Frequency")

    args = arg_parser.parse_args()
    print(args)

    print("Reading \"{}\"".format(args.input[0]))
    data = np.fromfile(args.input[0], dtype='int16')
    sig_avg = np.average(data)
    print(sig_avg)
    data_og = data
    corrected = False
    if 0x700 < sig_avg < 0x900:
        print("correcting for DAC optimized sound")
        print("Correction factors: Avg = {:.2f}, G={:.2f} dB".format(sig_avg, 20 * np.log10(0x7FFF / np.max(data))))
        data = data - sig_avg
        data = np.array(data * (0x7FFF / np.max(data)), dtype="int16")
        corrected = True

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
    wav.setframerate(float(fs))
    wav.writeframes(fragments)

    if corrected:
        n = data.shape[0]
        t = np.linspace(0, n / fs, n)
        plt.figure()
        plt.plot(t, data, label="Corrected Data")
        plt.plot(t, data_og, label="Original Data")
        plt.legend()
        plt.xlabel("Time (s)")
        plt.xlim((0, np.max(t)))
        plt.ylabel("Amplitude")
        plt.show()


if __name__ == "__main__":
    exit(main())
