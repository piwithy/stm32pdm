<h1> 01_PCM_PDM_Toolbox </h1>

Ce dossier contient un ensemble de scripts Python pour le traitement des signaux PDM, PCM et WAV. Ces scripts sont développés pour **Python 3.8.X**.


# Scripts
| Script | Objectif |
|:-------|:---------|
|[pcm_generator](#pcm-generator)| Générer des signaux PCM à une fréquence f0 donnée à partir des coefficients de la série de Fourier |
|[pcm2pdm.py](#pcm-2-pdm)| Convertir des signaux PCM en PDM |
|[pcm2plot.py](#pcm-2-plot)| Tracer les composantes temporelles et fréquentielles de signaux PCM|
|[pcm2wav.py](#pcm-2-wav)| Convertir un signal PCM en fichier WAV|
|[wav2plot.py](#wav-2-plot) | Tracer les composantes temporelles et fréquentielles de fichier WAV
|[raw_capture.py](#raw-capture)| Capturer les données transmises par la carte sur le port série

# Installation des modules Python nécessaires
```bash
# À partir de la racine du dépôt
# Création de l’environnement Python virtuel
virtualenv venv
# Activation de l’environnement virtuel
source venv/bin/activate
# Installation des modules Python
pip install -r "requirments.txt"
# Desactivation de l’environnement virtuel
deactivate
```

Extrait de `requirments.txt`
```
cycler==0.10.0
kiwisolver==1.3.2
matplotlib==3.4.3
numpy==1.21.2
Pillow==8.3.2
pyparsing==2.4.7
pyserial==3.5
python-dateutil==2.8.2
scipy==1.7.1
six==1.16.0
```


# PCM Generator

Ce script permet de générer un Signal PCM échantillonné à une fréquence $`fs`$ avec une fréquence fondamentale $`f0`$ qui suit la formule suivante :

pour $`n`$ coefficients stockés dans le tableau $`C`$

```math
x(t) =  \sum_{k=0}^{n-1} C[k] * sin(2* \pi* (k+1) * f0 * t)
```

### Message d’aide
```
usage: pcm_generator [-h] [-s SAMPLING] [-f0 F0] [-t TIME] [-c COEFS [COEFS ...]] <Output file>

Python Script to generate PCM waveforms

positional arguments:
  <Output file>         PCM output file

optional arguments:
  -h, --help            show this help message and exit
  -s SAMPLING, --sampling SAMPLING
                        PCM Sampling frequency
  -f0 F0                Fundamental Frequency of the signal
  -t TIME, --time TIME  Signal Duration
  -c COEFS [COEFS ...], --coefs COEFS [COEFS ...]
                        Fourier Coefficients
```

### Exemple d’utilisation
On veut générer un signal échantillonné à $`fs=48*10^3 Hz`$ avec une fréquence fondamentale $`f0=10^3 Hz`$ et les coefficients de Fourier suivant : `[1 -0.5 0.333 -0.25 0.2 -0.167]` pendant 2 ms.
Le script doit générer 2 périodes d’un signal en dent de scie
Pour se faire, on exécute la commande suivante :
```bash
# À partir de la racine du dépôt avec l’environnement virtuel actif
python 01_PCM_PDM_Toolbox/pcm_generator.py "waveform.pcm" -s 48000 -f0 1000 -t 0.002 -c 1 -0.5 0.333 -0.25 0.2 -0.167
```

on retrouvera alors dans le dossier racine du dépôt un fichier `waveform.pcm` qui contient nos échantillons PCM

Le script trace la figure suivante représentant les échantillons PCM en fonction du temps :

![Generated PCM](../00_Documentation/imgs/01_PCM_PDM_Toolbox/generated_pcm_fs48kHz_f01kHz_dent_de_scie_ordre_6.png)

# PCM 2 Plot

Ce script permet de tracer les composantes temporelles et fréquentielles de plusieurs signaux PCM

### Message d’aide
```
usage : pcm2plot [-h] [-s SAMPLING] <Input file> [<Input file> ...]

Python script to Plot PCM Files

positional arguments:
  <Input file>          PCM file to convert

optional arguments:
  -h, --help            show this help message and exit
  -s SAMPLING, --sampling SAMPLING
                        Sampling Frequency

```

### Exemple d’utilisation
On veut tracer les composantes temporelles et Fréquencielles du signal que l’on a généré dans l’exemple de [PCM Generator](#pcm-generator) afin de s’assurer que l’on a bien les 6 composantes Frequencielles a 1, 2, 3, 4, 5 et 6 kHz

On exécute la commande suivante :
```bash
# À partir de la racine du dépôt avec l’environnement virtuel actif
python 01_PCM_PDM_Toolbox/pcm2plot.py -s 48000 waveform.pcm
```

On obtient alors figure suivante :

![Generated PCM](../00_Documentation/imgs/01_PCM_PDM_Toolbox/time_and_frequency_waveform.png)

Pour mieux distinguer les fréquences, j’ai régénéré le même signal, mais sur 100 ms :

![Generated PCM](../00_Documentation/imgs/01_PCM_PDM_Toolbox/time_and_frequency_waveform_100ms.png)

# PCM 2 WAV

Ce script permet de « convertir » les signaux PCM en fichier son WAV

### Message d’aide
```
usage : pcm2wav [-h] [-s SAMPLING] <Input file> <Output file>

Python script to convert PCM Files to WAV

positional arguments:
  <Input file>          PCM file to convert
  <Output file>         WAV output file

optional arguments:
  -h, --help            show this help message and exit
  -s SAMPLING, --sampling SAMPLING
                        Audio Sampling Frequency


```

### Exemple d’utilisation
Si l’on souhaite convertir le signal PCM généré dans l’exemple [PCM Generator](#pcm-generator)

On exécute la commande suivante :
```bash
# À partir de la racine du dépôt avec l’environnement virtuel actif
python 01_PCM_PDM_Toolbox/pcm2wav.py -s 48000 waveform.pcm waveform.wav
```
Quand on visualise le fichier dans Audacity, on a le signal suivant :

![WAV Signal](../00_Documentation/imgs/01_PCM_PDM_Toolbox/Audacity_wav.png)

# WAV 2 Plot

Ce script permet de tracer les composantes temporelles et Fréquencielles de plusieurs fichiers au format WAV

### Message d’aide
```
usage : wav2plot [-h] <Input file> [<Input file> ...]

Python script to Plot WAV Files

positional arguments:
  <Input file>  PCM file to convert

optional arguments:
  -h, --help    show this help message and exit


```

### Exemple d’utilisation
On veut tracer les composantes temporelles et fréquentielles du signal que l’on a généré dans l’exemple de [PCM Generator](#pcm-generator) puis converti en fichier `WAV`dans l’exemple de [PCM 2 WAV](#pcm-2-wav) afin de s’assurer que l’on a bien les 6 composantes fréquentielles à 1, 2, 3, 4, 5 et 6 kHz

On exécute la commande suivante :
```bash
# À partir de la racine du dépôt avec l’environnement virtuel actif
python 01_PCM_PDM_Toolbox/wav2plot.py waveform.wav
```

On obtient alors figure suivante :

![WAV Plot](../00_Documentation/imgs/01_PCM_PDM_Toolbox/time_and_frequency_waveform_wav.png)

on obtient la sortie suivante en console :
```
File: .\waveform.wav
        Channels            : 1
        Sample Width        : 16 bits
        Sampling Frequency  : 48.0 kHz
        WAV sample count    : 96
        Duration            : 0.00 s
        Compression type    : NONE
        Compression name    : not compressed
        Size Consistent     : Yes
        Signal Minimum      : -32767
        Signal Maximum      : 32767
        Signal Average      : 0.00
        FFT Max Frequency   : 1.02 kHz
        FFT Max Value       : 80.00 dB
```


Pour mieux distinguer les fréquences, j’ai régénéré le même signal, mais sur 100 ms :

![Generated PCM](../00_Documentation/imgs/01_PCM_PDM_Toolbox/time_and_frequency_waveform_100ms_wav.png)

on obtient la sortie suivante en console :
```
File: .\waveform.wav
        Channels            : 1
        Sample Width        : 16 bits
        Sampling Frequency  : 48.0 kHz
        WAV sample count    : 4800
        Duration            : 0.10 s
        Compression type    : NONE
        Compression name    : not compressed
        Size Consistent     : Yes
        Signal Minimum      : -32767
        Signal Maximum      : 32767
        Signal Average      : 0.00
        FFT Max Frequency   : 1.00 kHz
        FFT Max Value       : 80.08 dB

```

# PCM 2 Plot

Ce script permet de convertir un signal PCM en signal PDM

(:warning: La fréquence d’échantillonnage du signal PDM sera la même que celle du signal PCM :warning:)

### Message d’aide
```
usage : pcm2pdm [-h] <Input file> <Output file>

Python script to convert PCM to PDM

positional arguments:
  <Input file>   PCM input file
  <Output file>  PDM output file

optional arguments:
  -h, --help     show this help message and exit
```

### Exemple d’utilisation
On veut convertir en PDM le signal que l’on a généré dans l’exemple de [PCM Generator](#pcm-generator)

On exécute la commande suivante :
```bash
# À partir de la racine du dépôt avec l’environnement virtuel actif
python 01_PCM_PDM_Toolbox/pcm2pdm.py waveform.pcm waveform.pdm
```

On obtient alors figure suivante :

![PCM To PDM](../00_Documentation/imgs/01_PCM_PDM_Toolbox/pcm_pdm_converted.png)

Pour permettre un traitement par le Programme C de filtrage j’ai régénéré le signal avec une fréquence d’échantillonnage $`fs=3.072MHz = 48 kHz*64`$ :

![PCM To PDM](../00_Documentation/imgs/01_PCM_PDM_Toolbox/pcm_pdm_converted_3072kHz.png)

# Raw Capture

Ce script permet de capturer les données envoyées par la carte sur le port série

### Message d’aide
```
usage: raw_capture.py [-h] [-b baud] <OUTPUT FILE> <COM PORT>

Python Script to capture RAW data From Serial Port

positional arguments:
  <OUTPUT FILE>  File where the captured data will be stored
  <COM PORT>     COM PORT to capture from

optional arguments:
  -h, --help     show this help message and exit
  -b baud        Serial Port Speed in Baud (defaults @115200 Baud)
```

### Exemple d’utilisation
Si on veut capturer les données enregistrées par [parrot](#wip)

Dans mon cas le port série de la carte est connecté sur le port COM4

On exécute la commande suivante :
```bash
# À partir de la racine du dépôt avec l’environnement virtuel actif
python 01_PCM_PDM_Toolbox/raw_capture.py 'captured.pcm' '/dev/ttyUSB0' -b 115200
```

On obtient alors un fichier Wav qui une fois fournis à [WAV 2 Plot](#Wav_to_plot) nous donne la figure suivante :

![Captured WAV](../00_Documentation/imgs/01_PCM_PDM_Toolbox/raw_capture_from_parrot_WAV.png)

On obtient les relevés suivants sur l’oscilloscope (« Channel 1 » : DAC Gauche, « Channel 2 » : DAC Droit) :

![Relevés oscilloscope](../00_Documentation/imgs/01_PCM_PDM_Toolbox/raw_capture_from_parrot_DSO.png)

Capture d’écran de l’oscilloscope :

![Capture d’écran oscilloscope](../00_Documentation/imgs/01_PCM_PDM_Toolbox/raw_capture_from_parrot_DSO_screen.png)
