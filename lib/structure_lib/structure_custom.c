#include <stdio.h>
#include <string.h>
#include "structure_custom.h"

data_t* data_setup() {
	data_t* data = (data_t*) malloc(sizeof(data_t));
	
	data->bms_hv.temperature = (bms_hv_temperature_data*)malloc(sizeof(bms_hv_temperature_data) * 500);
	data->bms_hv.temperature_count = 0;
	data->bms_hv.voltage = (bms_hv_voltage_data*)malloc(sizeof(bms_hv_voltage_data) * 500);
	data->bms_hv.voltage_count = 0;
	data->bms_hv.current = (bms_hv_current_data*)malloc(sizeof(bms_hv_current_data) * 500);
	data->bms_hv.current_count = 0;
	data->bms_hv.errors = (bms_hv_errors_data*)malloc(sizeof(bms_hv_errors_data) * 500);
	data->bms_hv.errors_count = 0;
	data->bms_hv.warnings = (bms_hv_warnings_data*)malloc(sizeof(bms_hv_warnings_data) * 500);
	data->bms_hv.warnings_count = 0;
	data->bms_lv.values = (bms_lv_values_data*)malloc(sizeof(bms_lv_values_data) * 500);
	data->bms_lv.values_count = 0;
	data->bms_lv.errors = (bms_lv_errors_data*)malloc(sizeof(bms_lv_errors_data) * 500);
	data->bms_lv.errors_count = 0;
	data->gps.latspd = (gps_latspd_data*)malloc(sizeof(gps_latspd_data) * 500);
	data->gps.latspd_count = 0;
	data->gps.lonalt = (gps_lonalt_data*)malloc(sizeof(gps_lonalt_data) * 500);
	data->gps.lonalt_count = 0;
	data->imu_gyro.xy = (imu_gyro_xy_data*)malloc(sizeof(imu_gyro_xy_data) * 500);
	data->imu_gyro.xy_count = 0;
	data->imu_gyro.z = (imu_gyro_z_data*)malloc(sizeof(imu_gyro_z_data) * 500);
	data->imu_gyro.z_count = 0;
	data->imu_axel = (imu_axel_data*)malloc(sizeof(imu_axel_data) * 500);
	data->imu_axel_count = 0;
	data->front_wheels_encoder = (front_wheels_encoder_data*)malloc(sizeof(front_wheels_encoder_data) * 500);
	data->front_wheels_encoder_count = 0;
	data->steering_wheel_encoder = (steering_wheel_encoder_data*)malloc(sizeof(steering_wheel_encoder_data) * 500);
	data->steering_wheel_encoder_count = 0;
	data->throttle = (throttle_data*)malloc(sizeof(throttle_data) * 500);
	data->throttle_count = 0;
	data->brake = (brake_data*)malloc(sizeof(brake_data) * 500);
	data->brake_count = 0;
	
	return data;
}

