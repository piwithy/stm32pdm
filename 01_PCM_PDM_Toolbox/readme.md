<h1> 01_PCM_PDM_Toolbox </h1>
Ce dossier contien un ensemble de scipt Python pour le traitement des signaux PDM, PCM et WAV

# Scripts
| Script | Objectif |
|:-------|:---------|
|[pcm_generator](pcm_generator.py)| Générer des signaux PCM à une fréquence f0 donnée à partir des coefficiant de la série de Fourier |
|[pcm2pdm](pcm2pdm.py)| Convertir des signaux PCM en PDM |
|[pcm2plot](pcm2plot.py)| Tracer les composantes temporelles et Frequencielles de signaux PCM|
|[pcm2wav](pcm2wav.py)| Convertir un signal PCM en fichier WAV|
|[wav2plot](wav2plot.py) | Tracer les composantes temporelles et Frequencielles de fichier WAV


# PCM Generator

Ce script permet de Générer un Signal PCM Echantionné à une fréquence $`fs`$ avec une fréquence fondamentalle $`f0`$ qui suit la formule suivante:

pour $`n`$ coefficiant stocké dans le tableau $`C`$

```math
x(t) =  \sum_{k=0}^{n-1} C[k] * sin(2* \pi* (k+1) * f0 * t)
```
