<h1> Direct Output </h1>

Ce Programme quand il est dans l'état `RECORDING` capte les sons ambiants via un microphone PDM, traite le le signal et joue sur les deux cannaux du DAC les son capté en temps réel

Peripheriques

<img src="../../00_Documentation/imgs/03_Embedding/direct_output/block_diagram.svg" alt="Block Diagram">

# Machine d'état
```mermaid
graph TD
    INIT(( ));
    IDLE([IDLE]);
    RECORDING([RECORDING]);
    INIT --> IDLE;

    IDLE -->| USER_BTN Pressed | RECORDING;

    RECORDING -->| USER_BTN Pressed | IDLE;

```
