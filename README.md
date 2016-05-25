# BB-GPS-lib

## Hardware
Tested using:
* BeagleBone Rev A6b
* Fastrax IT500-150M-ITX-3297

However, it supports any MediaTek GPS module (NMEA frames that the GPS module **receives** depends on the manufacturer implementation).

## Software
Tested used Ångström distribution.

However, it supports any UNIX system.

## Requirements
* GPS device driver (UART driver)

## How it works
The GPS module is permanently sending NMEA frames through the UART pin, so the library uses a character special file that the device is mapped to, in order to read or send NMEA frames.
You can check out the [slides](https://es.slideshare.net/secret/amAhmfrAIkJb4t) to know more about this lib.

## Compiler options
These are the C compiler available options (defined macros):

``-D DEBUG``: the lib will write debug info to ``/var/log/gps.log``.

``-D ASYNC``: a pthread will be created to be constantly reading frames, so ``gps_read`` will return the latest read frame. If this option is not specified, then (by default) ``gps_read`` will start reading a new frame when it is called.

``-D DEVICE_PATH=\"/x/y\"``: path to the character special file that the GPS device is mapped to. By default it is ``/dev/ttyO4``.

``-D DEBUG_PATH=\"/x/y/z.log\"``: path to the log file. By default it is ``/var/log/gps.log``.
