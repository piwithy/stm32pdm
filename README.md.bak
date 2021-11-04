# STM32PDM

Traitement de signal audio embarqué temps-réel sur carte STM32



## Contenu du dépôt

### 00_Documentation

Dossier contenant la documentation relative à la réalisation du projet (Carte Cible, µ-Controlleur, Exemples...)

### 01_PCM_PDM_Toolbox

Dossier contenant divers scripts Python pour les conversions PCM PDM WAV

#### pcm2pdm.py

Script pour convertir un Signal PCM en Signal PDM

Message d'aide:

```
usage: pcm2pdm [-h] [-f FACTOR] <Input file> <Output file>

Python script to convert PCM to PDM

positional arguments:
  <Input file>          PCM input file
  <Output file>         PDM output file

optional arguments:
  -h, --help            show this help message and exit
  -f FACTOR, --factor FACTOR
                        (PDM fs / PCM fs) factor
```

#### pcm2plot.py

Script pour tracer les composantes temporelles et fréquentielles de signaux PCM

Message d'aide:

 ```
 usage: pcm2wav [-h] [-s SAMPLING] <Input file> [<Input file> ...]
 
 Python script to Plot PCM Files
 
 positional arguments:
   <Input file>          PCM file to convert
 
 optional arguments:
   -h, --help            show this help message and exit
   -s SAMPLING, --sampling SAMPLING
                         Sampling Frequency
 ```



#### pcm2wav.py

Script pour convertir un signal PCM en fichier WAV

Message d'aide:

```
usage: pcm2wav [-h] [-s SAMPLING] <Input file> <Output file>

Python script to convert PCM Files to WAV

positional arguments:
  <Input file>          PCM file to convert
  <Output file>         WAV output file

optional arguments:
  -h, --help            show this help message and exit
  -s SAMPLING, --sampling SAMPLING
                        Audio Sampling Frequency
```

#### pcm_generator.py

Script pour générer des signaux PCM

Message d'aide:

```
usage: pcm_generator [-h] [-s SAMPLING] [-f0 F0] [-t TIME] [-c COEFFS [COEFFS ...]] <Output file>

Python Script to generate PCM waveforms

positional arguments:
  <Output file>         PCM output file

optional arguments:
  -h, --help            show this help message and exit
  -s SAMPLING, --sampling SAMPLING
                        PCM Sampling frequency
  -f0 F0                Fundamental Frequency of the signal
  -t TIME, --time TIME  Signal Duration
  -c COEFFS [COEFFS ...], --coeffs COEFFS [COEFFS ...]
                        Fourier Coefficients
```

Exemple de signal généré:

![pcm_generator_ex1.png](00_Documentation/imgs/pcm_generator_ex1.png)

### 02_Capturing

Dossier contenant les scripts Python utlisés pour la capture des données envoyées par la carte sur le port série

#### serial_capture.py

Fichier Python de "librairie" utilisé pour la capture de données sur le port série

Fonctions exposées:

```Python
def serial_capture(com_port:str, speed:int, timeout_s:float):
    """
    Function to capture Data from the serial Port

    Parameters
    ----------
    com_port : str
        Serial Port to listen
    speed: int
        Serial Communication Speed in Bauds
    timeout_s: float
        Time in seconds after last byte received to end the communication

    Returns
    -------
    list
        list of the bytes received
    """
    ...
```

#### raw_capture.py

Script Python exportant les données capturées sur le port série dans un fichier

Message d'aide:

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

### 03_Filtering

Dossier contenant le programme C utilisant la même libraire que celle embarqué pour le filtrage PDM dasn la carte cible

#### Compilation

```bash
cd 03_Filtering
mkdir build && cd build
cmake ..
make -j${nproc}
```

#### Message d'aide

```
PDM Filter v0.0.1
USAGE: ./pdm_filter [-h] [-f FACTOR] <Input File> <Outpufile>
C Program to filter PDM Signals to PCM format

Positional arguments:
    <Input File>        PDM Signal file
    <Output File>       PCM output file

Optional arguments:
    -h                  Show this help message
    -f FACTOR           Decimation Factor of the filter
```

