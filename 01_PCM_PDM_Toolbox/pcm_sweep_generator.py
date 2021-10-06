import argparse
import numpy as np
import matplotlib.pyplot as plt


def main():
    argument_parser = argparse.ArgumentParser("pcm_generator", description="Python Script to generate PCM waveforms")
    argument_parser.add_argument("output", metavar='<Output file>', help="PCM output file")
    argument_parser.add_argument('-s', '--sampling', type=int, help="PCM Sampling frequency")

    args = argument_parser.parse_args()

    print("Generating Signal")

    fs = args.sampling if args.sampling else 32000
    fmax = fs/2
    s_time = .01
    n = int(s_time * fs)
    f_range = np.arange(100, fmax, 100)

    t = np.linspace(0, s_time * f_range.shape[0], f_range.shape[0] * n)
    x = np.zeros(t.shape)
    N = x.shape[0]
    for i in range(len(f_range)):
        x[i * n:(i + 1) * n] = 0x7FFF * np.sin(np.pi * 2 * f_range[i] * t[i * n:(i + 1) * n])

    print(max(x))

    fft = np.fft.fft(x)
    fft = fft[range(int(N / 2))]
    frq = np.linspace(0, fs / 2, int(N / 2))

    x_int = np.array(x, dtype='int16')

    fp = open(args.output, "wb")
    for byt in x_int:
        fp.write(byt)
    fp.close()

    plt.semilogy(frq, np.abs(fft))
    plt.show()

    return 0


if __name__ == "__main__":
    exit(main())
