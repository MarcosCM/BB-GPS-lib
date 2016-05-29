/**
 * @file   gps.c
 * @author Marcos Canales Mayo
 * @date   7 May 2016
 * @version 1.0
 */

#ifdef ASYNC
#include <pthread.h>
#include <semaphore.h>
#endif
#include <termios.h>	// changing terminal options (baud rate, etc)
#include <fcntl.h>		// using low level functions (syscalls) to get more control over the char special file
#include <string.h>		// string functions
#include "gps.h"
#ifdef DEBUG
#include "gps_log.h"
#endif

#include <stdio.h>

#ifndef DEVICE_PATH
#define DEVICE_PATH "/dev/ttyO4"
#endif

static int device_fd;
static struct termios term_opts;

#ifdef ASYNC
static struct latest_read_frame latest_read_frame;
static pthread_t reader_th;
// reading/writing the frame is a CS
static pthread_mutex_t frame_mutex;
// a binary semaphore is needed for the flags (in order to sync)
static sem_t reader_exit;
static sem_t is_frame_rdy;

void *reader_thread(void *param){
	char frame[100], curr_char, valid_chks[5], frame_chks[5];
	int i = 0, j, frame_chks_idx = -1, is_chk_valid, is_frame_rdy_num;
	// while exit flag is not raised
	while(sem_trywait(&reader_exit) != 0){
		// reset values
		i = 0; frame_chks_idx = -1;
		// wait for the start of frame
		do{
			read(device_fd, &curr_char, 1);
		} while(curr_char != CHAR_START_OF_STC);
		frame[i] = curr_char;
		i++;

		// let n be the length of the frame
		// custom string concat: O(n)
		// strcat inside this loop would be slower: O(n^2)
		do{
			// completing the frame
			read(device_fd, &curr_char, 1);
			frame[i] = curr_char;

			// save the checksum index to later check whether it is a valid frame
			if (curr_char == CHAR_START_OF_CHKS) frame_chks_idx = i;

			i++;
		} while(curr_char != CHAR_END_OF_STC);
		frame[i] = '\0';

		// if the frame has no checksum then read the next frame
		if (frame_chks_idx == -1) continue;
		// if the frame has checksum then check whether it is valid
		// frame checksum: always a two hex digits code
		for(j=1; j<3; j++){
			frame_chks[j-1] = frame[frame_chks_idx+j];
		}
		frame_chks[j-1] = '\0';
		// valid checksum
		frame[frame_chks_idx] = '\0';
		nmea_checksum(frame, valid_chks);
		frame[frame_chks_idx] = CHAR_START_OF_CHKS;

		if (strcasecmp(frame_chks, valid_chks) == 0) is_chk_valid = 1;
		else is_chk_valid = 0;

		// write the frame: CS
		pthread_mutex_lock(&frame_mutex);
		strcpy(latest_read_frame.frame, frame);
		latest_read_frame.is_chk_valid = is_chk_valid;
		pthread_mutex_unlock(&frame_mutex);
		// raise the frame rdy flag, if it was already raised nothing will change
		// it is needed to check the sem value because it is a binary sem
		sem_getvalue(&is_frame_rdy, &is_frame_rdy_num);
		if (is_frame_rdy_num < 1) sem_post(&is_frame_rdy);
	}
	// the exit flag has been raised and the thread will terminate
	pthread_exit(NULL);
	return NULL;
}
#endif

int gps_init(void){
#ifdef DEBUG
	if (gps_log_init() < 0) return -1;
#endif
	// open device file
	device_fd = open(DEVICE_PATH, O_RDWR);
	if (device_fd<0){
#ifdef DEBUG
		gps_log("Could not open device", DEBUG_ERROR);
#else
		printf("%s\n", "Could not open device");
#endif
		return -1;
	}

	tcgetattr(device_fd, &term_opts);
#ifdef ASYNC
	sem_init(&is_frame_rdy, 0, 0);
	sem_init(&reader_exit, 0, 0);
	pthread_mutex_init(&frame_mutex, NULL);
	// create the frame reader thread
	pthread_create(&reader_th, NULL, reader_thread, NULL);
#endif
	return 0;
}

