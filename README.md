# Midi Plus Modbus

Testing possibility of using multiple Arduino UNOs connected over RS485 to a single Arduino Leonardo that acts as a USB MIDI device. UNOs use SoftwareSerial at early stages of prototyping. When everything works as expected this will be changed to HardwareSerial, probably will need to unsolder the UART-USB chip...

Modified SimpleModbusSlave and SimpleModbusMaster were used

```
____________                  ____________
|           |                 |           |
|           |                 |           |
|   UNO  D11| ----- RS485 ----| 1  LEO    | 
|        D10| ----- RS485 ----| 0         |
|           |                 |           |          
|           |                 |           |          
|           |                 |           |          
 \________/^                   \________/^
```

All pins use external pull-ups, switches to GND

```
____________    
|           |   
|           |   
|   UNO   D8|---------- Bass Drum
|         D6|---------- Snare Drum
|         D4|---------- Snare Drum
|           |
|           |
 \________/^ 
```
