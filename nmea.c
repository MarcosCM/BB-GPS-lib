/**
 * @file   nmea.c
 * @author Marcos Canales Mayo
 * @date   9 May 2016
 * @version 1.0
 */

#include <regex.h>	// regular expressions
#include <stdarg.h>	// variadic functions
#include <stdio.h>	// sscanf, sprintf
#include <stdlib.h>	// strtol
#include <string.h>
#include <sys/types.h>	// size_t
#include "nmea.h"

int nmea_frame_from_str(const char *str, struct nmea_frame *frame){
	regex_t regexp;
	char matched_res[100];
	int i, j, data_field_idx, data_idx, len, res = -2;
	size_t max_groups = 50;
	regmatch_t groups[max_groups];
	int iaux;
	double daux;
	char *caux, data_field[30];

	if ((res = regcomp(&regexp, NMEA_FRAME_REGEXP, REG_EXTENDED)) != 0) return res;
	if ((res = regexec(&regexp, str, max_groups, groups, 0)) != 0) goto FREE_AND_EXIT;

	// groups[0] is the entire validated string
	i=1;

	// frame type
	len = groups[i].rm_eo - groups[i].rm_so;
	memcpy((*frame).frame_type, &str[groups[i].rm_so], len);
	(*frame).frame_type[len] = '\0';
	i++;

	// if second param is cmd type
	if (groups[i].rm_so != -1){
		len = groups[i].rm_eo - groups[i].rm_so;
		memcpy((*frame).cmd_type.as_str, &str[groups[i].rm_so], len);
		(*frame).cmd_type.as_int = (int) strtol(&str[groups[i].rm_so], &caux, 10);
		(*frame).frame_type[len] = '\0';
	}
	i++;

	// data fields
	data_idx = 0;
	data_field_idx = 0;
	for(j=groups[i].rm_so; j<groups[i].rm_eo; j++){
		if (str[j] == ',') continue;
		// append char to data field
		data_field[data_field_idx] = str[j];
		// if there are no more chars to append
		// then copy it to the nmea frame
		if (str[j + 1] == ','){
			data_field[data_field_idx + 1] = '\0';
			strcpy((*frame).data[data_idx].as_str, data_field);
			// reset data field and increment data pointer
			data_field_idx = 0;
			data_idx++;
		}
		else data_field_idx++;
	}
	i++;

	// checksum
	len = groups[i].rm_eo - groups[i].rm_so;
	memcpy((*frame).checksum.as_str, &str[groups[i].rm_so], len);
	(*frame).checksum.as_str[len] = '\0';
	sscanf((*frame).checksum.as_str, "0x%x", &(*frame).checksum.as_byte);

	FREE_AND_EXIT:
	regfree(&regexp);
	return 0;
}

int nmea_checksum(const char *frame, char *buf){
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