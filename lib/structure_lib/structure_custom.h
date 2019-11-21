#ifndef STRUCT_H
#define STRUCT_H

#include <bson.h>

extern int verbose;
extern int receive_can_compact(int socket, int* id, int* data1, int*data2);

typedef struct {
	double max;
	double min;
	double average;
} bms_hv_temperature_value_data;

typedef struct {
	long timestamp;
	bms_hv_temperature_value_data value;
} bms_hv_temperature_data;

typedef struct {
	double max;
	double min;
	double total;
} bms_hv_voltage_value_data;

typedef struct {
	long timestamp;
	bms_hv_voltage_value_data value;
} bms_hv_voltage_data;

typedef struct {
	double current;
	double pow;
} bms_hv_current_value_data;

typedef struct {
	long timestamp;
	bms_hv_current_value_data value;
} bms_hv_current_data;

typedef struct {
	int fault_id;
	int fault_index;
} bms_hv_errors_value_data;

typedef struct {
	long timestamp;
	bms_hv_errors_value_data value;
} bms_hv_errors_data;

typedef struct {
	int fault_id;
	int fault_index;
} bms_hv_warnings_value_data;

typedef struct {
	long timestamp;
	bms_hv_warnings_value_data value;
} bms_hv_warnings_data;

typedef struct {
	bms_hv_temperature_data *temperature;
	int temperature_count;
	bms_hv_voltage_data *voltage;
	int voltage_count;
	bms_hv_current_data *current;
	int current_count;
	bms_hv_errors_data *errors;
	int errors_count;
	bms_hv_warnings_data *warnings;
	int warnings_count;
} bms_hv_data;

typedef struct {
	double voltage;
	double temperature;
} bms_lv_values_value_data;

typedef struct {
	long timestamp;
	bms_lv_values_value_data value;
} bms_lv_values_data;

typedef struct {
	long timestamp;
	int value;
} bms_lv_errors_data;

typedef struct {
	bms_lv_values_data *values;
	int values_count;
	bms_lv_errors_data *errors;
	int errors_count;
} bms_lv_data;

typedef struct {
	double latitude_ih;
	double latitude_il;
	double latitude_o;
	double speed;
} gps_latspd_value_data;

typedef struct {
	long timestamp;
	gps_latspd_value_data value;
} gps_latspd_data;

typedef struct {
	double longitude_ih;
	double longitude_il;
	double longitude_o;
	double altitude;
} gps_lonalt_value_data;

typedef struct {
	long timestamp;
	gps_lonalt_value_data value;
} gps_lonalt_data;

typedef struct {
	gps_latspd_data *latspd;
	int latspd_count;
	gps_lonalt_data *lonalt;
	int lonalt_count;
} gps_data;

typedef struct {
	double x;
	double y;
	double z;
	double scale;
} imu_gyro_value_data;

typedef struct {
	long timestamp;
	imu_gyro_value_data value;
} imu_gyro_data;

typedef struct {
	double x;
	double y;
	double z;
	double scale;
} imu_accel_value_data;

typedef struct {
	long timestamp;
	imu_accel_value_data value;
} imu_accel_data;

typedef struct {
	long timestamp;
	double value;
} front_wheels_encoder_data;

typedef struct {
	long timestamp;
	double value;
} steering_wheel_encoder_data;

typedef struct {
	double meters;
	double rotations;
	double angle;
	double clock_period;
} distance_value_data;

typedef struct {
	long timestamp;
	distance_value_data value;
} distance_data;

typedef struct {
	long timestamp;
	double value;
} throttle_data;

typedef struct {
	long timestamp;
	double value;
} brake_data;

typedef struct {
	int id;
	long timestamp;
	bms_hv_data bms_hv;
	bms_lv_data bms_lv;
	gps_data gps;
	imu_gyro_data *imu_gyro;
	int imu_gyro_count;
	imu_accel_data *imu_accel;
	int imu_accel_count;
	front_wheels_encoder_data *front_wheels_encoder;
	int front_wheels_encoder_count;
	steering_wheel_encoder_data *steering_wheel_encoder;
	int steering_wheel_encoder_count;
	distance_data *distance;
	int distance_count;
	throttle_data *throttle;
	int throttle_count;
	brake_data *brake;
	int brake_count;
	int marker;
} data_t;


data_t* data_setup();
int data_elaborate(data_t* data, bson_t** sending);
int data_quit(data_t* data);
int data_gather(data_t* data, int timing, int socket);

#endif