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
    int sending_imu_gyro;
    int sending_imu_axel;
    int sending_gps_data;
    int sending_bms_hv_data;
    int sending_bms_lv_data;
} config_t;

typedef struct {
	mongoc_uri_t *uri;
  	mongoc_client_t *client;
  	mongoc_database_t *database;
  	mongoc_collection_t *collection;
} dbhandler_t;

typedef struct {
  long timestamp;
  double value;
} basic_data;

typedef struct {
  long timestamp;
  double x;
  double y;
  double z;
} imu_axel_data;

typedef struct {
  imu_axel_data *imu_xy;
  int xy_position;

  imu_axel_data *imu_z;
  int z_position;
} imu_gyro_data;

typedef struct {
  long timestamp;
  double val1;
  double val2;
  double o;
} basic_gps_data;
  
typedef struct {
  basic_gps_data* latspd;
  int latspd_position;
  
  basic_gps_data* lonalt;
  int lonalt_position; 
} gps_data;


typedef struct {
  long timestamp;
  double total;
  double max;
  double min;
} basic_avg_data;

typedef struct {
  long timestamp;
  double current;
  double pow;
} basic_power_data;

typedef struct {
  long timestamp;
  int fault_id;
  int fault_index;
} fault_data;

typedef struct {
  basic_avg_data *voltage;
  int voltage_position;
  basic_avg_data *temperature;
  int temperature_position;

  basic_power_data *current;
  int current_position;

  fault_data *errors;
  int errors_position;
  fault_data *warnings;
  int warnings_position;
} bms_hv_data;

typedef struct {
  basic_data *temperature;
  int temperature_position;

  basic_data *voltage;
  int voltage_position;
} bms_lv_data;

typedef struct {
	uint32_t id;
	uint32_t timestamp;

	basic_data *front_wheels_encoder;
  int front_wheels_encoder_position;

	basic_data *throttle;
	int throttle_position;
  basic_data *brake;
  int brake_position;

	basic_data *steering_wheel_encoder;
  int steering_wheel_encoder_position;

  imu_gyro_data gyro;
  imu_axel_data *axel;
  int axel_position;

  gps_data gps;

  bms_hv_data bms_hv;
  bms_lv_data bms_lv;

  int marker;
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