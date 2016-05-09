/**
 * @file   nmea.c
 * @author Marcos Canales Mayo
 * @date   9 May 2016
 * @version 1.0
 */

#include <stdarg.h>	// variadic functions
#include <stdio.h>	// sscanf, sprintf
#include <string.h>
#include "nmea.h"

/**
 *	@brief Calculates the checksum over a NMEA frame.
 *
 *	@param frame	Pointer to NMEA frame.
 *	@param buf 		Pointer to user's buffer to store the checksum.
 *	@return Operation status: negative if error, otherwise success.
 */
static int nmea_checksum(const char *frame, char *buf){
	int i;
	unsigned char curr_char, checksum = 0x00;
	// omits first character $
	for(i=1; frame[i] != '\0'; i++){
		curr_char = frame[i];
		sscanf("%x", &curr_char);
		checksum ^= curr_char;
	}
	sprintf(buf, "%x", checksum);
	return 0;
}

int nmea_build_cmd(char *buf, const char *cmd_type, ...){
	va_list argsp;
	char cmd[100];
	char buf_aux[30];
	char checksum[5], *curr_arg;
	int i = CMD_TYPE_LENGTH, j;

	// append manufacturer starting and cmd_type
	strcpy(cmd, CMD_STARTS_WITH);
	strcat(cmd, cmd_type);

	// append list of parameters
	// avoids using strcat in favour of performance: O(n) vs O(n^2)
	va_start(argsp, cmd_type);
	BUILDING_CMD_DATA:
	curr_arg = va_arg(argsp, char *);
	if (curr_arg != NULL){
		buf_aux[0] = ',';
		strcpy(&buf_aux[1], curr_arg);
		for(j=0; buf_aux[j] != '\0'; j++){
			cmd[i] = buf_aux[j];
			i++;
		}
		goto BUILDING_CMD_DATA;
	}
	va_end(argsp);

	// append checksum
	cmd[i] = '\0';
	nmea_checksum(cmd, checksum);
	cmd[i] = '*';
	i++;
	cmd[i] = checksum[0];
	i++;
	cmd[i] = checksum[1];
	i++;
	cmd[i] = '\0';

	// append end of cmd
	strcat(cmd, CMD_ENDS_WITH);
	strcpy(buf, cmd);
	return 0;
}