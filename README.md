# loadCellProbe

## Arduino Nano Code for loadcell automatic bed leveling for Julia Pro series of 3D Printers which uses cantilivered carriage design

### Usage:
> **IMPORTANT** Make sure you use the arduino library from https://github.com/FracktalWorks/HX711_ADC
* Set INVERT_Z_DIR so select in which Z Direction the loadcell amp should be giving output
* LOWERTHRESHOLD UPPERTHRESHOLD set the upper and lower thresholds for the amplifier to just trigger, and start blinking LED