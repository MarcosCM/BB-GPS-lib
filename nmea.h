/**
 * @file   nmea.h
 * @author Marcos Canales Mayo
 * @date   7 May 2016
 * @version 1.0
 */

// NMEA sentences
#define STC_STARTS_WITH		"$GP"
#define STC_ENDS_WITH		"\r\n"
#define CHAR_START_OF_STC	'$'
#define CHAR_END_OF_STC		'\n'
#define CHAR_END_OF_STC_2	'\r'
#define CHAR_START_OF_CHKS	'*'

// Propietary sentences
#define CMD_STARTS_WITH		"$PMTK"
#define CMD_ENDS_WITH		"\r\n"
#define CHAR_START_OF_CMD	'$'
#define CHAR_END_OF_CMD		'\n'
#define CHAR_END_OF_CMD_2	'\r'
#define CMD_TYPE_LENGTH		8

#define CMD_TEST			"000"
#define CMD_ACK				"001"
#define CMD_STARTUP_MSG		"010"
#define CMD_HOT_START		"101"
#define CMD_WARM_START		"102"
#define CMD_COLD_START		"103"
#define CMD_FULL_COLD_START	"104"
#define CMD_CLEAR_EPO_FILE	"127"
 
#define CMD_SET_BAUDRATE	"251"
#define CMD_SET_FIX_CTL		"300"
#define CMD_SET_DGPS_MODE	"301"
#define CMD_SET_SBAS		"313"
#define CMD_SET_OUTPUT		"314"
#define CMD_SET_DATUM		"330"
#define CMD_SET_USER_OPTION	"390"

#define CMD_GET_FIX_CTL		"400"
#define CMD_GET_DGPS_MODE	"401"
#define CMD_GET_SBAS		"413"
#define CMD_GET_OUTPUT_FREQ	"414"
#define CMD_GET_DATUM		"430"
#define CMD_GET_USER_OPTION	"490"
#define CMD_GET_FW_INFO		"605"
#define CMD_GET_EPO_STATUS	"607"

enum data_type {INTEGER, DOUBLE, CHAR, STRING};

struct nmea_frame{
	// GGA, GSA, GSV, RMC, PMTK...
	char frame_type[5];
	// if it is a CMD: 000, 001, 010...
	struct{
		int as_int;
		char as_str[4];
	} cmd_type;
	// data fields
	struct{
		enum data_type data_type;
		union{
			int i;
			double d;
			char c;
		} as_num;
		char as_str[15];
	} data[30];
	// A9, 3F...
	struct{
		unsigned char as_byte;
		char as_str[3];
	} checksum;
};

/**
 *	@brief Creates a nmea_frame data from a str containing a NMEA frame.
 *
 *	@param str		Pointer to NMEA frame.
 *	@param frame 	Pointer to user's buffer to store the data.
 *	@return Operation status: negative if error, otherwise success.
 */
int nmea_frame_from_str(const char *str, struct nmea_frame *frame);
/**
 *	@brief Calculates the checksum over a NMEA frame.
 *
 *	@param frame	Pointer to NMEA frame.
 *	@param buf 		Pointer to user's buffer to store the checksum.
 *	@return Operation status: negative if error, otherwise success.
 */
int nmea_checksum(const char *frame, char *buf);
/**
 *	@brief Builds a NMEA command.
 *
 *	@param buf 		Pointer to user's buffer.
 *	@param cmd_type Pointer to command data type.
 *	@param ...		Command data. The last param MUST be a NULL char pointer ((char *) NULL) to
 *					let the function know when the list ends.
 *	@return	Operation status: negative if error, otherwise success.
 */
int nmea_build_cmd(char *buf, const char *cmd_type, ...);