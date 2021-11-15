<h1> Parrot </h1>

Ce Programme lors d'un appui sur le bouton `USER` commence a enregistrer les sons ambitants puis lors d'un second appui il rejoue jusqu'a 3s sur le `DAC` un fois rejouer le son et ransmit au format WAVE sur une liaison série

<figure>
    <img src="../../00_Documentation/imgs/03_Embedding/parrot/block_diagram.svg" alt="Peripherals Bock Diagram">
<figcaption style="text-align:center"><em>Peripherals Bock Diagram</em></figcaption>
</figure>

Ce Programme implemente la machine de 4 états suivante:

# Machine d'état
```mermaid
graph TD
    INIT(( ));
    IDLE([IDLE]);
    RECORDING([RECORDING]);
    PLAYBACK([PLAYBACK])
    TRANSMIT([TRANSMIT])

    INIT --> IDLE;

    IDLE -->| USER_BTN Pressed | RECORDING;

    RECORDING -->| USER_BTN Pressed | PLAYBACK;

    PLAYBACK --> | Playback finished | TRANSMIT

    TRANSMIT -->| Sound Transmitted | IDLE;
```

# Algortihme du Programme

```mermaid
graph TD

    STATE_SWITCH{current_state}

    subgraph IDLE
        USER_BTN_IDLE{USER_BTN pressed?}
        TRANS_RECORDING[program_state = RECORDING]
        RECORDING_LED_ON[Switching LD3 ON]
        IDLING[idle_counter += 1]

    end

    subgraph PLAYBACK
    end

    subgraph RECORDING
    end

    subgraph TRANSMIT
        READY_LED_OFF[Switching LD4 OFF]
        UART_TRANSMIT[[Transmitting Sound Over UART]]
        READY_LED_ON[Switching LD4 ON]
        TRANSITION_IDLE[program_state = IDLE]
    end

    STATE_SWITCH -->|IDLE| USER_BTN_IDLE

    USER_BTN_IDLE --> |Yes| RECORDING_LED_ON --> TRANS_RECORDING --> STATE_SWITCH;
    USER_BTN_IDLE --> |No| IDLING --> STATE_SWITCH

    STATE_SWITCH -->|TRANSMIT|READY_LED_OFF --> UART_TRANSMIT --> READY_LED_ON --> TRANSITION_IDLE --> STATE_SWITCH



```