### 04_Embeddeding

### Parrot

Parrot es tun demonstrateur qui lors de sur le bouton "USER" de la carte lance l'enregistremement des son captés par le microphone PDM et lors de d'un second appui  sur le bouton USER  les 3 dernière secondes sont rejouée sur le DAC puis sont tranmise au format WAV sur le port Serie.

#### Compilation

Pour compiler est téléverser le prgramme sur la carte

``` BASH
# A Partir du dossier racine du dépot
# /!\ Requière au minimum CMAKE 3.20 (sources cmake https://github.com/Kitware/CMake) /!\
# /!\ requirère le cross compilateur gcc-arm-none-eabi (disponible dans les paquets APT)
mkdir 04_Embeddeding/parrot/build && cd 04_Embeddeding/parrot/build
cmake ..
make -j ${nproc}

# Pour Charger les binaire il faut installé st-link (sources : https://github.com/stlink-org/stlink)
# Il est preferable de le compiler depuis les sources (Guide: https://freeelectron.ro/installing-st-link-v2-to-flash-stm32-targets-on-linux/)
st-flash write parrot.bin 0x8000000
```

Une fois le programme compilé apuyer sur le bouton RESET de la carte. La coarte execute maitenenant le programme.

#### Configuration CubeMX

##### Pinout

<table>
	<thead>
		<tr>
			<th>PIN</th>
			<th>Signal</th>
            <th>Commentaire</th>
            <th>Péripherique</th>
		<tr>
	</thead>
	<tbody>
		<tr>
			<td>PA0</td>
			<td>USER_BUTTON</td>
			<td>Bouton Bleu de la carte</td>
			 <td style="text-align: center; vertical-align: middle;" rowspan="3">GPIO</td>
        </tr>
        <tr>
			<td>PG13</td>
			<td>LD3</td>
			<td>LED Verte</td>
        </tr>
		<tr>
			<td>PG14</td>
			<td>LD4</td>
			<td>LED Rouge</td>
		<tr>
        <tr>
        	<td>PA4</td>
            <td>DAC_OUT1</td>
            <td>Canal Gauche du DAC (Fil Rouge)</td>
            <td style="text-align: center; vertical-align: middle;" rowspan="2">DAC</td>
        </tr>
        <tr>
        	<td>PA5</td>
            <td>DAC_OUT2</td>
            <td>Canal Droit du DAC (Fil Blanc)</td>
        </tr>
        <tr>
        	<td>PH0</td>
            <td>RCC_OSC_IN</td>
            <td style="vertical-align: middle;" rowspan="2">Signaux de l'oscillateur</td>
            <td style="text-align: center; vertical-align: middle;" rowspan="2">RCC</td>
        </tr>
         <tr>
        	<td>PH1</td>
            <td>RCC_OSC_OUT</td>
        </tr>
        <tr>
        	<td>PE4</td>
            <td>SAI_FS_A</td>
            <td>SAI SELECT</td>
            <td style="text-align: center; vertical-align: middle;" rowspan="3">SAI 1</td>
        </tr>
         <tr>
        	<td>PE5</td>
            <td>SAI_SCK_A</td>
            <td>SAI CLOCK</td>
        </tr>
        <tr>
        	<td>PE6</td>
            <td>SAI_SD_A</td>
            <td>SAI DATA</td>
        </tr>
        <tr>
        	<td>PA13</td>
            <td>SYS_JTMS-SWDIO</td>
            <td style="vertical-align: middle;" rowspan="2">Lignes de DEBUG</td>
            <td style="text-align: center; vertical-align: middle;" rowspan="2">SYS</td>
        </tr>
         <tr>
        	<td>PA14</td>
            <td>SYS_JTCK-SWCLK</td>
        </tr>
        <tr>
        	<td>PA9</td>
            <td>USART1_TX</td>
            <td>Canal TX de l'USART 1</td>
             <td style="text-align: center; vertical-align: middle;" rowspan="2">USART 1</td>
        </tr>
        <tr>
        	<td>PA9</td>
            <td>USART1_RX</td>
            <td>Canal RX de l'USART 1</td>
        </tr>
	</tbody>
