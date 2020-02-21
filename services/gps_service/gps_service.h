#ifndef TELEMETRY_GPS_SERVICE
#define TELEMETRY_GPS_SERVICE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include "../../state_machine/state_machine_condition.h"
#include "../../utils/log_utils/log_utils.h"

/* EXTERN */

extern condition_t condition;

typedef struct {
	double latitude;
	double longitude;
	double altitude;
	char* ns_indicator;
	char* ew_indicator;
	char* utc_time;
	int status;
} gps_gga_struct;

typedef struct {
	double latitude;
	double longitude;
	char* ns_indicator;
	char* ew_indicator;
	char* utc_time;
	int status;
} gps_gll_struct;

typedef struct {
	double ground_speed_knots;
	double ground_speed_human;
} gps_vtg_struct;

typedef struct {
	double latitude;
	double longitude;
	char* ns_indicator;
	char* ew_indicator;
	char* utc_time;
	char* date;
	int status;
	double ground_speed_knots;
} gps_rmc_struct;

typedef struct {
	gps_gga_struct *gga;
	gps_gll_struct *gll;
	gps_vtg_struct *vtg;
	gps_rmc_struct *rmc;
} gps_struct;

int openGPSPort(char *port);
gps_struct* readGPS();
void gpsFree(gps_struct* gps_data);

#endif


