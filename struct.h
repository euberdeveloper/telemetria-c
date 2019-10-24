#include <bson.h>
#include <mongoc.h>

#ifndef STRUCT_H
#define STRUCT_H

typedef struct {
 	char* broker_host;
  	int broker_port;
  	char* mqtt_topic;

  	char* mongo_host;
  	int mongo_port;
  	char* mongo_db;
  	char* mongo_collection;

  	int sending_time;

  	int sending_throttle;
  	int sending_brake;
  	int sending_steering_wheel_encoder;
  	int sending_front_wheels_encoder;
} config_t;

typedef struct {
	mongoc_uri_t *uri;
  	mongoc_client_t *client;
  	mongoc_database_t *database;
  	mongoc_collection_t *collection;
} dbhandler_t;

typedef struct {
	double avg;
	double sd;

	double sum;
	int count;
	double sumsquares;
} basic_avg_sd;


typedef struct {
	uint32_t id;
	uint32_t timestamp;

	basic_avg_sd *front_wheels_encoder;

	basic_avg_sd *throttle;
	basic_avg_sd *brake;

	basic_avg_sd *steering_wheel_encoder;
} data_t;

typedef struct {
	struct mosquitto *handler;

	char* broker_host;
  	int broker_port;
  	char* mqtt_topic;
} mosq_t;

#endif

/*
typedef struct {
    double *x;
    double *y;
    double *z;
} imu_tensor;

typedef struct {
    double latitude, longitude, speed, altitude;
    char lat_o, lon_o;
} gps_tensor;

typedef struct {
  double temp;
  double volt;
  double kwh;
} bms_hv;

typedef struct {
  double temp;
  double volt;
  double curr;
} inverter;

typedef struct {
  double temp;
  double volt;
} bms_lv;

  imu_tensor imu_gyro;
  imu_tensor imu_axel;
  
  gps_tensor gps;

  bms_hv bms_hv;
  inverter inv;
  bms_lv bms_lv;

*/