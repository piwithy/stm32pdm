import argparse
import numpy as np
import matplotlib.pyplot as plt


def pdm_generator(x, qe=0.):
    y = np.zeros(x.shape, dtype="int8")
    for i in range(len(x)):
        if x[i] > qe:
            y[i] = 1
        else:
            y[i] = -1
        qe += y[i] - x[i]
    y = (y + 1)
    y = np.array(y / 2, dtype="uint8")
    return y, qe


def main():
    arg_parser = argparse.ArgumentParser("pcm2pdm", description="Python script to convert PCM to PDM")
    arg_parser.add_argument('input', metavar="<Input file>", help="PCM input file", nargs=1)
    arg_parser.add_argument('output', metavar="<Output file>", help="PDM output file", nargs=1)
    arg_parser.add_argument('-f', '--factor', type=int, help="(PDM fs / PCM fs) factor")

    args = arg_parser.parse_args()
    sampling_factor = args.factor if args.factor else 64

    # Reading PCM File
    # PCM -> 16kHz, 16 bit ints
    print("Importing PCM data from \"{}\"".format(args.input[0]))
    pcm_signal_int = np.fromfile(args.input[0], dtype="int16")
    print("PCM Length: {}".format(pcm_signal_int.shape[0]))
    pcm_max = np.max(pcm_signal_int) if np.max(pcm_signal_int) != 0 else 1
    pcm_signal_float = np.array(pcm_signal_int, dtype='float64') / pcm_max
    pcm_n = pcm_signal_int.shape[0]

    print("Oversampling PCM signal for PDM Conversion (PDM fs = {} * PCM_fs)".format(sampling_factor))
    pdm_n = pcm_n * sampling_factor
    pcm_signal_oversampled = np.zeros((pdm_n,), dtype='float64')
    for i in range(pcm_n):
        for j in range(sampling_factor):
            pcm_signal_oversampled[(i * sampling_factor) + j] = pcm_signal_float[i]

    print("Converting PCM Signal to PDM")
    (pdm_signal, pdm_qe) = pdm_generator(pcm_signal_oversampled, 0.)
    print("\tConversion Qe={}".format(pdm_qe))

    print("Writing 16bits chunks of PDM in \"{}\"".format(args.output[0]))

    pdm_signal_16bits = np.zeros((int(pdm_signal.shape[0] / 16),), dtype="uint16")
    print("PDM_length: {}".format(pdm_signal_16bits.shape[0]))
    for i in range(int(pdm_signal.shape[0] / 16)):
        val = 0
        for j in range(16):
            val += pdm_signal[i * 16 + j] * (2 ** j)
        pdm_signal_16bits[i] = val

    fp = open(args.output[0], "wb")
    for i in range(pdm_signal_16bits.shape[0]):
        fp.write(pdm_signal_16bits[i])
    fp.close()


if __name__ == '__main__':
    exit(main())