int gps_read(char *buf){
#ifdef ASYNC
	int is_chk_valid;
	// wait for the frame rdy flag to be raised
	sem_wait(&is_frame_rdy);
	// read the frame: CS
	pthread_mutex_lock(&frame_mutex);
	strcpy(buf, latest_read_frame.frame);
	is_chk_valid = latest_read_frame.is_chk_valid;
	pthread_mutex_unlock(&frame_mutex);
#ifdef DEBUG
	gps_log(buf, DEBUG_INFO);
#endif
	if (is_chk_valid) return 0;
	else return -1;
#else
	char frame[100], curr_char, valid_chks[5], frame_chks[5];
	int i = 0, j, frame_chks_idx = -1;

	// wait for the start of frame
	do{
		read(device_fd, &curr_char, 1);
	} while(curr_char != CHAR_START_OF_STC);
	frame[i] = curr_char;
	i++;

	// let n be the length of the frame
	// custom string concat: O(n)
	// strcat inside this loop would be slower: O(n^2)
	do{
		// completing the frame
		read(device_fd, &curr_char, 1);
		frame[i] = curr_char;

		// save the checksum index to later check whether it is a valid frame
		if (curr_char == CHAR_START_OF_CHKS) frame_chks_idx = i;

		i++;
	} while(curr_char != CHAR_END_OF_STC);
	frame[i] = '\0';

#ifdef DEBUG
	gps_log(frame, DEBUG_INFO);
#endif
	// copy the frame to user's buffer
	strcpy(buf, frame);

	// if the frame has no checksum return error status
	if (frame_chks_idx == -1) return -1;
	// if the frame has checksum then check whether it is valid
	// frame checksum: always a two hex digits code
	for(j=1; j<3; j++){
		frame_chks[j-1] = frame[frame_chks_idx+j];
	}
	frame_chks[j-1] = '\0';
	// valid checksum
	frame[frame_chks_idx] = '\0';
	nmea_checksum(frame, valid_chks);

	if (strcasecmp(frame_chks, valid_chks) == 0) return 0;
	else return -1;
#endif
}

int gps_write(const char *buf){
	int len, res;
	len = strlen(buf);
	res = write(device_fd, buf, len);
	if (res<0 || res != len){
#ifdef DEBUG
		gps_log("Could not send data to the GPS", DEBUG_ERROR);
#endif
		return -1;
	}
	return res;
}

int gps_change_baudrate(const char *str_baudrate){
	speed_t baudrate;
	// default baudrate
	if(strcmp(str_baudrate, "0") == 0) baudrate = B9600;
	else if(strcmp(str_baudrate, "4800") == 0) baudrate = B4800;
	else if(strcmp(str_baudrate, "9600") == 0) baudrate = B9600;
	// 14400 is not supported by termios.h
	//else if(strcmp(str_baudrate, "14400")) baudrate = B14400;
	else if(strcmp(str_baudrate, "19200") == 0) baudrate = B19200;
	else if(strcmp(str_baudrate, "38400") == 0) baudrate = B38400;
	else if(strcmp(str_baudrate, "57600") == 0) baudrate = B57600;
	else if(strcmp(str_baudrate, "115200") == 0) baudrate = B115200;
	else return -1;

	if (cfsetispeed(&term_opts, baudrate) < 0
		|| cfsetospeed(&term_opts, baudrate) < 0
		|| tcflush(device_fd, TCIOFLUSH)) return -1;
	return tcsetattr(device_fd, TCSANOW, &term_opts);
}

int gps_exit(void){
#ifdef DEBUG
	gps_log_exit();
#endif
#ifdef ASYNC
	// raise flag to terminate the reader thread
	sem_post(&reader_exit);
	// wait the reader thread to terminate
	pthread_join(reader_th, NULL);
	// free memory
	pthread_mutex_destroy(&frame_mutex);
	sem_destroy(&reader_exit);
	sem_destroy(&is_frame_rdy);
#endif
	close(device_fd);
	return 0;
}