# Overview
This project provides a schematic, a helper program and a script to program an Arduino via bluetooth on Linux.

The bootloader of the Arduino waits for a short time after resetting for a special command to end up in flash mode. Otherwise the previously flashed program will be executed.

# Circuit
The schematic describes how the HC-05 module and the Arduino should be wired for reliable resetting and flashing of the Arduino.
Power supply, voltage divider for the serial connection as well as the capacitor, that connects the state pin to the reset pin are similar to schematics that can be found on the internet. Apart from that I did two small modifications:
-There is an internal pull-up-resistor for the reset pin on the Arduino, which was too low rated to reliably reset the Arduino an every re-connection for me, thus I added an pull-down-resistor.
-The serial connection is not designed for multiple receivers, thus communication via USB will fail after connecting the bluetooth module. For me the USB connection works reliably when adding a resistor between the bluetooth module and the Arduino. When USB is disconnected communication and programming works with the bluetooth module.


# Compile reset program
To reset the Arduino without influencing any later communication, the serial port has to be opened without reading from it (like normal terminal programs would). Therefore the little helper program "arduino_reset" exists, which can be compiled by

```bash
gcc -Wall arduino_reset.cc -o arduino_reset
```

# Modified avrdude script

The modified avrdude script kills all programs that could prevent the serial port from closing (which is the required starting state). Afterwards it sets the parameters of the port that are required for the programming, resets the Arduino, waits for a short time and starts flashing.


# Todo

The modified script and the reset program aren't very generic. It is assumed, that the serial bluetooth port is /dev/rfcomm0 and that the original avrdude binary can be found at "/usr/share/arduino/hardware/tools/avr/bin/avrdude_orig"

# Remarks

Bluez newest version (as of July 2017) seems to have some problems with rfcomm or dropped the support for it.
Therefore on Archlinux there was the need to install '''bluez-utils-compat''' and to modify '''/etc/systemd/system/dbus-org.bluez.service''' to run '''ExecStart=/usr/lib/bluetooth/bluetoothd --compat'''
To connect to the HC-05 running as a server you need to bind pair (via gui or command line) and bind the rfcomm port (dummy MAC address):
'''sudo rfcomm bind hci0 98:D3:31:FB:0C:2F'''
afterwards you can connect for example with minicom and pipe the output to a file:
'''minicom -D /dev/rfcomm0  -C autopilot.log'''

To run an rfcomm server on linux you should check if there is a serial port already registered, and note its port:
```bash
sdptool browse local
```
if there is none add it:
```bash
sdptool add --channel=22 SP
```
and bind it:
```bash
sudo rfcomm listen /dev/rfcomm0 22
```
afterwards you can connect with rfcomm to it:
```bash
minicom -D /dev/rfcomm0
```



