/**
 * @file   gps_log.c
 * @author Marcos Canales Mayo
 * @date   7 May 2016
 * @version 1.0
 */

#include <stdio.h>	// using high level lib to let it manage the regular file with buffers
#include <sys/stat.h>	// file permissions
#include <time.h>

#include "gps_log.h"

static FILE *log_file;

/**
 *	@brief Gets the current datetime.
 *
 *	@return Pointer to current datetime.
 */
static struct tm *get_datetime(){
	time_t rawtime = time(NULL);
	return localtime(&rawtime);
}

int gps_log_init(void){
	// open or create log file
	log_file = fopen(LOG_PATH, "a");
	if (log_file<0){
		printf("Could not open log file %s\nAborting...\n", LOG_PATH);
		return -1;
	}
	return 0;
}

int gps_log(const char *buf, const char *level){
	struct tm tm;
	if (log_file<0) return -1;
	tm = *get_datetime();
	fprintf(log_file, LOG_PATTERN,
		tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec,
		buf);
	return 0;
}

int gps_log_exit(void){
	fclose(log_file);
	return 0;
}