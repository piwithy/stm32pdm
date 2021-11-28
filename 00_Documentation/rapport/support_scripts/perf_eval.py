import numpy as np
import matplotlib.pyplot as plt

fs_pcm = np.array([8 * i for i in range(1, 25)])

flag_time_us = np.array([1000] * fs_pcm.shape[0])

filtering_time_us_72 = np.array([80, 150, 230, 310, 290, 450, 530, 600, 670, 750, 820, 900, 980, 1060])
filtering_time_us_108 = np.array([52, 108, 164, 212, 264, 316, 372, 420, 476, 524, 576, 628, 680, 732,
                                  784, 840, 892, 940, 995, 1060])
filtering_time_us_144 = np.array([44, 88, 128, 166, 204, 248, 284, 328, 368, 408, 448, 488, 532, 572,
                                  612, 652, 696, 736, 776, 816, 860, 896, 940, 985])

load_72 = filtering_time_us_72 / flag_time_us[:filtering_time_us_72.shape[0]]
load_108 = filtering_time_us_108 / flag_time_us[:filtering_time_us_108.shape[0]]

load_144 = filtering_time_us_144 / flag_time_us

fig, ax = plt.subplots(constrained_layout=True)
fig.suptitle("SAI Cycle Occupation by the PDM Filter at different PCM Sampling Frequency")
ax.plot(fs_pcm[:filtering_time_us_72.shape[0]], load_72 * 100, label="$f_{system} = 72 MHz$")
ax.plot(fs_pcm[:filtering_time_us_108.shape[0]], load_108 * 100, label="$f_{system} = 108 MHz$")
ax.plot(fs_pcm, load_144 * 100, label="$f_{system} = 144 MHz$")

ax.set_xlabel("PCM Sampling Frequency (kHz)")
ax.set_ylabel("SAI Cycle Occupation (%)")
ax.xaxis.set_ticks([8 * i for i in range(00, 25, 2)])
plt.ylim((0, 110))
ax.set_xlim((0, 200))
ax.yaxis.grid(True)
ax.legend()

plt.show()
