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
	printf("\n%s\n", "TEST: reading ordinary frames");
	for(i=0; i<8; i++){
		if (gps_read(buf) == -1) printf(ERR_READING, buf);
		else printf("Read frame: %s", buf);
	}


	/* Reading all the fields of a single frame */
	printf("\n%s\n", "TEST: reading fields of nmea_frame");
	if (gps_read(buf) == -1) printf(ERR_READING, buf);
	else{
		printf("%s", buf);
		nmea_frame_from_str(buf, &nmea_frame);
		printf("Frame type: %s\n", nmea_frame.frame_type);
		printf("Cmd type (can be blank): %s\n", nmea_frame.cmd_type);
		for(i=0; i<MAX_DATA_FIELDS; i++){
			printf("data[%d]: %s\n", i, nmea_frame.data[i]);
		}
		printf("Checksum: %s\n", nmea_frame.checksum);
	}


	/* Querying configuration data to GPS */
	printf("\n%s\n", "TEST: sending queries to the GPS");
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


	/* Sending commands to the GPS */
	printf("\n%s\n", "TEST: sending commands to the GPS");
	// TEST frame
	nmea_build_cmd(buf, CMD_TEST, (char *) NULL);
	if (gps_write(buf) == -1) printf(ERR_WRITING, buf);
	else printf(WRITTEN_CMD_LOG_PATTERN, buf);
	if (nmea_try_read_cmd_ack(buf, CMD_TEST, 10) == -1) printf(ERR_READING, buf);
	else printf(READ_RESPONSE_LOG_PATTERN, buf);

	// Clear EPO file
	nmea_build_cmd(buf, CMD_CLEAR_EPO_FILE, "0", (char *) NULL);
	if (gps_write(buf) == -1) printf(ERR_WRITING, buf);
	else printf(WRITTEN_CMD_LOG_PATTERN, buf);
	if (nmea_try_read_cmd_ack(buf, CMD_CLEAR_EPO_FILE, 10) == -1) printf(ERR_READING, buf);
	else printf(READ_RESPONSE_LOG_PATTERN, buf);

	/* Set baudrate
	 * BE CAREFUL!! This may break the program, as it changes the actual baud rate of the device
	 */
	/*
	nmea_build_cmd(buf, CMD_SET_BAUDRATE, "0", (char *) NULL);
	if (gps_write(buf) == -1) printf(ERR_WRITING, buf);
	else printf(WRITTEN_CMD_LOG_PATTERN, buf);
	if (nmea_try_read_cmd_ack(buf, CMD_SET_BAUDRATE, 10) == -1) printf(ERR_READING, buf);
	else printf(READ_RESPONSE_LOG_PATTERN, buf);
	*/

	// Set fix control
	nmea_build_cmd(buf, CMD_SET_FIX_CTL, "1000", "0", "0", "0.0", "0.0", (char *) NULL);
	if (gps_write(buf) == -1) printf(ERR_WRITING, buf);
	else printf(WRITTEN_CMD_LOG_PATTERN, buf);
	if (nmea_try_read_cmd_ack(buf, CMD_SET_FIX_CTL, 10) == -1) printf(ERR_READING, buf);
	else printf(READ_RESPONSE_LOG_PATTERN, buf);

	// Set DGPS
	nmea_build_cmd(buf, CMD_SET_DGPS_MODE, "0", (char *) NULL);
	if (gps_write(buf) == -1) printf(ERR_WRITING, buf);
	else printf(WRITTEN_CMD_LOG_PATTERN, buf);
	if (nmea_try_read_cmd_ack(buf, CMD_SET_DGPS_MODE, 10) == -1) printf(ERR_READING, buf);
	else printf(READ_RESPONSE_LOG_PATTERN, buf);

	// Set SBAS
	nmea_build_cmd(buf, CMD_SET_SBAS, "0", (char *) NULL);
	if (gps_write(buf) == -1) printf(ERR_WRITING, buf);
	else printf(WRITTEN_CMD_LOG_PATTERN, buf);
	if (nmea_try_read_cmd_ack(buf, CMD_SET_SBAS, 10) == -1) printf(ERR_READING, buf);
	else printf(READ_RESPONSE_LOG_PATTERN, buf);

	/* Set frames output frequency:
	 * Values:
	 * 1=once every position fix, 2=every two position fixes, 3=every three,
	 * 4=every four, 5=every five
	 * Position of values:
	 * GLL, RMC, VTG, GGA, GSA, GSV, GRS, GST, MALM, MEPH, MDGP, MDBG, ZDA, MCHN
	 */
	nmea_build_cmd(buf, CMD_SET_OUTPUT, "0", "1", "0", "1", "1", "1", "0",
		"0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", (char *) NULL);
	if (gps_write(buf) == -1) printf(ERR_WRITING, buf);
	else printf(WRITTEN_CMD_LOG_PATTERN, buf);
	if (nmea_try_read_cmd_ack(buf, CMD_SET_OUTPUT, 10) == -1) printf(ERR_READING, buf);
	else printf(READ_RESPONSE_LOG_PATTERN, buf);

	// Set datum
	nmea_build_cmd(buf, CMD_SET_DATUM, "0", (char *) NULL);
	if (gps_write(buf) == -1) printf(ERR_WRITING, buf);
	else printf(WRITTEN_CMD_LOG_PATTERN, buf);
	if (nmea_try_read_cmd_ack(buf, CMD_SET_DATUM, 10) == -1) printf(ERR_READING, buf);
	else printf(READ_RESPONSE_LOG_PATTERN, buf);

	/* Override default settings (writing to the flash, but there's a max number of writes)
	 * Position of values:
	 * freeze_settings, update_rate, baud_rate, GLL_period, RMC_period, VTG_period, GSA_period, GSV_period,
	 * GGA_peiod, ZDA_period, MCHN_period, datum, DGPS_mode, RCM_baudrate
	 */
	/*
	nmea_build_cmd(buf, CMD_SET_USER_OPTION, "0", (char *) NULL);
	if (gps_write(buf) == -1) printf(ERR_WRITING, buf);
	else printf(WRITTEN_CMD_LOG_PATTERN, buf);
	if (nmea_try_read_cmd_ack(buf, CMD_SET_USER_OPTION, 10) == -1) printf(ERR_READING, buf);
	else printf(READ_RESPONSE_LOG_PATTERN, buf);
	*/

	gps_exit();

	return 0;
}