</table>
##### GPIO

###### MODE

N/A

###### Configuration

| Pin Name | Signal on Pin | PGIO output Level | GPIO Mode                                             | GPIO Pull-up/Pull-down      | Maximum Output Speed | User Label   | Type            |
| -------- | ------------- | ----------------- | ----------------------------------------------------- | --------------------------- | :------------------: | ------------ | --------------- |
| PAO/WKUP | N/A           | N/A               | **External Event with Rising edge trigger detection** | No pull-up and no pull-down |         N/A          | **USER_BTN** | **GPIO_EXTIO**  |
| PG13     | N/A           | Low               | Output Push Pull                                      | No pull-up and no pull-down |         Low          | **LD3**      | **GPIO_Output** |
| PG14     | N/A           | Low               | Output Push Pull                                      | No pull-up and no pull-down |         Low          | **LD4**      | **GPIO_Output** |

##### RCC

###### MODE

| Clé                          | Valeur                          |
| ---------------------------- | ------------------------------- |
| High Speed Clock (HSE)       | **Crystal / Ceramic Resonator** |
| Low Speed Clock (LSE)        | Disable                         |
| Master Clock Output 1        | No                              |
| Master Clock Output 2        | No                              |
| Audio Clock Input (I2S_CKIN) | No                              |

###### Configuration

Par défaut

##### SYS

###### MODE

| Clé             | Valeur          |
| --------------- | --------------- |
| Debug           | **Serial Wire** |
| System Wake Up  | N/A             |
| Timebase Source | **TIM6**        |

###### Configuration

N/A

##### DAC

###### MODE

| Clé                | Valeur  |
| ------------------ | ------- |
| OUT1 Configuration | **Yes** |
| OUT2 Configuration | **Yes** |
| External Trigger   | No      |

###### Configuration

<table>
    <thead>
    	<tr>
        	<th>Clé</th>
            <th>Valeur</th>
        </tr>
    </thead>
    <tbody>
    	<tr>
        	<td style="text-align: center" colspan="2">DAC Out1 Settings</td>
        </tr>
        <tr>
        	<td>Output Buffer</td>
            <td>Enable</td>
        </tr>
        <tr>
        	<td>Trigger</td>
            <td style="font-weight: bolder;">Timer 2 Trigger Out event</td>
        </tr>
        <tr>
            <td>Wave Generation mode</td>
            <td>Disabled</td>
        </tr>
            	<tr>
        	<td style="text-align: center" colspan="2">DAC Out2 Settings</td>
        </tr>
        <tr>
        	<td>Output Buffer</td>
            <td>Enable</td>
        </tr>
        <tr>
        	<td>Trigger</td>
            <td style="font-weight: bolder;">Timer 2 Trigger Out event</td>
        </tr>
        <tr>
            <td>Wave Generation mode</td>
            <td>Disabled</td>
        </tr>
    </tbody>
</table>

###### DMA

| DMA Request | Stream       | Direction            | Priority | Mode         | Data Width    |
| ----------- | ------------ | -------------------- | -------- | ------------ | ------------- |
| **DAC1**    | DMA1 Stream5 | Memory to Peripheral | Low      | **Circular** | **Half Word** |
| **DAC2**    | DMA1 Stream6 | Memory to Peripheral | Low      | **Circular** | **Half Word** |



##### TIM2

###### MODE

| Clé                        | Valeur             |
| -------------------------- | ------------------ |
| Slave Mode                 | Disable            |
| Trigger Source             | Disable            |
| Clock Source               | **Internal Clock** |
| Channel1                   | Disable            |
| Channel2                   | Disable            |
| Channel3                   | Disable            |
| Channel4                   | Disable            |
| Combined Channels          | Disable            |
| Use ETR as Clearing Source | N/A                |
| XOR Activation             | N/A                |
| One Pulse Mode             | No                 |

###### Configuration

