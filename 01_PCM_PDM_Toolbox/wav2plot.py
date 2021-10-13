import argparse
import wave

import numpy as np
import matplotlib.pyplot as plt


def main():
    arg_parser = argparse.ArgumentParser("wav2plot", description="Python script to Plot WAV Files")
    arg_parser.add_argument('input', metavar="<Input file>", help="PCM file to convert", nargs='+')

    args = arg_parser.parse_args()



    Ts = []
    fs = 0

    wav_files = []
    for file in args.input:
        file_dict = {
            "title": file
        }
        wav = wave.open(file, 'rb')
        fs = wav.getframerate()
        frames = wav.readframes(wav.getnframes())
        sig = np.frombuffer(frames, dtype='int16')
        file_dict["signal"] = sig
        file_dict["n"] = file_dict["signal"].shape[0]
        T = file_dict["n"] / fs
        Ts.append(T)
        file_dict["time_vector"] = np.linspace(0, T, file_dict["n"])

        fft = np.fft.fft(file_dict["signal"])
        fft = fft[range(int(file_dict["n"] / 2))] / file_dict["n"]
        frq = np.linspace(0, fs / 2, int(file_dict['n'] / 2))
        file_dict["fft"] = fft
        file_dict["freq_vector"] = frq

        wav_files.append(file_dict)

    fig, axs = plt.subplots(2, 1, constrained_layout=True)
    fig.suptitle("WAV Sound (Sampled @ {:.1f} kHz)".format(fs / 1000.))

    for data in wav_files:
        axs[0].plot(data["time_vector"], data["signal"], label=data["title"])
        # plt.plot(t, np.ones((data[0].shape[0],)) * np.average(data[0]), label="avg: {}".format(data[1]))
        # print("Avg: \"{}\" = {}".format(data["title"], np.average(data["signal"])))
    # axs[0].plot([0, 3], [0, 0], label="DAC min")
    # axs[0].plot([0, 3], [0x7FF, 0x7FF], label="DAC half")
    # axs[0].plot([0, 3], [3, 3], label="DAC max")
    axs[0].set_title("Time Domain")
    axs[0].set_xlim((0, max(Ts)))
    # axs[0].set_ylim((0, 3))
    axs[0].set_xlabel("Time (s)")
    axs[0].set_ylabel("Amplitude")
    axs[0].legend()

    for data in wav_files:
        axs[1].plot(data["freq_vector"], 20 * np.log10(np.abs(data["fft"])), label=data["title"])
    axs[1].set_xlim([0, fs / 2])
    axs[1].set_ylabel("Response (dB)")
    axs[1].set_xlabel("Frequency (Hz)")
    axs[1].set_title("Frequency Domain")
    axs[1].legend()

    plt.show()


if __name__ == "__main__":
    exit(main())
