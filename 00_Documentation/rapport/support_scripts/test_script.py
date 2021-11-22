import numpy as np
import matplotlib.pyplot as plt
from string import Template


def pdm_generator(x, qe=0):
    y = np.zeros(x.shape, dtype="int8")
    for i in range(x.shape[0]):
        y[i] = (int(x[i] > qe) * 2) - 1
        qe += y[i] - x[i]
    return y, qe


def undersample(x, factor: int = 4):
    if factor < 1:
        raise AttributeError("Undersampling factor MUST be superior or equal to 1!")
    n = int(x.shape[0] / factor)
    y = np.zeros((n,))
    for i in range(n):
        y[i] = x[factor * i]
    return y


if __name__ == "__main__":
    N = 128
    T = 1
    f0 = 1
    undersampling_factor = 4
    fs = N * T / undersampling_factor
    t = np.linspace(0, T, N)
    t_n = np.linspace(0, N, N)
    t_pcm = undersample(t, undersampling_factor)
    x = (1 / 2) * (np.sin(2 * np.pi * f0 * t) + 1)
    x_pdm, qe = pdm_generator(2 * x - 1, 0)
    x_pcm = undersample(x, undersampling_factor)
    n_b = 4
    for i in range(x_pcm.shape[0]):
        y_i = 0
        x_i = x_pcm[i]
        for j in range((2 ** n_b) + 1):
            lim_b = j / (2 ** n_b)
            lim_h = (j + 1) / (2 ** n_b)
            if lim_b <= x_i < lim_h:
                y_i = lim_b
                break
        x_pcm[i] = y_i

    x_pcm_hf = np.zeros(x.shape)
    for i in range(x_pcm.shape[0]):
        x_pcm_hf[i * undersampling_factor: (i + 1) * undersampling_factor] = x_pcm[i]

    Qerr = np.abs(x - x_pcm_hf)

    fft_pdm = np.fft.fft(x_pdm) / N
    fft_x = np.fft.fft(2 * x - 1) / N

    fft_pcm = np.fft.fft(2 * x_pcm - 1) / x_pcm.shape[0]
    freqs = np.linspace(0, N, N)
    freqs_pcm = np.linspace(0, fs, x_pcm.shape[0])

    print("Qe = {}  (Running Error)".format(qe))

    # fig = plt.figure("Analog_and_PDM")
    # fig.suptitle("Signal and it's PDM Equivalent")
    # ax = fig.add_subplot(111)
    # ax.plot(t_n, x, label="Analog", linewidth=4.0)
    # ax.step(t_n, x_pdm, where="post", label="PDM")
    # ax.set_xlim((-1, N + 1))
    # ax.set_xlabel("Discrete Time (n)")
    # ax.set_ylabel("Signal Amplitude")
    # plt.legend()

    fig, ax = plt.subplots(2, 1, constrained_layout=True)
    fig.suptitle("Analog Signal and It's digital PCM counterpart (fs={:.1f}Hz, n={})".format(fs, n_b))
    ax[0].set_title("Time Domain")
    ax[0].plot(t, x, 'r-', label="Analog Signal")
    # [0]ax.plot(t, Qerr, 'g-', label="Quantification Error")
    # [0]ax.set_yticks([])
    ax[0].legend(loc="upper left")
    ax[0].set_xlabel("Time (s)")
    ax[0].set_xlim((0, T))
    ax[0].set_ylabel("Analog Value")
    ax[0] = ax[0].twinx()
    ax[0].plot(t_pcm, x_pcm, "bo", label="PCM")
    y_tick = [i / (2 ** n_b) for i in range((2 ** n_b))]
    ax[0].set_yticks(y_tick)
    ax[0].set_ylabel("Digital Value")
    fmt_string = "{:0" + str(n_b) + "b}"
    ax[0].set_yticklabels([fmt_string.format(i) for i in range((2 ** n_b))])
    ax[0].legend(loc="upper right")
    ax[0].grid(axis='y')
    ax[1].set_title("Frequency Domain")
    ax[1].plot(freqs_pcm, np.abs(fft_pcm),"b-", label="PCM")
    ax[1].plot(freqs, np.abs(fft_x),"r-", label="Analog")
    ax[1].legend(loc="upper right")
    ax[1].set_xlabel("Frequency (Hz)")
    ax[1].set_xlim((0, fs / 2))
    ax[1].set_ylabel("Amplitude")



    fig, ax = plt.subplots(2,1,constrained_layout=True)
    fig.suptitle(r"Analog Signal and It's PDM digital counterpart (fpdm={} Hz)".format(fs * undersampling_factor))
    ax[0].set_title("Time Domain")
    ax[0].plot(t, 2 * x - 1, 'r-', label="Analog Signal")
    # [0]ax.plot(t, Qerr, 'g-', label="Quantification Error")
    # [0]ax.set_yticks([])
    ax[0].legend(loc="upper left")
    ax[0].set_xlabel("Time (s)")
    ax[0].set_xlim((0, T))
    ax[0].set_ylabel("Analog Value")
    ax[0] = ax[0].twinx()
    ax[0].step(t, x_pdm, "b-", where="post", label="PDM")
    ax[0].set_yticks([-1, 1])
    ax[0].set_ylabel("Digital Value")
    ax[0].set_yticklabels(["False", "True"])
    ax[0].legend(loc="upper right")
    ax[0].grid(axis='y')
    ax[1].set_title("Frequency Domain")
    ax[1].plot(freqs, np.abs(fft_pdm),"b-", label="PDM")
    ax[1].plot(freqs, np.abs(fft_x),"r-", label="Analog")
    ax[1].legend(loc="upper right")
    ax[1].set_xlabel("Frequency (Hz)")
    ax[1].set_xlim((0, N / 2))
    ax[1].set_ylabel("Amplitude")



    # fig = plt.figure("Analog_and_PCM")
    # fig.suptitle("Signal and it's PCM Equivalent")
    # ax = fig.add_subplot(111)
    # ax.plot(t_n, x, label="Analog", linewidth=4.0)
    # # ax.stem(t_n_pcm, x_pcm, label="PCM", basefmt=" ")
    # ax.set_xlim((-1, N + 1))
    # ax.set_xlabel("Discrete Time (n)")
    # ax.set_ylabel("Signal Amplitude")
    # labels = [item.get_text() for item in ax.get_yticklabels()]
    # l_count = len(labels)
    # for i in range(l_count):
    #     print(labels[i])
    # plt.legend()
    #
    # plt.show()

    plt.show()