<table>
    <thead>
    	<tr>
        	<th>Clé</th>
            <th>Valeur</th>
        </tr>
    </thead>
    <tbody>
    	<tr>
        	<td style="text-align: center" colspan="2">Counter Settings</td>
        </tr>
        <tr>
        	<td>Prescaler (PSC - 16bits)</td>
            <td>0</td>
        </tr>
        <tr>
        	<td>Counter Mode</td>
            <td>Up</td>
        </tr>
        <tr>
        	<td>Counter Period (AutoReload Register 32 bits)</td>
            <td style="font-weight: bolder;">1499</td>
        </tr>
        <tr>
            <td>Internal Clock Division (CKD)</td>
            <td>No Division</td>
        </tr>
        <tr>
            <td>auto-reload preload</td>
            <td>Disable</td>
        </tr>
            	<tr>
        	<td style="text-align: center" colspan="2">Trigger Output (TGRO) Parameters</td>
        </tr>
        <tr>
        	<td>Master/Slave Mode (MSM Bit)</td>
            <td>Disable (Trigger input Effect not deleyed)</td>
        </tr>
        <tr>
        	<td>Trigger Event Selection</td>
            <td style="font-weight: bolder;">Update Event</td>
        </tr>
    </tbody>
</table>

##### USART1

###### MODE

| Clé                           | Valeur           |
| ----------------------------- | ---------------- |
| Mode                          | **Asynchronous** |
| Hardware Flow Control (RS232) | Disable          |


###### Configuration

<table>
    <thead>
    	<tr>
        	<th>Clé</th>
            <th>Valeur</th>
        </tr>
    </thead>
    <tbody>
    	<tr>
        	<td style="text-align: center" colspan="2">Basic Parameters</td>
        </tr>
        <tr>
        	<td>Baud Rate</td>
            <td>115 200 Bits/s</td>
        </tr>
        <tr>
        	<td>Word Length</td>
            <td>8 bits (Including Parity)</td>
        </tr>
        <tr>
        	<td>Parity</td>
            <td >None</td>
        </tr>
        <tr>
            <td>Stop Bits</td>
            <td>1</td>
        </tr>
        <tr>
        	<td style="text-align: center" colspan="2">Advanced Parameters</td>
        </tr>
        <tr>
        	<td>Data Direction</td>
            <td>Receive and Transmit</td>
        </tr>
        <tr>
        	<td>Over Sampling</td>
            <td>16 Samples</td>
        </tr>
    </tbody>
</table>
##### SAI

###### MODE

| Clé        | Valeur     |
| ---------- | ---------- |
| SAI_A Mode | **Master** |
| SAI_B Mode | Disable    |


###### Configuration

<table>
    <thead>
    	<tr>
        	<th>Clé</th>
            <th>Valeur</th>
        </tr>
    </thead>
    <tbody>
    	<tr>
        	<td style="text-align: center" colspan="2">SAI A</td>
        </tr>
        <tr>
        	<td style="text-align: center" colspan="2">Basic Parameters</td>
        </tr>
        <tr>
        	<td>Protocol</td>
            <td>Free</td>
        </tr>
        <tr>
        	<td>Audio Mode</td>
            <td style="font-weight: bolder;">Master Receive</td>
        </tr>
        <tr>
        	<td>Frame Length (only Even Values)</td>
            <td style="font-weight: bolder;">64 Bits</td>
        </tr>
        <tr>
        	<td>Data Size</td>
            <td style="font-weight: bolder;">16 Bits</td>
        </tr>
        <tr>
            <td>Slot Size</td>
            <td>DataSize</td>
        </tr>
        <tr>
        	<td style="text-align: center" colspan="2">Frame Parameters</td>
        </tr>
        <tr>
            <td>First Bit</td>
            <td>MSB First</td>
        </tr>
        <tr>
        	<td>Frame Synchro Active Level Length</td>
            <td style="font-weight: bolder;">32</td>
        </tr>
        <tr>
        	<td>Frame Synchro Definition</td>
            <td style="font-weight: bolder;">Channel Identification</td>
        </tr>
        <tr>
        	<td>Frame Synchro Polarity</td>
            <td>Active Low</td>
        </tr>
        <tr>
        	<td>Frame Synchro Offset</td>
            <td>First Bit</td>
        </tr>
        <tr>
        	<td style="text-align: center" colspan="2">Slot Parameters</td>
        </tr>
        <tr>
        	<td>First Bit Offset</td>
            <td>0</td>
        </tr>
        <tr>
        	<td>Number of Slots (Only Even Values)</td>
            <td style="font-weight: bolder;">4</td>
        </tr>
        <tr>
        	<td>Slot Active Final Value</td>
            <td style="font-weight: lighter;">0x000FFFF</td>
        </tr>
        <tr>
        	<td>Slot Active </td>
            <td style="font-weight: bolder;">All</td>
        </tr>
        <tr>
        	<td style="text-align: center" colspan="2">Clock Parameters</td>
        </tr>
        <tr>
        	<td>Clock Source</td>
            <td style="font-weight: lighter;">SAI PLL Clock</td>
        </tr>
        <tr>
        	<td>Master Clock Divider</td>
            <td style="font-weight: bolder;">Disable</td>
        </tr>
        <tr>
        	<td>Real Audio Fequency</td>
            <td style="font-weight: lighter;">48.0 kHz</td>
        </tr>
        <tr>
        	<td>Error Between Selected</td>
            <td style="font-weight: lighter;">-65.5 %</td>
        </tr>
        <tr>
        	<td>Clock Strobing</td>
            <td style="font-weight: bolder;">Rizing Edge</td>
        </tr>
        <tr>
        	<td style="text-align: center" colspan="2">Advanced Parameters</td>
        </tr>
        <tr>
        	<td>FIFO Threshold</td>
            <td>Empty</td>
        </tr>
        <tr>
        	<td>Output Drive</td>
            <td>Disabled</td>
        </tr>
    </tbody>
