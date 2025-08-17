# Midi Plus Modbus

Testing possibility of using multiple Arduino UNOs connected over RS485 to a single Arduino Leonardo that acts as a USB MIDI device. UNOs uses HardwareSerial at 1MHz, so don't connect with usb data cable.

Modified SimpleModbusSlave and SimpleModbusMaster were used

```
____________                  ____________
|           |                 |           |
|           |                 |           |
|   UNO  D0 | ----- RS485 ----| 1  LEO    | 
|        D1 | ----- RS485 ----| 0         |
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
