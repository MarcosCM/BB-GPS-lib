/**
 * @file   gps_log.h
 * @author Marcos Canales Mayo
 * @date   7 May 2016
 * @version 1.0
 */

#include <fcntl.h>	// open, read, write, close syscalls
#include <stdio.h>	// printf...
#include <sys/stat.h>	// file permissions
#include <time.h>

#define LOG_PATH	"/var/log/gps"
#define LOG_PATTERN	"[%d-%d-%d %d:%d:%d] %s\n"

/**
 *	Log init
 */
int gps_log_init(void);
/**
 *	Writes to log
 */
int gps_log(const char *buf);
/**
 *	Log exit
 */
int gps_log_exit(void);