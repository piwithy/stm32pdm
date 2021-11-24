<h1> Direct Output </h1>

Direct Output est un démonstrateur dont l’objectif est rejouer en temps réel les sons ambiants


# Schéma Block des Péripheriques
<figure>
    <img src="../../00_Documentation/imgs/03_Embedding/direct_output/block_diagram.svg" alt="Peripherals Bock Diagram">
<figcaption style="text-align:center"><em>Peripherals Bock Diagram</em></figcaption>
</figure>

# Pinout

<table>
 <thead>
  <tr>
   <th>PIN</th>
   <th>Signal</th>
   <th>Commentaire</th>
   <th>Périphérique</th>
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
  <td>PH0</td>
   <td>RCC_OSC_IN</td>
   <td style="vertical-align: middle;" rowspan="2">Signaux de l’oscillateur</td>
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
        <td>PA5</td>
        <td>DAC_OUT2</td>
        <td>Sortie du Canal 2 du DAC</td>
        <td style="text-align: center; vertical-align: middle;" rowspan="2">DAC</td>
  </tr>
  <tr>
        <td>PA4</td>
        <td>DAC_OUT1</td>
        <td>Sortie du Canal 1 du DAC</td>
  </tr>
 </tbody>
</table>

# Vue Système

<figure>
    <img src="../../00_Documentation/imgs/03_Embedding/direct_output/system_view.png" alt="System View">
<figcaption style="text-align:center"><em>System View</em></figcaption>
</figure>

# Horloges

<figure>
    <img src="../../00_Documentation/imgs/03_Embedding/direct_output/clocks.png" alt="Clocks">
<figcaption style="text-align:center"><em>Clocks</em></figcaption>
</figure>

### Horloges importantes

#### Schéma des horloges importantes

 ```mermaid
 graph LR
     INPUT(("Input Frequency"))
     HCLK(("HCLK"))
     SAI_A_CLK(("SAI-A Clock"))
     DIVIDE_M["/M"]
     MPLL_MULTIPLY_N[*N]
     MPLL_DIVIDE_P["/P"]
     AHB_PRESCALER["/AHB_Prescaler"]
     PLL_SAIQCLK["/PLL_SAIQCLK"]
     SPLL_MULTIPLY_N["*N"]
     SPLL_DIVIDE_Q["/Q"]


     subgraph Main PLL
         MPLL_MULTIPLY_N --> MPLL_DIVIDE_P;
     end;

     subgraph PLL SAI
        SPLL_MULTIPLY_N --> SPLL_DIVIDE_Q;
     end

     INPUT --> DIVIDE_M;
     DIVIDE_M --> MPLL_MULTIPLY_N;
     DIVIDE_M -->SPLL_MULTIPLY_N
     MPLL_DIVIDE_P --> AHB_PRESCALER;
     AHB_PRESCALER -->HCLK;

     SPLL_DIVIDE_Q --> PLL_SAIQCLK;
     PLL_SAIQCLK-->SAI_A_CLK;
 ```

 #### Configuration des horloges

 - HCLK

<table>
    <thead>
        <tr>
            <th>Variable</th><th>Bloc</th><th>Valeur</th><th>Opération</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>M</td><td>N/A</td><td>4</td><td>Division</td>
        </tr>
        <tr>
            <td>N</td><td>Main PLL</td><td>72</td><td>Multiplication</td>
        </tr>
        <tr>
            <td>P</td><td>Main PLL</td><td>2</td><td>Division</td>
        </tr>
        <tr>
            <td>AHB_PRESCALER</td><td>N/A</td><td>1</td><td>Division</td>
        </tr>
    </tbdoy>
</table>



 ```math
HCLK = \frac{\frac{\frac{Input Frequency}{M} * N}{P}}{AHB Prescaler} = \frac{\frac{\frac{8}{4} * 72}{2}}{1} *10^6 =  72*10^6 Hz
 ```

 - SAI-A Clock
 <table>
     <thead>
         <tr>
             <th>Variable</th><th>Bloc</th><th>Valeur</th><th>Opération</th>
         </tr>
     </thead>
     <tbody>
         <tr>
             <td>M</td><td>N/A</td><td>4</td><td>Division</td>
         </tr>
         <tr>
             <td>N</td><td>PLL SAI</td><td>192</td><td>Multiplication</td>
         </tr>
         <tr>
             <td>P</td><td>PLL SAI</td><td>5</td><td>Division</td>
         </tr>
         <tr>
             <td>PLL_SAIQCLK</td><td>N/A</td><td>25</td><td>Division</td>
         </tr>
     </tbdoy>
 </table>

```math
SAI A Clock = \frac{\frac{\frac{Input Frequency}{M} * N}{Q}}{PLL SAIQCLK} = \frac{\frac{\frac{8}{4} * 192}{5}}{25} *10^6 = 3.072 *10^6Hz
```

