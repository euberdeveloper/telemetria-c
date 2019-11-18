#ifndef STRUCT_H
#define STRUCT_H

#include <bson.h>
#include <mongoc.h>

typedef struct
{
    char *broker_host;
    int broker_port;
    char *mqtt_topic;

    char *mongo_host;
    int mongo_port;
    char *mongo_db;
    char *mongo_collection;

    int sending_time;

    int throttle_max_count;
    int brake_max_count;
    int steering_wheel_encoder_max_count;
    int front_wheels_encoder_max_count;
    int imu_gyro_xy_max_count;
    int imu_gyro_z_max_count;
    int imu_axel_max_count;
    int gps_latspd_max_count;
    int gps_lonalt_max_count;
    int bms_hv_voltage_max_count;
    int bms_hv_temperature_max_count;
    int bms_hv_current_max_count;
    int bms_hv_errors_max_count;
    int bms_hv_warnings_max_count;
    int bms_lv_values_max_count;
    int bms_lv_errors_max_count;
} config_t;

typedef struct
{
    mongoc_uri_t *uri;
    mongoc_client_t *client;
    mongoc_database_t *database;
    mongoc_collection_t *collection;
} dbhandler_t;

typedef struct {
	struct mosquitto *handler;

	char* broker_host;
	int broker_port;
	char* mqtt_topic;
} mosq_t;

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
	int code;
	int index;
	int value;
} bms_hv_errors_value_data;

typedef struct {
	long timestamp;
	bms_hv_errors_value_data value;
} bms_hv_errors_data;

typedef struct {
	long timestamp;
	int value;
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
	double temperature_avg;
	double temperature_max;
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
	double latitude;
	double speed;
	double lat_o;
} gps_latspd_value_data;

typedef struct {
	long timestamp;
	gps_latspd_value_data value;
} gps_latspd_data;

typedef struct {
	double longitude;
	double altitude;
	double lon_o;
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
} imu_gyro_xy_value_data;

typedef struct {
	long timestamp;
	imu_gyro_xy_value_data value;
} imu_gyro_xy_data;

typedef struct {
	long timestamp;
	double value;
} imu_gyro_z_data;

typedef struct {
	imu_gyro_xy_data *xy;
	int xy_count;
	imu_gyro_z_data *z;
	int z_count;
} imu_gyro_data;

typedef struct {
	double x;
	double y;
	double z;
} imu_axel_value_data;

typedef struct {
	long timestamp;
	imu_axel_value_data value;
} imu_axel_data;

typedef struct {
	long timestamp;
	double value;
} front_wheels_encoder_data;

typedef struct {
	long timestamp;
	double value;
} steering_wheel_encoder_data;

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
	imu_gyro_data imu_gyro;
	imu_axel_data *imu_axel;
	int imu_axel_count;
	front_wheels_encoder_data *front_wheels_encoder;
	int front_wheels_encoder_count;
	steering_wheel_encoder_data *steering_wheel_encoder;
	int steering_wheel_encoder_count;
	throttle_data *throttle;
	int throttle_count;
	brake_data *brake;
	int brake_count;
	int marker;
} data_t;



#endif