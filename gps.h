/**
 * @file   gps.h
 * @author Marcos Canales Mayo
 * @date   7 May 2016
 * @version 1.0
 */

#include "nmea.h"

struct latest_read_frame{
	char frame[100];
	int is_chk_valid;
};
/**
 *	@brief Lib init.
 *
 *	@return	Operation status: negative if error, otherwise success.
 */
int gps_init(void);
/**
 *	@brief Reads a single NMEA frame.
 *
 *	@param buf Pointer to user's buffer.
 *	@return	Operation status: negative if error, otherwise success.
 */
int gps_read(char *buf);
/**
 *	@brief Writes a single NMEA frame.
 *
 *	@param buf Pointer to user's buffer.
 *	@return	Operation status: negative if error, otherwise success.
 */
int gps_write(const char *buf);
/**
 *	@brief Changes the device terminal baud rate.
 *
 *	@param buf Baud rate: 4800, 9600, 19200, 38400, 57600 or 115200
 *	@return	Operation status: negative if error, otherwise success.
 */
int gps_change_baudrate(const char *str_baudrate);
/**
 *	@brief Lib exit.
 *
 *	@return	Operation status: negative if error, otherwise success.
 */
int gps_exit(void);