/**
 * @file   gps.h
 * @author Marcos Canales Mayo
 * @date   7 May 2016
 * @version 1.0
 */

#include "nmea.h"

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
 *	@brief Lib exit.
 *
 *	@return	Operation status: negative if error, otherwise success.
 */
int gps_exit(void);