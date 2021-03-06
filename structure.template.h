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

// {{GENERATE_STRUCTURE_CODE}}

#endif