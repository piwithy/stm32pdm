<h1> Direct Output </h1>

Ce Programme quand il est dans l'état `RECORDING` capte les sons ambiants via un microphone PDM, traite le le signal et joue sur les deux cannaux du DAC les son capté en temps réel

<figure>
    <img src="../../00_Documentation/imgs/03_Embedding/direct_output/block_diagram.svg" alt="Peripherals Bock Diagram">
<figcaption style="text-align:center"><em>Peripherals Bock Diagram</em></figcaption>
</figure>

Ce Programme implemente la mache de 2 états suivante
```mermaid
graph LR
    INIT(( ));
    IDLE([IDLE]);
    RECORDING([RECORDING]);
    INIT --> IDLE;

    IDLE -->| USER_BTN Pressed | RECORDING;

    RECORDING -->| USER_BTN Pressed | IDLE;

```

La transition d'un état à l'autre se fait par un appui sur le bouton `USER` (bouton bleu sur la carte)



## Algoritme du Programme

Les Flags `dac_flag` et `sai_flag` sont mis a `0` par les fonctions executier quand le `DMA` atteind la moitier ou la fin d'un du buffer correspondant ( sai : `pdm_buffer`, dac : `dac_buffer_l`)


```mermaid
graph TB
    START([Begin])
    INIT[[Program Initialisation]]

    USER_BTN_PRESSED{USER_BTN pressed?}
    RECORDING_LED_OFF[Switching LD3 OFF]
    RECORDING_LED_ON[Switching LD3 ON]

    TRANSITION_IDLE[program_state = IDLE]
    TRANSITION_RECORDING[program_state = RECORDING]

    SWITCH{program_state?}

    IDLING([ ])


    CHECK_SAI{sai_flag == 0}
    RESET_SAI[sai_flag = 1]
    CHECK_DAC{dac_flag == 0}
    RESET_DAC[dac_flag = 1]



    FILTERING[[Filtering PDM Sample]]

    COPY_ON_DAC_LEFT[[Copying to_copy buffer in DAC channel 1 buffer]]
    COPY_ON_DAC_RIGHT[[Copying to_copy buffer sample on DAC channel 2 buffer]]

    START --> INIT;
    INIT --> CHECK_SAI;
    CHECK_SAI --> |true| RESET_SAI --> FILTERING --> CHECK_DAC;
    CHECK_SAI --> |false| CHECK_DAC;
    CHECK_DAC -->|True| RESET_DAC --> COPY_ON_DAC_LEFT --> COPY_ON_DAC_RIGHT --> USER_BTN_PRESSED;
    CHECK_DAC --> |false| USER_BTN_PRESSED

    USER_BTN_PRESSED -->|No| IDLING;
    USER_BTN_PRESSED -->|Yes| SWITCH;

    SWITCH -->|IDLE| RECORDING_LED_ON;
    SWITCH -->|RECORDING| RECORDING_LED_OFF;

    RECORDING_LED_OFF --> to_copy_void[to_copy = void_buff] -->TRANSITION_IDLE --> IDLING;
    RECORDING_LED_ON --> to_copy_pcm[to_copy = pcm_buffer]-->TRANSITION_RECORDING --> IDLING

    IDLING --> CHECK_SAI

```




Code de la machine d'états:

```c
uint32_t idle_counter = 0;
    uint32_t cool_down = 0;
    const uint16_t *to_copy = void_buff;
    HAL_GPIO_WritePin(GPIOG, LD4_Pin, GPIO_PIN_SET); // Ready LED
    while (1) {
        if(!sai_flag == 0){
            (void) pdm_fir_fif_ftl_chunk(&pdm_filter, pcm_buffer, pdm_buffer + sai_half * PDM_BUFFER_SIZE, PDM_BUFFER_SIZE);
            if(cool_down > 0) cool_down--;
            sai_flag=1;
        }
        if(dac_flag == 0){
            memcpy(dac_buffer_l + dac_half * PCM_BUFFER_SIZE, to_copy, sizeof(uitn16_t) * PCM_BUFFER_SIZE);
            memcpy(dac_buffer_r + dac_half * PCM_BUFFER_SIZE, to_copy, sizeof(uitn16_t) * PCM_BUFFER_SIZE);
        }

        switch (current_state) {
            case IDLE:
                if(HAL_GPIO_ReadPin(GPIOA, USER_BTN_Pin) == GPIO_PIN_SET && cool_down == 0){
                    current_state = RECORDING;
                    cool_down = 500;
                    HAL_GPIO_WritePin(GPIOG, LD3_Pin, GPIO_PIN_SET);
                    to_copy = pcm_buffer;
                }
                break;
            case RECORDING:
                if(HAL_GPIO_ReadPin(GPIOA, USER_BTN_Pin) == GPIO_PIN_SET && cool_down == 0){
                    current_state = IDLE;
                    cool_down = 500;
                    HAL_GPIO_WritePin(GPIOG, LD3_Pin, GPIO_PIN_RESET);
                    to_copy = void_buff;
                }
                break;

        }

    }


```
