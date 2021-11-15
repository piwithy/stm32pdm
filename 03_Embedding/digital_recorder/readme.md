<h1> Digital Recorder </h1>

# Machine d'état
```mermaid
graph LR
    INIT(( ));
    WAITING_FOR_USB([WAITING_FOR_USB])
    IDLE([IDLE]);
    RECORDING([RECORDING]);
    INIT --> WAITING_FOR_USB;

    WAITING_FOR_USB -->|USB Disk Inserted| IDLE

    IDLE -->| USER_BTN Pressed | RECORDING;
    IDLE -->|USB_DISK_REMOVED| WAITING_FOR_USB

    RECORDING -->| USER_BTN Pressed | IDLE;
    RECORDING -->|USB_DISK_REMOVED| WAITING_FOR_USB




```
