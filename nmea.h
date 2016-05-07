/**
 * @file   nmea.h
 * @author Marcos Canales Mayo
 * @date   7 May 2016
 * @version 1.0
 */

#define SENTENCE_STARTS_WITH	"$GP"
#define START_OF_SENTENCE	'$'
#define END_OF_SENTENCE	'\n'

#define COMMAND_STARTS_WITH	"$PMTK"
#define START_OF_COMMAND	'$'
#define END_OF_COMMAND		'\n'

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
#define CMD_GET_OUTPUT		"414"
#define CMD_GET_DATUM		"430"
#define CMD_GET_USER_OPTION	"490"
#define CMD_GET_FW_INFO		"605"
#define CMD_GET_EPO_STATUS	"607"