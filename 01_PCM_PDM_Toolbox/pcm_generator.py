import argparse
import numpy as np
import matplotlib.pyplot as plt


def main():
    argument_parser = argparse.ArgumentParser("pcm_generator", description="Python Script to generate PCM waveforms")
    argument_parser.add_argument("output", metavar='<Output file>', help="PCM output file")
    argument_parser.add_argument('-s', '--sampling', type=int, help="PCM Sampling frequency")
    argument_parser.add_argument('-f0', type=int, help='Fundamental Frequency of the signal')
    argument_parser.add_argument('-t', '--time', type=float, help="Signal Duration")
    argument_parser.add_argument('-c', '--coeffs', type=float, help="Fourier Coefficients", nargs='+')

    args = argument_parser.parse_args()

    print("Generating Signal")

    fs = args.sampling if args.sampling else 32000
    f0 = args.f0 if args.f0 else 1000
    T = args.time if args.time else 5.
    coeffs = args.coeffs if args.coeffs else [1.]
    N = int(T * fs)

    t = np.linspace(0, T, N)
    x = np.zeros(t.shape)
    for i in range(len(coeffs)):
        x += coeffs[i] * np.sin(2 * np.pi * (i + 1) * f0 * t)

    x = (x / np.max(x)) * 0x7FFF
    x_int = np.array(x, dtype='int16')

    fp = open(args.output, "wb")
    bytes_wr = 0
    for byt in x_int:
        fp.write(byt)
        bytes_wr += 2
    print("Wrote {} Byte{} ({} samples) to \"{}\"".format(bytes_wr, "s" if bytes_wr > 1 else "", bytes_wr * 2,
                                                          args.output))


    plt.title("Generated PCM Signal\nfs={:.1f}kHz, f0={}Hz, Fourier coefficients: {}".format(fs / 1000, f0, coeffs))
    # plt.plot(t, x, label='float64')!

    plt.plot(t, x_int, label='PCM Waveform')
    plt.xlim((0, T))
    plt.legend()
    plt.xlabel("Time (s)")
    plt.ylabel("Amplitude")
    plt.show()

    return 0


if __name__ == "__main__":
    exit(main())
