/**
 * @file   gps_test.c
 * @author Marcos Canales Mayo
 * @date   7 May 2016
 * @version 1.0
 */

#include <stdio.h>
#include "gps.h"

int main(int argc, char **argv){
	char buf[100];
	int i;
	struct nmea_frame nmea_frame;

	gps_init();
	gps_read(buf);
	printf("%s\n", buf);
	nmea_frame_from_str(buf, &nmea_frame);
	printf("%s\n", nmea_frame.frame_type);
	printf("%s\n", nmea_frame.cmd_type);
	for(i=0; i<MAX_DATA_FIELDS; i++){
		printf("data[%d]: %s\n", i, nmea_frame.data[i]);
	}
	/*// Reading default GGA, GSA, GSV and RMC frames
	for(i=0; i<8; i++){
		if (gps_read(buf) == -1) printf("Error while reading from the GPS, frame: %s\n", buf);
	}

	// Sending commands to the GPS
	// TEST frame
	nmea_build_cmd(buf, CMD_TEST, (char *) NULL);
	if (gps_write(buf) == -1) printf("Error while sending command to the GPS, frame: %s\n", buf);

	// Querying configuration data to GPS
	// Position fix ctl
	nmea_build_cmd(buf, CMD_GET_FIX_CTL, (char *) NULL);
	if (gps_write(buf) == -1) printf("Error while querying conf data to the GPS, frame: %s\n", buf);
	// DGPS mode
	nmea_build_cmd(buf, CMD_GET_DGPS_MODE, (char *) NULL);
	if (gps_write(buf) == -1) printf("Error while querying conf data to the GPS, frame: %s\n", buf);
	// SBAS enabled?
	nmea_build_cmd(buf, CMD_GET_SBAS, (char *) NULL);
	if (gps_write(buf) == -1) printf("Error while querying conf data to the GPS, frame: %s\n", buf);
	// NMEA sentences output frequencies
	nmea_build_cmd(buf, CMD_GET_OUTPUT_FREQ, (char *) NULL);
	if (gps_write(buf) == -1) printf("Error while querying conf data to the GPS, frame: %s\n", buf);
	// Datum
	nmea_build_cmd(buf, CMD_GET_DATUM, (char *) NULL);
	if (gps_write(buf) == -1) printf("Error while querying conf data to the GPS, frame: %s\n", buf);
	// User settings
	nmea_build_cmd(buf, CMD_GET_USER_OPTION, (char *) NULL);
	if (gps_write(buf) == -1) printf("Error while querying conf data to the GPS, frame: %s\n", buf);
	// Firmware info
	nmea_build_cmd(buf, CMD_GET_FW_INFO, (char *) NULL);
	if (gps_write(buf) == -1) printf("Error while querying conf data to the GPS, frame: %s\n", buf);
	// EPO file status
	nmea_build_cmd(buf, CMD_GET_EPO_STATUS, (char *) NULL);
	if (gps_write(buf) == -1) printf("Error while querying conf data to the GPS, frame: %s\n", buf);

	// Reading ACKs (returned data)
	for(i=0; i<20; i++){
		if (gps_read(buf) == -1) printf("Error while reading from the GPS, frame: %s\n", buf);
	}*/

	gps_exit();

	return 0;
}