import scipy.signal
import numpy as np
import matplotlib.pyplot as plt


class PDMFir:
    def __init__(self, filter_taps: int, signal_fs: int, filter_cutoff: int, cutoff_window: int = 2000,
                 scale_bits: int = 30, plot: bool = False):
        print(
            "Generating PDM Filter LUT:\n\tPDM fs = {:.1f} kHz\n\tFilter Cutoff = {:.1f} kHz\n\tCut Off Window: {:.1f} kHz".format(
                signal_fs / 1000, filter_cutoff / 1000, cutoff_window / 1000))
        self.taps_count = filter_taps
        self.scale_bits = scale_bits
        self.taps = scipy.signal.firwin(16 * self.taps_count, filter_cutoff, cutoff_window, fs=signal_fs)
        self.amplified_taps = (self.taps * (2 ** self.scale_bits)).astype(int)
        if plot:
            # plt.figure("FIR Window")
            # plt.title("FIR Window")
            # plt.plot(self.amplified_taps)

            plt.figure("Frequency Response")
            plt.title("Digital Frequency Response")
            plt.xlabel("Frequency (Hz)")
            plt.ylabel("Response (dB)")
            w, h = scipy.signal.freqz(self.taps)
            plt.plot(w * signal_fs / (2 * np.pi), 20 * np.log10(np.abs(h)))
            plt.xlim((0, signal_fs / 2))
        self.byte_coefs = self.__gen_byte_coef_array()

    def __gen_byte_coef_array(self):
        byte_coeff = np.zeros((self.taps_count * 2, 256), dtype='int32')
        for i in range(byte_coeff.shape[0]):
            for j in range(byte_coeff.shape[1]):
                byte_coeff[i][j] = self.__gen_byte_coef(i, j)
        return byte_coeff

    def __gen_byte_coef(self, i, b):
        bit, off, tot = 1 << 7, 8 * i, 0
        while bit:
            if bit & b:
                tot += self.amplified_taps[off]
            else:
                tot -= self.amplified_taps[off]
            bit >>= 1
            off += 1
        return tot

    def filter_signal(self, x: np.ndarray, d: int = 64):
        y_hf = np.convolve(x, self.taps, mode="same")
        y_lf = np.zeros((int(y_hf.shape[0] / d),))
        for i in range(int(y_hf.shape[0] / d)):
            y_lf[i] = y_hf[d * i]
        return y_hf, y_lf


def pdm_generator(x, qe=0.):
    y = np.zeros(x.shape, dtype="int8")
    for i in range(len(x)):
        if x[i] > qe:
            y[i] = 1
        else:
            y[i] = -1
        qe += y[i] - x[i]
    return y, qe


