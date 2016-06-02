/**
 * @file   gps_utils.h
 * @author Marcos Canales Mayo
 * @date   2 Jun 2016
 * @version 1.0
 */

struct gps_position{
	char *datetime;
	// latitude degrees
	float lat_deg;
	// latitude direction
	char lat_dir;
	// longitude degrees
	float lon_deg;
	// longitude direction
	char lon_dir;
};

/**
 *	@brief Tries to read the GPS position.
 *
 *	@param gps_position	Pointer to user's buffer to store the data.
 *	@param num_reads	Number of reads till read fails. If 0 then there will not be limit.
 *
 *	@return	Operation status: negative if error, otherwise success.
 */
int gps_try_get_position(struct gps_position *gps_position, int num_reads);