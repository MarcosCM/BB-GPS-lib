/**
 * @file   gps.c
 * @author Marcos Canales Mayo
 * @date   7 May 2016
 * @version 1.0
 */

#define LOG // write to log

#include <fcntl.h>	// using low level functions (syscalls) to get more control over the char special file
#include <string.h>	// string functions
#include "gps.h"
#ifdef LOG
#include "gps_log.h"
#endif

#define DEVICE_PATH "/dev/ttyO4"

static int device_fd;

int gps_init(void){
	char *msg;
	#ifdef LOG
	gps_log_init();
	#endif
	// open device file
	device_fd = open(DEVICE_PATH, O_RDWR);
	if (device_fd<0){
		msg = "Could not open device";
		#ifdef LOG
		gps_log(msg, LOG_ERROR);
		#else
		printf("%s\n", msg);
		#endif
		return -1;
	}
	return 0;
}

int gps_read(char *buf){
	char frame[100], curr_char[10];
	int i = 0;

	// wait for the start of frame
	do{
		read(device_fd, curr_char, 1);
	} while(curr_char[0] != CHAR_START_OF_STC);
	frame[i] = curr_char[0];
	i++;

	// let n be the length of the frame
	// custom string concat: O(n)
	// strcat inside this loop would be slower: O(n^2)
	do{
		// completing the frame
		read(device_fd, curr_char, 1);
		frame[i] = curr_char[0];
		i++;
	} while(curr_char[0] != CHAR_END_OF_STC);
	frame[i] = '\0';

	#ifdef LOG
	gps_log(frame, LOG_INFO);
	#endif
	strcpy(buf, frame);
	return 0;
}

int gps_write(const char *buf){
	int len, res;
	len = strlen(buf);
	res = write(device_fd, buf, len);
	if (res<0 || res != len){
		#ifdef LOG
		gps_log("Could not send data to the GPS", LOG_ERROR);
		#endif
		return -1;
	}
	return res;
}

int gps_exit(void){
	#ifdef LOG
	gps_log_exit();
	#endif
	close(device_fd);
	return 0;
}