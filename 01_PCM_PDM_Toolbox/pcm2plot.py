import argparse
import numpy as np
import matplotlib.pyplot as plt


def main():
    arg_parser = argparse.ArgumentParser("pcm2wav", description="Python script to Plot PCM Files")
    arg_parser.add_argument('input', metavar="<Input file>", help="PCM file to convert", nargs='+')
    arg_parser.add_argument("-s", '--sampling', type=int, help="Sampling Frequency", required=False)

    args = arg_parser.parse_args()

    if args.sampling:
        fs = args.sampling
    else:
        fs = 32000

    Ts = []

    pcm_files = []
    for file in args.input:
        file_dict = {
            "signal": np.fromfile(file, dtype='int16'),
            "title": file
        }
        file_dict["n"] = file_dict["signal"].shape[0]
        T = file_dict["n"] / fs
        Ts.append(T)
        file_dict["time_vector"] = np.linspace(0, T, file_dict["n"])
        file_dict["fft"] = np.fft.fft(file_dict["signal"])
        file_dict["freq_vector"] = np.fft.fftfreq(file_dict["n"], d=1 / fs)
        pcm_files.append(file_dict)

    fig, axs = plt.subplots(2, 1, constrained_layout=True)
    fig.suptitle("PCM Sound (Sampled @ {:.1f} kHz)".format(fs / 1000.))
    for data in pcm_files:
        axs[0].plot(data["time_vector"], data["signal"], label=data["title"])
        # plt.plot(t, np.ones((data[0].shape[0],)) * np.average(data[0]), label="avg: {}".format(data[1]))
        print("Avg: \"{}\" = {}".format(data["title"], np.average(data["signal"])))
    axs[0].set_title("Time Domain")
    axs[0].set_xlim((0, max(Ts)))
    axs[0].set_xlabel("Time (s)")
    axs[0].set_ylabel("Amplitude")
    axs[0].legend()

    plt.subplot(212)
    for data in pcm_files:
        axs[1].plot(data["freq_vector"], np.abs(data["fft"]), label=data["title"])
    axs[1].set_xlim([-fs/2, fs / 2])
    axs[1].set_xlabel("Frequency (Hz)")
    axs[1].set_title("Frequency Domain")
    axs[1].legend()

    plt.show()


if __name__ == "__main__":
    exit(main())
