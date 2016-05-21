/**
 * @file   nmea.c
 * @author Marcos Canales Mayo
 * @date   9 May 2016
 * @version 1.0
 */

#include <regex.h>	// regular expressions
#include <stdarg.h>	// variadic functions
#include <stdio.h>	// sscanf, sprintf
#include <string.h>
#include <sys/types.h>	// size_t
#include "nmea.h"

int nmea_frame_from_str(const char *str, struct nmea_frame *frame){
	regex_t regexp;
	char matched_res[100];
	int i, j, data_field_idx, data_idx, len, res = -2;
	size_t max_groups = 50;
	regmatch_t groups[max_groups];
	char data_field[30];

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
		memcpy((*frame).cmd_type, &str[groups[i].rm_so], len);
		(*frame).frame_type[len] = '\0';
	}
	else{
		(*frame).cmd_type[0] = '\0';
	}
	i++;

	// data fields
	data_idx = 0;
	data_field_idx = 0;
	// char at groups[i].rm_so is ',' so it will skip it
	for(j=groups[i].rm_so + 1; j<groups[i].rm_eo; j++){
		if (str[j] == CHAR_DATA_DELIMITER){
			data_field[data_field_idx] = '\0';
			strcpy((*frame).data[data_idx], data_field);
			// reset data field and increment data pointer
			data_field_idx = 0;
			data_field[0] = '\0';
			data_idx++;
		}
		else{
			// append char to data field
			data_field[data_field_idx] = str[j];
			data_field_idx++;
		}
	}
	// set the remaining data fields as blank
	for(; data_idx<MAX_DATA_FIELDS; data_idx++){
		(*frame).data[data_idx][0] = '\0';
	}
	i++;

	// checksum
	len = groups[i].rm_eo - groups[i].rm_so;
	memcpy((*frame).checksum, &str[groups[i].rm_so], len);
	(*frame).checksum[len] = '\0';

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
		buf_aux[0] = CHAR_DATA_DELIMITER;
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

int nmea_try_read_cmd_ack(char *buf, char *cmd_type, int num_reads){
	int i;
	char read_frame[100], *caux;
	struct nmea_frame nmea_frame;
	int needed_res_type;

	// response type is always 001 (ACK)
	needed_res_type = 1;

	if(num_reads>0){
		for(i=0; i<num_reads; i++){
			gps_read(read_frame);
			nmea_frame_from_str(read_frame, &nmea_frame);
			// check whether it is a sentence or a command response
			if (nmea_frame.cmd_type[0] != '\0'
					// check whether it is an ACK
					&& needed_res_type == (int) strtol(nmea_frame.cmd_type, &caux, 10)
					// first data field of ACK is the command type the user sent
					&& (int) strtol(cmd_type, &caux, 10) == (int) strtol(nmea_frame.data[0], &caux, 10)){
				strcpy(buf, read_frame);
				return 0;
			}
		}
	}
	else{
		while(1){
			gps_read(read_frame);
			nmea_frame_from_str(read_frame, &nmea_frame);
			// check whether it is a sentence or a command response
			if (nmea_frame.cmd_type[0] != '\0'
					// check whether it is an ACK
					&& needed_res_type == (int) strtol(nmea_frame.cmd_type, &caux, 10)
					// first data field of ACK is the command type the user sent
					&& (int) strtol(cmd_type, &caux, 10) == (int) strtol(nmea_frame.data[0], &caux, 10)){
				strcpy(buf, read_frame);
				return 0;
			}
		}
	}

	return -1;
}

int nmea_try_read_query_res(char *buf, char *query_type, int num_reads){
	int i;
	char read_frame[100], *caux;
	struct nmea_frame nmea_frame;
	int needed_res_type;

	// response type is always query command type plus 100
	needed_res_type = (int) strtol(query_type, &caux, 10);
	needed_res_type += 100;

	if(num_reads>0){
		for(i=0; i<num_reads; i++){
			gps_read(read_frame);
			nmea_frame_from_str(read_frame, &nmea_frame);
			// check whether it is a sentence or a command response
			if (nmea_frame.cmd_type[0] != '\0'
					// check whether it is a response to this kind of query
					&& needed_res_type == (int) strtol(nmea_frame.cmd_type, &caux, 10)){
				strcpy(buf, read_frame);
				return 0;
			}
		}
	}
	else{
		while(1){
			gps_read(read_frame);
			nmea_frame_from_str(read_frame, &nmea_frame);
			// check whether it is a sentence or a command response
			if (nmea_frame.cmd_type[0] != '\0'
					// check whether it is a response to this kind of query
					&& needed_res_type == (int) strtol(nmea_frame.cmd_type, &caux, 10)){
				strcpy(buf, read_frame);
				return 0;
			}
		}
	}

	return -1;
}