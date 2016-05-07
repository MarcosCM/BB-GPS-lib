/**
 * @file   gps_log.c
 * @author Marcos Canales Mayo
 * @date   7 May 2016
 * @version 1.0
 */

#include "gps_log.h"

static int log_fd;

static struct tm *get_datetime(){
	time_t rawtime = time(NULL);
	return localtime(&rawtime);
}

int gps_log_init(void){
	// open or create log file
	log_fd = open(LOG_PATH, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH );
	if (log_fd<0){
		printf("Could not open log file %s\nAborting...\n", LOG_PATH);
		return -1;
	}
	return 0;
}

int gps_log(const char *buf){
	struct tm tm;
	if (log_fd<0) return -1;
	tm = *get_datetime();
	dprintf(log_fd, LOG_PATTERN, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, buf);
	return 0;
}

int gps_log_exit(void){
	close(log_fd);
	return 0;
}