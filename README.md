# BB-GPS-lib

## Hardware
* BeagleBone Rev A6b
* Fastrax IT500-150M-ITX-3297

## Software
* Angstrom distribution

## How it works
The GPS module is permanently sending NMEA frames to the BeagleBone through the UART4 pin. It can also emit NMEA frames.
This lib uses `/dev/ttyO4`, which is the default character special file the device is mapped to, to get/send the NMEA frames.
