# BB-GPS-lib

## Hardware
* BeagleBone Rev A6b
* Fastrax IT500-150M-ITX-3297

## Software
* Angstrom distribution

## How it works
The GPS module is permanently sending NMEA frames to the BeagleBone through the UART4 pin. It can also emit NMEA frames.
This lib uses `/dev/ttyO4`, which is the default character special file the device is mapped to, to get/send the NMEA frames.

## Compiler options
These are the C compiler available options (defined macros):

``-D DEBUG``: the lib will write debug info to ``/var/log/gps.log``.

``-D ASYNC``: a pthread will be created to be constantly reading frames, so ``gps_read`` will return the latest read frame. If this option is not specified, then (by default) ``gps_read`` will start reading a new frame when it is called.
