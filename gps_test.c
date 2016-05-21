/**
 * @file   gps_test.c
 * @author Marcos Canales Mayo
 * @date   7 May 2016
 * @version 1.0
 */

#include <stdio.h>
#include "gps.h"

#define ERR_READING					"Error while reading frame from the GPS, frame: %s"
#define ERR_WRITING					"Error while sending frame to the GPS, frame: %s"
#define WRITTEN_CMD_LOG_PATTERN		"Written command: %s"
#define READ_RESPONSE_LOG_PATTERN	"Read response: %s"

int main(int argc, char **argv){
	char buf[100];
	int i;
	struct nmea_frame nmea_frame;

	gps_init();


	/* Reading default GGA, GSA, GSV and RMC frames */
	for(i=0; i<8; i++){
		if (gps_read(buf) == -1) printf(ERR_READING, buf);
	}


	/* Reading all the fields of a single frame */
	if (gps_read(buf) == -1) printf(ERR_READING, buf);
	else{
		printf("%s\n", buf);
		nmea_frame_from_str(buf, &nmea_frame);
		printf("Frame type: %s\n", nmea_frame.frame_type);
		printf("Cmd type (can be blank): %s\n", nmea_frame.cmd_type);
		for(i=0; i<MAX_DATA_FIELDS; i++){
			printf("data[%d]: %s\n", i, nmea_frame.data[i]);
		}
		printf("Checksum: %s\n", nmea_frame.checksum);
	}


	/* Sending commands to the GPS */
	// TEST frame
	nmea_build_cmd(buf, CMD_TEST, (char *) NULL);
	if (gps_write(buf) == -1) printf(ERR_WRITING, buf);
	else printf(WRITTEN_CMD_LOG_PATTERN, buf);
	if (nmea_try_read_cmd_ack(buf, CMD_TEST, 10) == -1) printf(ERR_READING, buf);
	else printf(READ_RESPONSE_LOG_PATTERN, buf);


	/* Querying configuration data to GPS */
	// Position fix ctl
	nmea_build_cmd(buf, CMD_GET_FIX_CTL, (char *) NULL);
	if (gps_write(buf) == -1) printf(ERR_WRITING, buf);
	else printf(WRITTEN_CMD_LOG_PATTERN, buf);
	if (nmea_try_read_query_res(buf, CMD_GET_FIX_CTL, 10) == -1) printf(ERR_READING, buf);
	else printf(READ_RESPONSE_LOG_PATTERN, buf);

	// DGPS mode
	nmea_build_cmd(buf, CMD_GET_DGPS_MODE, (char *) NULL);
	if (gps_write(buf) == -1) printf(ERR_WRITING, buf);
	else printf(WRITTEN_CMD_LOG_PATTERN, buf);
	if (nmea_try_read_query_res(buf, CMD_GET_DGPS_MODE, 10) == -1) printf(ERR_READING, buf);
	else printf(READ_RESPONSE_LOG_PATTERN, buf);

	// SBAS enabled?
	nmea_build_cmd(buf, CMD_GET_SBAS, (char *) NULL);
	if (gps_write(buf) == -1) printf(ERR_WRITING, buf);
	else printf(WRITTEN_CMD_LOG_PATTERN, buf);
	if (nmea_try_read_query_res(buf, CMD_GET_SBAS, 10) == -1) printf(ERR_READING, buf);
	else printf(READ_RESPONSE_LOG_PATTERN, buf);

	// NMEA sentences output frequencies
	nmea_build_cmd(buf, CMD_GET_OUTPUT_FREQ, (char *) NULL);
	if (gps_write(buf) == -1) printf(ERR_WRITING, buf);
	else printf(WRITTEN_CMD_LOG_PATTERN, buf);
	if (nmea_try_read_query_res(buf, CMD_GET_OUTPUT_FREQ, 10) == -1) printf(ERR_READING, buf);
	else printf(READ_RESPONSE_LOG_PATTERN, buf);

	// Datum
	nmea_build_cmd(buf, CMD_GET_DATUM, (char *) NULL);
	if (gps_write(buf) == -1) printf(ERR_WRITING, buf);
	else printf(WRITTEN_CMD_LOG_PATTERN, buf);
	if (nmea_try_read_query_res(buf, CMD_GET_DATUM, 10) == -1) printf(ERR_READING, buf);
	else printf(READ_RESPONSE_LOG_PATTERN, buf);

	// User settings
	nmea_build_cmd(buf, CMD_GET_USER_OPTION, (char *) NULL);
	if (gps_write(buf) == -1) printf(ERR_WRITING, buf);
	else printf(WRITTEN_CMD_LOG_PATTERN, buf);
	if (nmea_try_read_query_res(buf, CMD_GET_USER_OPTION, 10) == -1) printf(ERR_READING, buf);
	else printf(READ_RESPONSE_LOG_PATTERN, buf);

	// Firmware info
	nmea_build_cmd(buf, CMD_GET_FW_INFO, (char *) NULL);
	if (gps_write(buf) == -1) printf(ERR_WRITING, buf);
	else printf(WRITTEN_CMD_LOG_PATTERN, buf);
	if (nmea_try_read_query_res(buf, CMD_GET_FW_INFO, 10) == -1) printf(ERR_READING, buf);
	else printf(READ_RESPONSE_LOG_PATTERN, buf);

	// EPO file status
	nmea_build_cmd(buf, CMD_GET_EPO_STATUS, (char *) NULL);
	if (gps_write(buf) == -1) printf(ERR_WRITING, buf);
	else printf(WRITTEN_CMD_LOG_PATTERN, buf);
	if (nmea_try_read_query_res(buf, CMD_GET_EPO_STATUS, 10) == -1) printf(ERR_READING, buf);
	else printf(READ_RESPONSE_LOG_PATTERN, buf);

	/* Reading more frames */
	for(i=0; i<20; i++){
		if (gps_read(buf) == -1) printf(ERR_READING, buf);
	}

	gps_exit();

	return 0;
}