def main():
    D = 64
    fs_pdm = 3.072 * 10 ** 6
    fs_pcm = fs_pdm / D
    f0 = 1000
    t0 = 1 / f0
    T = 10 * 10 ** -3
    N_hf = int(T * fs_pdm)

    t_hf = np.linspace(0, T, N_hf)

    N_lf = int(N_hf / D)
    t_lf = np.zeros((N_lf,))

    for i in range(int(N_lf)):
        t_lf[i] = t_hf[D * i]

    points_per_period_hf = int(t0 * fs_pdm)

    print("fs_PDM={:.1f}kHz, fs_PCM={:.1f}kHz, D={}".format(fs_pdm * 10 ** -3, fs_pcm * 10 ** -3, D))
    print("f0={:.1f}kHz, N_per={}".format(f0 * 10 ** -3, points_per_period_hf))

    idx_scale_start = int(N_hf / 2)
    idx_scale_end = idx_scale_start + (points_per_period_hf * 1)
    x_scale = (t_hf[idx_scale_start], t_hf[idx_scale_end])
    order = 20
    x_pcm_hf = np.zeros(t_hf.shape)
    for i in range(1, order + 1):
        a = (2 / np.pi) * ((-1) ** (i + 1)) / i
        x_pcm_hf += a * np.sin(2 * np.pi * f0 * t_hf * i)

    x_pcm_hf /= np.max(x_pcm_hf)
    x_pdm, qe = pdm_generator(x_pcm_hf, 0.)

    fir_filter = PDMFir(16, fs_pdm, 20000, 4000, 30, True)

    x_pcm_filtered_hf, x_pcm_filtered_lf = fir_filter.filter_signal(x_pdm, D)

    # FFTs

    fft_pcm_hf = np.abs(np.fft.fft(x_pcm_hf) / N_hf)
    fft_pdm_hf = np.abs(np.fft.fft(x_pdm) / N_hf)
    fft_filtered_hf = np.abs(np.fft.fft(x_pcm_filtered_hf) / N_hf)
    fft_filtered_lf = np.abs(np.fft.fft(x_pcm_filtered_lf) / N_lf)

    freqs_hf = np.linspace(0, fs_pdm, N_hf)
    freqs_lf = np.linspace(0, fs_pcm, N_lf)

    # ------------------------------------------------------------------------------------------------------------------
    # Original Signal
    fig, ax = plt.subplots(2, 1, constrained_layout=True)
    fig.suptitle("Original Signal")

    # Configuring Subplot 1 | Time
    ax[0].set_title("Time Domain")

    # X Axis
    ax[0].set_xlabel("Time (s)")
    ax[0].set_xlim(x_scale)

    # dual Y Scale
    # ...

    # Y Axes
    ax[0].set_ylabel("Amplitude")

    # Plot
    ax[0].plot(t_hf, x_pcm_hf, 'r-', label="Original Signal")

    # legending
    ax[0].legend(loc="upper left")

    # Configuring Subplot 2 | Frequency
    ax[1].set_title("Frequency Domain")

    # X Axis
    ax[1].set_xlabel("Frequency (Hz)")
    ax[1].set_xlim((0, fs_pdm / 2))

    # dual Y Scale
    # ...

    # Y Axes
    ax[1].set_ylabel("Amplitude")

    # Plot
    ax[1].plot(freqs_hf, fft_pcm_hf, 'r-', label="Original Signal")

    # legending
    ax[1].legend(loc="upper left")

    # ------------------------------------------------------------------------------------------------------------------
    # Original + PDM Signal
    fig, ax = plt.subplots(2, 1, constrained_layout=True)
    fig.suptitle("Converting to PDM | Original  And PDM Signals")

    # Configuring Subplot 1 | Time
    ax[0].set_title("Time Domain")

    # X Axis
    ax[0].set_xlabel("Time (s)")
    ax[0].set_xlim(x_scale)

    # dual Y Scale
    ax_tmp = ax[0].twinx()
    ax[0] = (ax[0], ax_tmp)

    # Y Axes
    ax[0][1].set_ylabel("Analog Value")
    ax[0][0].set_ylabel("Digital Value")
    ax[0][0].set_yticks([-1, 1])
    ax[0][0].set_yticklabels(["False", "True"])

    # Plot
    ax[0][1].plot(t_hf, x_pcm_hf, 'r-', label="Original Signal")
    ax[0][0].step(t_hf, x_pdm, 'b-', label="PDM Signal")

    # legending
    ax[0][1].legend(loc="upper left")
    ax[0][0].legend(loc="upper right")
    ax[0][0].yaxis.tick_right()
    ax[0][0].yaxis.set_label_position("right")
    ax[0][1].yaxis.tick_left()
    ax[0][1].yaxis.set_label_position("left")

    # Configuring Subplot 2 | Frequency
    ax[1].set_title("Frequency Domain")

    # X Axis
    ax[1].set_xlabel("Frequency (Hz)")
    ax[1].set_xlim((0, fs_pdm / 2))

    # dual Y Scale
    # ...

    # Y Axes
    ax[1].set_ylabel("Amplitude")

    # Plot
    ax[1].plot(freqs_hf, fft_pdm_hf, 'b-', label="PDM Signal")
    ax[1].plot(freqs_hf, fft_pcm_hf, 'r-', label="Original Signal")

    # legending
    ax[1].legend(loc="upper left")

    # ------------------------------------------------------------------------------------------------------------------
    # PDM + filtered (HF) Signal
    fig, ax = plt.subplots(2, 1, constrained_layout=True)
    fig.suptitle("Filtering | PDM and Filtered Signals")

    # Configuring Subplot 1 | Time
    ax[0].set_title("Time Domain")

    # X Axis
    ax[0].set_xlabel("Time (s)")
    ax[0].set_xlim(x_scale)

    # dual Y Scale
    ax_tmp = ax[0].twinx()
    ax[0] = (ax[0], ax_tmp)

    # Y Axes
    ax[0][1].set_ylabel("PCM Value")
    ax[0][0].set_ylabel("Digital Value")
    ax[0][0].set_yticks([-1, 1])
    ax[0][0].set_yticklabels(["False", "True"])

    # Plot
    ax[0][1].plot(t_hf, x_pcm_filtered_hf, 'b-', label="Filtered Signal")
    ax[0][0].step(t_hf, x_pdm, 'r-', label="PDM Signal")

    # legending
    ax[0][0].legend(loc="upper left")
    ax[0][1].legend(loc="upper right")

    # Configuring Subplot 2 | Frequency
    ax[1].set_title("Frequency Domain")

    # X Axis
    ax[1].set_xlabel("Frequency (Hz)")
    ax[1].set_xlim((0, fs_pdm / 2))

    # dual Y Scale
    # ...

    # Y Axes
    ax[1].set_ylabel("Amplitude")

    # Plot
    ax[1].plot(freqs_hf, fft_pdm_hf, 'r-', label="PDM Signal")
    ax[1].plot(freqs_hf, fft_filtered_hf, 'b-', label="Filtered Signal")

    # legending
    ax[1].legend(loc="upper left")

    # ------------------------------------------------------------------------------------------------------------------
    # Filtered and PCM
    fig, ax = plt.subplots(2, 1, constrained_layout=True)
    fig.suptitle("Down Sampling | Filtered Signal and Down Sampled")

    # Configuring Subplot 1 | Time
    ax[0].set_title("Time Domain")

    # X Axis
    ax[0].set_xlabel("Time (s)")
    ax[0].set_xlim(x_scale)

    # dual Y Scale
    # ...

    # Y Axes
    ax[0].set_ylabel("Amplitude")

    # Plot
    ax[0].plot(t_hf, x_pcm_filtered_hf, "r-", label="Filtered Signal")
    ax[0].stem(t_lf, x_pcm_filtered_lf, label="Down Sampled Signal", basefmt=" ", markerfmt="bo", linefmt="b-")
    # legending
    ax[0].legend(loc="upper left")

    # Configuring Subplot 2 | Frequency
    ax[1].set_title("Frequency Domain")

    # X Axis
    ax[1].set_xlabel("Frequency (Hz)")
    ax[1].set_xlim((0, fs_pcm / 2))

    # dual Y Scale
    # ...

    # Y Axes
    ax[1].set_ylabel("Amplitude")

    # Plot
    ax[1].plot(freqs_hf, fft_filtered_hf, 'r-', label="Filtered Signal")
    ax[1].plot(freqs_lf, fft_filtered_lf, 'b-', label="Down Sampled Signal")

    # legending
    ax[1].legend(loc="upper left")

    # ------------------------------------------------------------------------------------------------------------------
    # Original and PCM
    fig, ax = plt.subplots(2, 1, constrained_layout=True)
    fig.suptitle("Original and PCM Signal")

    # Configuring Subplot 1 | Time
    ax[0].set_title("Time Domain")

    # X Axis
    ax[0].set_xlabel("Time (s)")
    ax[0].set_xlim(x_scale)

    # dual Y Scale
    # ...

    # Y Axes
    ax[0].set_ylabel("Amplitude")

    # Plot
    ax[0].plot(t_hf, x_pcm_hf, "r-", label="Orignal Signal")
    ax[0].plot(t_lf, x_pcm_filtered_lf, 'b-', label="PCM Signal")
    # legending
    ax[0].legend(loc="upper left")

    # Configuring Subplot 2 | Frequency
    ax[1].set_title("Frequency Domain")

    # X Axis
    ax[1].set_xlabel("Frequency (Hz)")
    ax[1].set_xlim((0, fs_pcm / 2))

    # dual Y Scale
    # ...

    # Y Axes
    ax[1].set_ylabel("Amplitude")

    # Plot
    ax[1].plot(freqs_hf, fft_pcm_hf, 'r-', label="Original Signal")
    ax[1].plot(freqs_lf, fft_filtered_lf, 'b-', label="PCM Signal")

    # legending
    ax[1].legend(loc="upper left")

    # plt.figure("Original")
    # plt.title("Original Signal - Centered on 1 Period")
    # plt.plot(t_hf, x_pcm_hf, label="Original Signal")
    # plt.xlabel("Time (s)")
    # plt.ylabel("Value")
    # plt.legend(loc='upper right')
    # plt.xlim(x_scale)
    #
    # plt.figure("Original PCM to PDM")
    # plt.title("Original Signal : PCM & PDM  - Centered on 1 Period")
    # plt.step(t_hf, x_pdm, label="PDM", where="post")
    # plt.plot(t_hf, x_pcm_hf, label="PCM")
    # plt.xlabel("Time (s)")
    # plt.ylabel("Value")
    # plt.legend(loc='upper right')
    # plt.xlim(x_scale)
    #
    # plt.figure("PDM to filtered HF")
    # plt.title("PDM Signal \u2192 Filtered Signal (before decimation) - Centered on 1 Period")
    # plt.step(t_hf, x_pdm, label="PDM", where="post")
    # plt.plot(t_hf, x_pcm_filtered_hf, label="Filtered PCM (before decimation)")
    # plt.xlabel("Time (s)")
    # plt.ylabel("Value")
    # plt.legend(loc='upper right')
    # plt.xlim(x_scale)
    #
    # plt.figure("Filtered HF to LF")
    # plt.title("Decimation of the Filtered Signal - Centered on 1 Period")
    # plt.plot(t_hf, x_pcm_filtered_hf, '.', label="Not Decimated")
    # plt.plot(t_lf, x_pcm_filtered_lf, '.', label="Decimated")
    # plt.xlabel("Time (s)")
    # plt.ylabel("Value")
    # plt.legend(loc='upper right')
    # plt.xlim(x_scale)
    #
    # filt_err = np.zeros(t_lf.shape)
    # for i in range(N_lf):
    #     filt_err[i] = np.abs(x_pcm_hf[i * D] - x_pcm_filtered_lf[i])
    #
    # print("Filter Error: max={:.3f} %; min={:.3f} %, avg={:.3f} %".format(
    #     np.max(filt_err) * 10 ** 2, np.min(filt_err) * 10 ** 2, np.average(filt_err) * 10 ** 2
    # ))
    #
    # plt.figure("Original vs filtered")
    # plt.title("Original signal and filtered Signal - Centered on 1 Period")
    # plt.xlabel("Time (s)")
    # plt.ylabel("Value")
    # plt.plot(t_hf, x_pcm_hf, label="Original")
    # plt.plot(t_lf, x_pcm_filtered_lf, label="Filtered")
    # plt.plot(t_lf, filt_err, '-', label="| Original - Filtered |")
    # plt.xlim(x_scale)
    # plt.legend()

    plt.show()
    return 0


if __name__ == "__main__":
    exit(main())
