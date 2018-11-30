# RDTech_PS_Comm
An Arduino example for controlling and communicating with Rui Deng Tech digital control power supplies like the DPS5020. The code provides examples for turning the supply output on and off, changing the supply voltage, and changing the current limit. It has been tested with the DPS5020 and DPS5005 supplies and seems as though the communication protocol is the same for most of Rui Deng's digital power supplies. The protocol for serial communication is described as a MODBUS protocol, though in practice the order of the two bytes in the checksum are swapped from the MODBUS examples I could find in my research. 

Future work todo:

Implement bidirectional communication. Right now, the code implements functions to send commands to the power supply module. The power supply module will respond with confirmations for some commands and it is possible to query some parameters from the module, but this code does not currently handle that.