#### Valeurs des Horloges

 ```mermaid
 graph LR
 INPUT(("Input Frequency"))
 HCLK(("HCLK"))
 SAI_A_CLK(("SAI-A Clock"))
     DIVIDE_M["/4"]
     MPLL_MULTIPLY_N[*72]
     MPLL_DIVIDE_P["/2"]
     AHB_PRESCALER["/1"]
     PLL_SAIQCLK["/25"]
     SPLL_MULTIPLY_N["*128"]
     SPLL_DIVIDE_Q["/5"]


     subgraph Main PLL
         MPLL_MULTIPLY_N -->|"144 MHz"| MPLL_DIVIDE_P;
     end;

     subgraph PLL SAI
        SPLL_MULTIPLY_N -->|"384 MHz"| SPLL_DIVIDE_Q;
     end

     INPUT -->|"8 MHz"| DIVIDE_M;
     DIVIDE_M -->|"2 MHz"| MPLL_MULTIPLY_N;
     DIVIDE_M -->|"2 MHz"| SPLL_MULTIPLY_N
     MPLL_DIVIDE_P -->|"72 MHz"| AHB_PRESCALER;
     AHB_PRESCALER -->|"72 MHz"|HCLK;

     SPLL_DIVIDE_Q -->|"76.2 MHz"| PLL_SAIQCLK;
     PLL_SAIQCLK-->|"3.072 MHz"|SAI_A_CLK;
 ```

# Paramètres du filtre
| Paramètre                       | Nom             | Valeur    | Unité    |
|:--------------------------------|:---------------:|:---------:|:--------:|
| Fréquence du flux PDM           | $`f_{PDM}`$     | $`3.072`$ | $`MHz`$  |
| Facteur de sous échantillonnage | $`D`$           | $`64`$    |          |
| Fréquence d’échantillonnage PCM | $`fs_{PCM}`$    | $`48`$    | $`kHz`$  |
| Durée du Cycle SAI              | $`t_{SAI}`$     | $`1`$     | $`ms`$   |
| Fréquence système               | $`f_{system}`$  | $`72`$    | $`MHz`$  |
| Fréquence de coupure du filtre  | $`f_c`$         | $`16`$    | $`kHz`$  |
| Ordre du filtre                 | $`N`$           | $`16`$    |          |
| Taille des échantillons PCM     | $`n`$           | $`12`$    | $`bits`$ |

# Machine d’état
```mermaid
graph LR;
INIT(( ));
IDLE([IDLE]);
RECORDING([RECORDING]);
INIT --> IDLE;

IDLE -->| USER_BTN Pressed | RECORDING;

RECORDING -->| USER_BTN Pressed | IDLE;

```


## Logigramme des États
### IDLE

```mermaid
graph TD
    INIT(["Entrée dans l'état 'IDLE'"])
    FINISH(["Sortie de l'état 'IDLE'"])

    SAI_FLAG{"Drapeau SAI levé ?"};
    FILTER["Fitrage des échantillons PDM"];
    RESET_SAI_FLAG["Abaisser le Drapeau SAI"]

    DAC_FLAG_UP{"Drapeau DAC levé ?"}
    RESET_DAC_FLAG["Abaisser le Drapeau DAC"]

    USER_BTN_PRESSED{"Appui sur le bouton 'USER'?"};

    TIME_IN_STATE{"Temps passé dans l'état> 500 ms?"}

    NEXT_STATE["Prochain état: 'RECORDING'"] --> FINISH;

    INIT --> SAI_FLAG;
    SAI_FLAG -->|"Non"| DAC_FLAG_UP;
    SAI_FLAG -->|"Oui"| RESET_SAI_FLAG;
    RESET_SAI_FLAG -->FILTER;
    FILTER --> DAC_FLAG_UP;

    DAC_FLAG_UP -->|"Oui"| RESET_DAC_FLAG
    RESET_DAC_FLAG --> USER_BTN_PRESSED;
    DAC_FLAG_UP --> |"Non"| USER_BTN_PRESSED;

    USER_BTN_PRESSED -->|"Oui"| TIME_IN_STATE;
    TIME_IN_STATE --> |"Oui"| NEXT_STATE;

    USER_BTN_PRESSED -->|"Non"| SAI_FLAG;
    TIME_IN_STATE -->|"Non"| SAI_FLAG;
```

### RECORDING

```mermaid
graph TD
    INIT(["Entrée dans l'état 'RECORDING'"])
    FINISH(["Sortie de l'état 'RECORDING'"])

    SAI_FLAG{"Drapeau SAI levé ?"};
    FILTER["Fitrage des échantillons PDM"];
    RESET_SAI_FLAG["Abaisser le Drapeau SAI"]

    DAC_FLAG_UP{"Drapeau DAC levé ?"}
    RESET_DAC_FLAG["Abaisser le Drapeau DAC"];
    COPY_DAC["Copie du buffer PCM sur les buffers du DAC"];

    USER_BTN_PRESSED{"Appui sur le bouton 'USER'?"};

    TIME_IN_STATE{"Temps passé dans l'état> 500 ms?"}


    NEXT_STATE["Prochain état: 'IDLE'"] --> FINISH;

    INIT --> SAI_FLAG;
    SAI_FLAG -->|"Non"| DAC_FLAG_UP;
    SAI_FLAG -->|"Oui"| RESET_SAI_FLAG;
    RESET_SAI_FLAG -->FILTER;
    FILTER --> DAC_FLAG_UP;

    DAC_FLAG_UP -->|"Oui"| RESET_DAC_FLAG;
    RESET_DAC_FLAG --> COPY_DAC;
    COPY_DAC --> USER_BTN_PRESSED;
    DAC_FLAG_UP --> |"Non"| USER_BTN_PRESSED;

    USER_BTN_PRESSED -->|"Oui"| TIME_IN_STATE;
    TIME_IN_STATE --> |"Oui"| NEXT_STATE;

    USER_BTN_PRESSED -->|"Non"| SAI_FLAG;
    TIME_IN_STATE -->|"Non"| SAI_FLAG;
```
