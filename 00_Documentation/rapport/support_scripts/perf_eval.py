import numpy as np
import matplotlib.pyplot as plt

fs_pcm = np.array([8 * i for i in range(0, 15)])

flag_time_us = np.array([1000] * 15)

filtering_time_us_72 = np.array([0, 80, 150, 230, 310, 290, 450, 530, 600, 670, 750, 820, 900, 980, 1060])
filtering_time_us_144 = np.array([0, 44, 88, 128, 166, 204, 248, 284, 328, 368, 408, 448, 488, 532, 572])

load_72 = filtering_time_us_72 / flag_time_us
load_144 = filtering_time_us_144 / flag_time_us
load_72_th = load_144 * 2

plt.figure()
plt.title("Filtering CPU Load")
plt.plot(fs_pcm, load_72 * 100, label="HCLK= 72 MHz")
plt.plot(fs_pcm, load_144 * 100, label="HCLK= 144 MHz")
plt.xlabel("PCM Sampling Frequency (kHz)")
plt.ylabel("CPU Load (% of SAI cycle time)")
plt.ylim((0, 120))
plt.xlim((0, 120))
plt.legend()

plt.show()
