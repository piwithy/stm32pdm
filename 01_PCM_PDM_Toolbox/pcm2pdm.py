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

    args = arg_parser.parse_args()

    # Reading PCM File
    # PCM -> 16kHz, 16 bit ints
    print("Importing PCM data from \"{}\"".format(args.input[0]))
    pcm_signal_int = np.fromfile(args.input[0], dtype="int16")
    # print("PCM Length: {}".format(pcm_signal_int.shape[0]))
    pcm_max = np.max(pcm_signal_int) if np.max(pcm_signal_int) != 0 else 1
    pcm_signal_float = np.array(pcm_signal_int, dtype='float64') / pcm_max
    pcm_n = pcm_signal_int.shape[0]

    print("Converting PCM Signal to PDM")
    (pdm_signal, pdm_qe) = pdm_generator(pcm_signal_float, 0.)
    print("\tConversion Qe={}".format(pdm_qe))

    print("Writing 16bits chunks of PDM in \"{}\"".format(args.output[0]))

    pdm_signal_16bits = np.zeros((int(pdm_signal.shape[0] / 16),), dtype="uint16")
    # print("PDM_length: {}".format(pdm_signal_16bits.shape[0]))
    for i in range(int(pdm_signal.shape[0] / 16)):
        val = 0
        for j in range(16):
            val += pdm_signal[i * 16 + j] * (2 ** j)
        pdm_signal_16bits[i] = val

    fp = open(args.output[0], "wb")
    for i in range(pdm_signal_16bits.shape[0]):
        fp.write(pdm_signal_16bits[i])
    fp.close()

    fig, axs = plt.subplots(2, 1, constrained_layout=True)
    fig.suptitle("PCM-PDM Signal comparison")

    n = np.linspace(0, 1, pdm_signal.shape[0])
    n_pcm = np.linspace(0, 1, pcm_signal_float.shape[0])
    axs[1].set_title("PDM")
    axs[0].set_title("PCM")
    axs[1].step(n, pdm_signal, label="PDM Signal", where="post")
    axs[0].plot(n_pcm, pcm_signal_float, label="PCM Signal")
    axs[0].legend()
    axs[1].legend()
    axs[0].set_xlim((0, 1))
    axs[1].set_xlim((0, 1))

    plt.figure()
    plt.step(n, pdm_signal, label="PDM Signal", where="post")
    plt.plot(n_pcm, (pcm_signal_float / 2) + .5, label="PCM Signal")
    plt.xlim((0, 1))
    plt.legend()
    plt.show()


if __name__ == '__main__':
    exit(main())