int data_elaborate(data_t* data, bson_t** sending) {
	*sending = bson_new();
	bson_t *children = (bson_t*)malloc(sizeof(bson_t) * 4);
	BSON_APPEND_INT32(*sending, "id", data->id);
	BSON_APPEND_INT64(*sending, "timestamp", data->timestamp);
	BSON_APPEND_DOCUMENT_BEGIN(*sending, "bms_hv", &children[0]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "temperature", &children[1]);
	for (int i = 0; i < (data->bms_hv.temperature_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->bms_hv.temperature[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "value", &children[3]);
		BSON_APPEND_DOUBLE(&children[3], "max", data->bms_hv.temperature[i].value.max);
		BSON_APPEND_DOUBLE(&children[3], "min", data->bms_hv.temperature[i].value.min);
		BSON_APPEND_DOUBLE(&children[3], "average", data->bms_hv.temperature[i].value.average);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "voltage", &children[1]);
	for (int i = 0; i < (data->bms_hv.voltage_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->bms_hv.voltage[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "value", &children[3]);
		BSON_APPEND_DOUBLE(&children[3], "max", data->bms_hv.voltage[i].value.max);
		BSON_APPEND_DOUBLE(&children[3], "min", data->bms_hv.voltage[i].value.min);
		BSON_APPEND_DOUBLE(&children[3], "total", data->bms_hv.voltage[i].value.total);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "current", &children[1]);
	for (int i = 0; i < (data->bms_hv.current_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->bms_hv.current[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "value", &children[3]);
		BSON_APPEND_DOUBLE(&children[3], "current", data->bms_hv.current[i].value.current);
		BSON_APPEND_DOUBLE(&children[3], "pow", data->bms_hv.current[i].value.pow);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "errors", &children[1]);
	for (int i = 0; i < (data->bms_hv.errors_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->bms_hv.errors[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "value", &children[3]);
		BSON_APPEND_INT32(&children[3], "fault_id", data->bms_hv.errors[i].value.fault_id);
		BSON_APPEND_INT32(&children[3], "fault_index", data->bms_hv.errors[i].value.fault_index);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "warnings", &children[1]);
	for (int i = 0; i < (data->bms_hv.warnings_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->bms_hv.warnings[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "value", &children[3]);
		BSON_APPEND_INT32(&children[3], "fault_id", data->bms_hv.warnings[i].value.fault_id);
		BSON_APPEND_INT32(&children[3], "fault_index", data->bms_hv.warnings[i].value.fault_index);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	bson_append_document_end(*sending, &children[0]);
	bson_destroy(&children[0]);
	BSON_APPEND_DOCUMENT_BEGIN(*sending, "bms_lv", &children[0]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "values", &children[1]);
	for (int i = 0; i < (data->bms_lv.values_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->bms_lv.values[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "value", &children[3]);
		BSON_APPEND_DOUBLE(&children[3], "voltage", data->bms_lv.values[i].value.voltage);
		BSON_APPEND_DOUBLE(&children[3], "temperature", data->bms_lv.values[i].value.temperature);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "errors", &children[1]);
	for (int i = 0; i < (data->bms_lv.errors_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->bms_lv.errors[i].timestamp);
		BSON_APPEND_INT32(&children[2], "value", data->bms_lv.errors[i].value);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	bson_append_document_end(*sending, &children[0]);
	bson_destroy(&children[0]);
	BSON_APPEND_DOCUMENT_BEGIN(*sending, "gps", &children[0]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "latspd", &children[1]);
	for (int i = 0; i < (data->gps.latspd_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->gps.latspd[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "value", &children[3]);
		BSON_APPEND_DOUBLE(&children[3], "latitude", data->gps.latspd[i].value.latitude);
		BSON_APPEND_DOUBLE(&children[3], "speed", data->gps.latspd[i].value.speed);
		BSON_APPEND_DOUBLE(&children[3], "lat_o", data->gps.latspd[i].value.lat_o);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "lonalt", &children[1]);
	for (int i = 0; i < (data->gps.lonalt_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->gps.lonalt[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "value", &children[3]);
		BSON_APPEND_DOUBLE(&children[3], "longitude", data->gps.lonalt[i].value.longitude);
		BSON_APPEND_DOUBLE(&children[3], "altitude", data->gps.lonalt[i].value.altitude);
		BSON_APPEND_DOUBLE(&children[3], "lon_o", data->gps.lonalt[i].value.lon_o);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	bson_append_document_end(*sending, &children[0]);
	bson_destroy(&children[0]);
	BSON_APPEND_DOCUMENT_BEGIN(*sending, "imu_gyro", &children[0]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "xy", &children[1]);
	for (int i = 0; i < (data->imu_gyro.xy_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->imu_gyro.xy[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "value", &children[3]);
		BSON_APPEND_DOUBLE(&children[3], "x", data->imu_gyro.xy[i].value.x);
		BSON_APPEND_DOUBLE(&children[3], "y", data->imu_gyro.xy[i].value.y);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "z", &children[1]);
	for (int i = 0; i < (data->imu_gyro.z_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->imu_gyro.z[i].timestamp);
		BSON_APPEND_DOUBLE(&children[2], "value", data->imu_gyro.z[i].value);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	bson_append_document_end(*sending, &children[0]);
	bson_destroy(&children[0]);
	BSON_APPEND_ARRAY_BEGIN(*sending, "imu_axel", &children[0]);
	for (int i = 0; i < (data->imu_axel_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[0], "0", &children[1]);
		BSON_APPEND_INT64(&children[1], "timestamp", data->imu_axel[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "value", &children[2]);
		BSON_APPEND_DOUBLE(&children[2], "x", data->imu_axel[i].value.x);
		BSON_APPEND_DOUBLE(&children[2], "y", data->imu_axel[i].value.y);
		BSON_APPEND_DOUBLE(&children[2], "z", data->imu_axel[i].value.z);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
		bson_append_document_end(&children[0], &children[1]);
		bson_destroy(&children[1]);
	}
	bson_append_array_end(*sending, &children[0]);
	bson_destroy(&children[0]);
	BSON_APPEND_ARRAY_BEGIN(*sending, "front_wheels_encoder", &children[0]);
	for (int i = 0; i < (data->front_wheels_encoder_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[0], "0", &children[1]);
		BSON_APPEND_INT64(&children[1], "timestamp", data->front_wheels_encoder[i].timestamp);
		BSON_APPEND_DOUBLE(&children[1], "value", data->front_wheels_encoder[i].value);
		bson_append_document_end(&children[0], &children[1]);
		bson_destroy(&children[1]);
	}
	bson_append_array_end(*sending, &children[0]);
	bson_destroy(&children[0]);
	BSON_APPEND_ARRAY_BEGIN(*sending, "steering_wheel_encoder", &children[0]);
	for (int i = 0; i < (data->steering_wheel_encoder_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[0], "0", &children[1]);
		BSON_APPEND_INT64(&children[1], "timestamp", data->steering_wheel_encoder[i].timestamp);
		BSON_APPEND_DOUBLE(&children[1], "value", data->steering_wheel_encoder[i].value);
		bson_append_document_end(&children[0], &children[1]);
		bson_destroy(&children[1]);
	}
	bson_append_array_end(*sending, &children[0]);
	bson_destroy(&children[0]);
	BSON_APPEND_ARRAY_BEGIN(*sending, "throttle", &children[0]);
	for (int i = 0; i < (data->throttle_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[0], "0", &children[1]);
		BSON_APPEND_INT64(&children[1], "timestamp", data->throttle[i].timestamp);
		BSON_APPEND_DOUBLE(&children[1], "value", data->throttle[i].value);
		bson_append_document_end(&children[0], &children[1]);
		bson_destroy(&children[1]);
	}
	bson_append_array_end(*sending, &children[0]);
	bson_destroy(&children[0]);
	BSON_APPEND_ARRAY_BEGIN(*sending, "brake", &children[0]);
	for (int i = 0; i < (data->brake_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[0], "0", &children[1]);
		BSON_APPEND_INT64(&children[1], "timestamp", data->brake[i].timestamp);
		BSON_APPEND_DOUBLE(&children[1], "value", data->brake[i].value);
		bson_append_document_end(&children[0], &children[1]);
		bson_destroy(&children[1]);
	}
	bson_append_array_end(*sending, &children[0]);
	bson_destroy(&children[0]);
	BSON_APPEND_INT32(*sending, "marker", data->marker);
	
	return 0;
}

int data_quit(data_t* data) {
	free(data->bms_hv.temperature);
	free(data->bms_hv.voltage);
	free(data->bms_hv.current);
	free(data->bms_hv.errors);
	free(data->bms_hv.warnings);
	free(data->bms_lv.values);
	free(data->bms_lv.errors);
	free(data->gps.latspd);
	free(data->gps.lonalt);
	free(data->imu_gyro.xy);
	free(data->imu_gyro.z);
	free(data->imu_axel);
	free(data->front_wheels_encoder);
	free(data->steering_wheel_encoder);
	free(data->throttle);
	free(data->brake);
	free(data);
	
	return 0;
}

int data_gather(data_t* data, int timing, int socket) {
	double msec = 0, end = 0;
	struct timespec tstart={0,0}, tend={0,0};
	clock_gettime(CLOCK_MONOTONIC, &tstart);
	end = ((double)tstart.tv_sec*1000 + 1.0e-6*tstart.tv_nsec);
	
	int id, data1, data2;

	do {
		id = data1 = data2 = 0;
		receive_can_compact(socket,&id,&data1,&data2);
	    
	    struct timespec tmessage={0,0};
	    clock_gettime(CLOCK_MONOTONIC, &tmessage);
		size_t message_timestamp = ((double)tmessage.tv_sec*1000 + 1.0e-6*tmessage.tv_nsec);

	    int firstByte = ((data1 >> 24) & 255);

	    switch(id) {        
			case(0xAA): //BMS HV
				switch(firstByte) {
					case 0x01: //temperature
						data->bms_hv.voltage[data->bms_hv.voltage_count].timestamp = message_timestamp;
						data->bms_hv.voltage[data->bms_hv.voltage_count].value.total = (data1 & 0x00FFFFFF)/10000;
						data->bms_hv.voltage[data->bms_hv.voltage_count].value.max = ((data2 >> 16) & 0x0000FFFF)/10000;
						data->bms_hv.voltage[data->bms_hv.voltage_count++].value.min = (data2 & 0x0000FFFF)/10000;
					break;

					case 0x0A: //voltage
						data->bms_hv.temperature[data->bms_hv.temperature_count].timestamp = message_timestamp;
						data->bms_hv.temperature[data->bms_hv.temperature_count].value.average = ((data1 >> 8) & 0x0000FFFF)/100;
						data->bms_hv.temperature[data->bms_hv.temperature_count].value.max = ((data1 & 0x000000FF)*256+((data2 >> 24) & 0x000000FF))/100;
						data->bms_hv.temperature[data->bms_hv.temperature_count++].value.min = ((data2 >> 16) & 0x0000FFFF)/100;
					break;

					case 0x05: //current
						data->bms_hv.current[data->bms_hv.current_count].timestamp = message_timestamp;
						data->bms_hv.current[data->bms_hv.current_count].value.current = ((data1 >> 8) & 0x0000FFFF)/10;
						data->bms_hv.current[data->bms_hv.current_count++].value.pow = ((data1 & 0x000000FF)*256+((data2 >> 24) & 0x000000FF));
					break;

					case 0x08: //errors
						data->bms_hv.errors[data->bms_hv.errors_count].timestamp = message_timestamp;
						data->bms_hv.errors[data->bms_hv.errors_count].value.fault_id = ((data1 >> 16) & 0x000000FF);
						data->bms_hv.errors[data->bms_hv.errors_count++].value.fault_index = ((data1 >> 8) & 0x000000FF)/10;
					break;

					case 0x09: //errors
						data->bms_hv.warnings[data->bms_hv.warnings_count].timestamp = message_timestamp;
						data->bms_hv.warnings[data->bms_hv.warnings_count].value.fault_id = ((data1 >> 16) & 0x000000FF);
						data->bms_hv.warnings[data->bms_hv.warnings_count++].value.fault_index = ((data1 >> 8) & 0x000000FF)/10;
					break;
				}
			break;

			case(0xB0): //Pedals
				if (firstByte == 0x01) {
					data->throttle[data->throttle_count].timestamp = message_timestamp;
					data->throttle[data->throttle_count++].value = ((data1 >> 16) & 255);
				} else if (firstByte = 0x02) {
					data->brake[data->brake_count].timestamp = message_timestamp;
					data->brake[data->brake_count++].value = ((data1 >> 16) & 255);
				}
			break;

			case (0xC0): //IMU and SWE
				switch (firstByte) {
					case 0x03: //XY
						//OK
						data->imu_gyro.xy[data->imu_gyro.xy_count].timestamp = message_timestamp;
						data->imu_gyro.xy[data->imu_gyro.xy_count].value.x = (data1 >> 8) & 0x0000FFFF;
						data->imu_gyro.xy[data->imu_gyro.xy_count++].value.y = (data2 >> 16) & 0x0000FFFF;
					break;

					case 0x04: //Z
						//OK
						data->imu_gyro.z[data->imu_gyro.z_count].timestamp = message_timestamp;
						data->imu_gyro.z[data->imu_gyro.z_count++].value = (data1 >> 8) & 0x0000FFFF;
					break;

					case 0x05: //axel
						//OK
						data->imu_axel[data->imu_axel_count].timestamp = message_timestamp;
						data->imu_axel[data->imu_axel_count].value.x = ((data1 >> 16) & 255) * 256 + ((data1 >> 8) & 255);
						data->imu_axel[data->imu_axel_count].value.y = ((data1) & 255) * 256 + ((data2 >> 24) & 255);
						data->imu_axel[data->imu_axel_count++].value.z = ((data2 >> 16) & 255) * 256 + ((data2 >> 8) & 255);
					break;
				
					case 0x02: //steering wheel
						data->steering_wheel_encoder[data->steering_wheel_encoder_count].timestamp = message_timestamp;
						data->steering_wheel_encoder[data->steering_wheel_encoder_count++].value = ((data1 >> 16) & 255);
					break;
				}
			break;

			case (0xD0): //GPS and FWE
				switch (firstByte) {
					case 0x01: //lat and speed
						//OK
						data->gps.latspd[data->gps.latspd_count].timestamp = message_timestamp;
						data->gps.latspd[data->gps.latspd_count].value.latitude  = (((data1 >> 16) & 255)*256 + ((data1 >> 8) & 255)) * 100000 + ((data1 & 255)*256+((data2 >> 24) & 255));
						data->gps.latspd[data->gps.latspd_count].value.speed = (((data2 >> 8) & 255) *256) + (data2 & 255);
						data->gps.latspd[data->gps.latspd_count++].value.lat_o = data2 >> 16 & 255;
					break;

					case 0x02: //lon and altitude
						//OK
						data->gps.lonalt[data->gps.lonalt_count].timestamp = message_timestamp;
						data->gps.lonalt[data->gps.lonalt_count].value.longitude  = (((data1 >> 16) & 255)*256 + ((data1 >> 8) & 255)) * 100000 + ((data1 & 255)*256+((data2 >> 24) & 255));
						data->gps.lonalt[data->gps.lonalt_count].value.altitude = (((data2 >> 8) & 255) *256) + (data2 & 255);
						data->gps.lonalt[data->gps.lonalt_count++].value.lon_o = data2 >> 16 & 255;
					break;

					case 0x06: //front wheels
						data->front_wheels_encoder[data->front_wheels_encoder_count].timestamp = message_timestamp;
						data->front_wheels_encoder[data->front_wheels_encoder_count++].value = ((data1 >> 16) & 255) *256 + ((data1 >> 8) & 255);
					break;
				}
			break;

			case (0xFF): //BMS LV
				//OK
				data->bms_lv.values[data->bms_lv.values_count].timestamp = message_timestamp;
				data->bms_lv.values[data->bms_lv.values_count].value.voltage = ((data1>>24) & 255)/10.0;
				data->bms_lv.values[data->bms_lv.values_count++].value.temperature = ((data1>>8) & 255)/5.0;
			break;

			case (0xAB): //Marker
				//OK
				data->marker = 1;
			break;
	    }

		clock_gettime(CLOCK_MONOTONIC, &tend);
		msec = (((double)tend.tv_sec*1000 + 1.0e-6*tend.tv_nsec) - end);
	} while ( msec < timing );
  return 0;

}

/* THIS WILL BE THE CODE WITH FENICE AND SPADA'S MESSAGES.H

	switch(id)
	{
		case 241:
			if (data->bms_hv.temperature_count < 500)
			{
				data->bms_hv.temperature[data->bms_hv.temperature_count].timestamp = timestamp;
				data->bms_hv.temperature[data->bms_hv.temperature_count].value.max = (byte[3] * 1)+ (byte[2] * 16);
				data->bms_hv.temperature[data->bms_hv.temperature_count].value.min = (byte[5] * 1)+ (byte[4] * 16);
				data->bms_hv.temperature[data->bms_hv.temperature_count].value.average = (byte[0] * 1);
				data->bms_hv.temperature_count++;
			}
			break;
	}
	

*/