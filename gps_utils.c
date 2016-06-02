/**
 * @file   gps_utils.c
 * @author Marcos Canales Mayo
 * @date   2 Jun 2016
 * @version 1.0
 */

int gps_try_get_position(struct gps_position *gps_position, int num_reads){
	int i;
	char read_frame[100], *caux;
	struct nmea_frame nmea_frame;

	if(num_reads>0){
		for(i=0; i<num_reads; i++){
			// error checking after getting a frame (e.g. invalid checksum)
			if (gps_read(read_frame) < 0 || nmea_frame_from_str(read_frame, &nmea_frame) < 0) return -1;
			// check whether it is a GGA frame
			if (strcmp(nmea_frame.frame_type, "GPGGA") == 0){
				// datetime of the moment the position was taken
				strcpy((*gps_position).datetime, nmea_frame.data[0]);
				// latitude
				(*gps_position).lat_deg = strtof(nmea_frame.data[1], &caux);
				(*gps_position).lat_dir = nmea_frame.data[2][0];
				// longitude
				(*gps_position).lon_deg = strtof(nmea_frame.data[3], &caux);
				(*gps_position).lon_dir = nmea_frame.data[4][0];
				return 0;
			}
		}
	}
	else{
		while(1){
			// error checking after getting a frame (e.g. invalid checksum)
			if (gps_read(read_frame) < 0 || nmea_frame_from_str(read_frame, &nmea_frame) < 0) return -1;
			// check whether it is a GGA frame
			if (strcmp(nmea_frame.frame_type, "GPGGA") == 0){
				// datetime of the moment the position was taken
				strcpy((*gps_position).datetime, nmea_frame.data[0]);
				// latitude
				(*gps_position).lat_deg = strtof(nmea_frame.data[1], &caux);
				(*gps_position).lat_dir = nmea_frame.data[2][0];
				// longitude
				(*gps_position).lon_deg = strtof(nmea_frame.data[3], &caux);
				(*gps_position).lon_dir = nmea_frame.data[4][0];
				return 0;
			}
		}
	}

	return -1;
}