</table>



###### DMA

| DMA Request | Stream       | Direction            | Priority | Mode         | Data Width    |
| ----------- | ------------ | -------------------- | -------- | ------------ | ------------- |
| **SAI1_A**  | DMA2 Stream1 | Peripheral to Memory | Low      | **Circular** | **Half Word** |



TODO CLock & + demo & other demo



## Exemple Chaine PCM&rarr;PDM&rarr;PCM

### Etape 1 : Génération d'un signal PCM

Dans cet exemple on va générer un signal pseudo-caré de fréquence 200Hz echantionné à 32kHz pendant 50 ms

```bash
# terminal located in repository root folder
virturalenv venv  # creating Python virtual environement
source venv/bin/activate
pip install -r requirements.txt # install Python modules required
python 01_PCM_PDM_Toolbox/pcm_generator.py generated.pcm -s 32000 -f0 200 -t .05 -c 1 0 .5 0 .25 0 .125
# Window showing genrated PCM signal
deactivate
```

On obtiendra la figure suivante traçant le signal dans le temps

![signal généré](00_Documentation/imgs/generated_pcm.png)

### Etape 2 : Convertion du signal PCM en PDM



```bash
# terminal located in repository root folder
source venv/bin/activate
python 01_PCM_PDM_Toolbox/pcm2pdm.py generated.pcm generated.pdm -f 64
# Window showing PCM and PDM Signals
deactivate
```

On obtiendra la figure suivante traçant les signaux PCM et PDM

![signaux PCM PDM](00_Documentation/imgs/generated_pcm_pdm.png)

### Etape 3 : Filtrage du signal PDM

Pour filtrer le signal PCM on utilise le prgramme C fourni dans le dossier [03_filtering](##03_filtering)

```bash
# terminal located in repository root folder
make -C 03_Filtering/build
./03_Filtering/build/pdm_filter -f 64 generated.pdm filtered.pcm

```

### Etape 4: Comparaison du PCM Original de celui obtenue par la chaine PCM&rarr;PDM&rarr;PCM

```bash
# terminal located in repository root folder
source venv/bin/activate
python 01_PCM_PDM_Toolbox/pcm2plot.py generated.pcm filtered.pcm
# Window showing PCM and PDM Signals
deactivate
```

On obtiens alors les figures suivantes:

![comparasion PCM](00_Documentation/imgs/pcm_comparisson.png)

## Etude de la reponse du filtre

TODO 



## Architecture Embarquée

TODO
