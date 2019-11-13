#include "struct.h"

data_t *data_setup(config_t *cfg)
{
	data_t *data;
	data = (data_t *)malloc(sizeof(data_t));

	data->bms_hv.temperature = (bms_hv_temperature_data*)malloc(sizeof(bms_hv_temperature_data) * config->bms_hv_temperature_max_count);
	data->bms_hv.temperature_count = 0;
	data->bms_hv.voltage = (bms_hv_voltage_data*)malloc(sizeof(bms_hv_voltage_data) * config->bms_hv_voltage_max_count);
	data->bms_hv.voltage_count = 0;
	data->bms_hv.current = (bms_hv_current_data*)malloc(sizeof(bms_hv_current_data) * config->bms_hv_current_max_count);
	data->bms_hv.current_count = 0;
	data->bms_hv.errors = (bms_hv_errors_data*)malloc(sizeof(bms_hv_errors_data) * config->bms_hv_errors_max_count);
	data->bms_hv.errors_count = 0;
	data->bms_hv.warnings = (bms_hv_warnings_data*)malloc(sizeof(bms_hv_warnings_data) * config->bms_hv_warnings_max_count);
	data->bms_hv.warnings_count = 0;
	data->bms_lv.values = (bms_lv_values_data*)malloc(sizeof(bms_lv_values_data) * config->bms_lv_values_max_count);
	data->bms_lv.values_count = 0;
	data->bms_lv.errors = (bms_lv_errors_data*)malloc(sizeof(bms_lv_errors_data) * config->bms_lv_errors_max_count);
	data->bms_lv.errors_count = 0;
	data->gps.latspd = (gps_latspd_data*)malloc(sizeof(gps_latspd_data) * config->gps_latspd_max_count);
	data->gps.latspd_count = 0;
	data->gps.lonalt = (gps_lonalt_data*)malloc(sizeof(gps_lonalt_data) * config->gps_lonalt_max_count);
	data->gps.lonalt_count = 0;
	data->imu_gyro = (imu_gyro_data*)malloc(sizeof(imu_gyro_data) * config->imu_gyro_max_count);
	data->imu_gyro_count = 0;
	data->imu_axel = (imu_axel_data*)malloc(sizeof(imu_axel_data) * config->imu_axel_max_count);
	data->imu_axel_count = 0;
	data->front_wheels_encoder = (front_wheels_encoder_data*)malloc(sizeof(front_wheels_encoder_data) * config->front_wheels_encoder_max_count);
	data->front_wheels_encoder_count = 0;
	data->steering_wheel_encoder = (steering_wheel_encoder_data*)malloc(sizeof(steering_wheel_encoder_data) * config->steering_wheel_encoder_max_count);
	data->steering_wheel_encoder_count = 0;
	data->throttle = (throttle_data*)malloc(sizeof(throttle_data) * config->throttle_max_count);
	data->throttle_count = 0;
	data->brake = (brake_data*)malloc(sizeof(brake_data) * config->brake_max_count);
	data->brake_count = 0;
	
	
	return data;
}