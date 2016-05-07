/**
 * @file   gps.h
 * @author Marcos Canales Mayo
 * @date   7 May 2016
 * @version 1.0
 */

#include "nmea.h"

/**
 *	Lib init
 */
int gps_init(void);
/**
 *	Reads a single NMEA frame
 */
int gps_read(char *buf);
/**
 *	Writes a single NMEA frame
 */
int gps_write(const char *buf);
/**
 *	Lib exit
 */
int gps_exit(void);