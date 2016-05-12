/**
 * @file   gps_test.c
 * @author Marcos Canales Mayo
 * @date   7 May 2016
 * @version 1.0
 */

#include <stdio.h>
#include "gps.h"
#include "nmea.h"

int main(int argc, char **argv){
	char buf[100];
	int i;

	gps_init();
	for(i=0; i<12; i++){
		if (gps_read(buf) == -1) printf("Read error, frame: %s\n", buf);
	}
	nmea_build_cmd(buf, CMD_TEST, (char *) NULL);
	gps_write(buf);
	for(i=0; i<12; i++){
		if (gps_read(buf) == -1) printf("Read error, frame: %s\n", buf);
	}
	gps_exit();

	return 0;
}