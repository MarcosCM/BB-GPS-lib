/**
 * @file   gps_log.h
 * @author Marcos Canales Mayo
 * @date   7 May 2016
 * @version 1.0
 */

#ifndef DEBUG_PATH
#define DEBUG_PATH		"/var/log/gps.log"
#endif
#define DEBUG_PATTERN	"[%d-%d-%d %d:%d:%d] %s %s\n"
#define DEBUG_INFO		"-INFO-"
#define DEBUG_WARNING	"-WARNING"
#define DEBUG_ERROR		"-ERROR-"

/**
 *	@brief Log init.
 *
 *	@return	Operation status: negative if error, otherwise success.
 */
int gps_log_init(void);
/**
 *	@brief Writes to log.
 *
 *	@param buf 		Pointer to user's buffer.
 *	@param level	Pointer to log level.
 *	@return	Operation status: negative if error, otherwise success.
 */
int gps_log(const char *buf, const char *level);
/**
 *	@brief Log exit.
 *
 *	@return	Operation status: negative if error, otherwise success.
 */
int gps_log_exit(void);