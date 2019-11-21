#include <stdio.h>
#include <string.h>
#include "structure_custom.h"

data_t* data_setup() {
	data_t* data = (data_t*) malloc(sizeof(data_t));
	
	// {{GENERATE_STRUCTURE_INITIALIZER_CODE}}
	return data;
}

int data_elaborate(data_t* data, bson_t** sending) {
	//{{GENERATE_BSON_CODE}}
	return 0;
}

int data_quit(data_t* data) {
	//{{GENERATE_STRUCTURE_DEALLOCATOR_CODE}}
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
					case 0x00: // IMU GYRO
						data->imu_gyro[data->imu_gyro_count].timestamp = message_timestamp;
						data->imu_gyro[data->imu_gyro_count].value.x = (data1 >> 8) & 0x0000FFFF;
						data->imu_gyro[data->imu_gyro_count].value.y = ((data1 & 0x000000FF) * 0xFF) + ((data2 >> 24) & 0x000000FF);
						data->imu_gyro[data->imu_gyro_count].value.z = (data2 >> 8) & 0x0000FFFF;
						data->imu_gyro[data->imu_gyro_count++].value.scale = ((data2 >> 24) & 0x000000FF) / 10;
					break;

					case 0x05: // IMU AXEL
						data->imu_accel[data->imu_accel_count].timestamp = message_timestamp;
						data->imu_accel[data->imu_accel_count].value.x = (data1 >> 8) & 0x0000FFFF;
						data->imu_accel[data->imu_accel_count].value.y = ((data1 & 0x000000FF) * 0xFF) + ((data2 >> 24) & 0x000000FF);
						data->imu_accel[data->imu_accel_count].value.z = (data2 >> 8) & 0x0000FFFF;
						data->imu_accel[data->imu_accel_count++].value.scale = (data2 >> 24) & 0x000000FF;
					break;
				
					case 0x02: // STEERING WHEEL ENCODER
						data->steering_wheel_encoder[data->steering_wheel_encoder_count].timestamp = message_timestamp;
						data->steering_wheel_encoder[data->steering_wheel_encoder_count++].value = ((data1 >> 16) & 255);
					break;
				}
			break;

			case (0xD0): //GPS and FWE
				switch (firstByte) {
					case 0x01: //lat and speed
						data->gps.latspd[data->gps.latspd_count].timestamp = message_timestamp;
						data->gps.latspd[data->gps.latspd_count].value.latitude_ih  = (data1 >> 8) & 0x0000FFFF;
						data->gps.latspd[data->gps.latspd_count].value.latitude_il = ((data1 & 0x000000FF) * 0xFF) + ((data2 >> 24) & 0x000000FF);
						data->gps.latspd[data->gps.latspd_count].value.latitude_o  = (data2 >> 16) & 0x000000FF;
						data->gps.latspd[data->gps.latspd_count++].value.speed = data2 & 0x0000FFFF;
					break;

					case 0x02: //lon and altitude
						data->gps.lonalt[data->gps.lonalt_count].timestamp = message_timestamp;
						data->gps.lonalt[data->gps.lonalt_count].value.longitude_ih  = (data1 >> 8) & 0x0000FFFF;
						data->gps.lonalt[data->gps.lonalt_count].value.longitude_il = ((data1 & 0x000000FF) * 0xFF) + ((data2 >> 24) & 0x000000FF);
						data->gps.lonalt[data->gps.lonalt_count].value.longitude_o  = (data2 >> 16) & 0x000000FF;
						data->gps.lonalt[data->gps.lonalt_count++].value.altitude = data2 & 0x0000FFFF;
					break;

					case 0x06: //front wheels
						data->front_wheels_encoder[data->front_wheels_encoder_count].timestamp = message_timestamp;
						data->front_wheels_encoder[data->front_wheels_encoder_count++].value = ((data1 >> 16) & 255) *256 + ((data1 >> 8) & 255);
					break;

					case 0x08: // DISTANCE
						data->distance[data->distance_count].timestamp = message_timestamp;
						data->distance[data->distance_count].value.meters = (data1 >> 8) & 0x0000FFFF;
						data->distance[data->distance_count].value.rotations = ((data1 & 0x000000FF) * 0xFF) + ((data2 >> 24) & 0x000000FF);
						data->distance[data->distance_count].value.angle = (data2 >> 16) & 0x000000F;
						data->distance[data->distance_count++].value.clock_period = (data2 >> 8) & 0x000000F;
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

	//{{GENERATE_PARSE_CAN_MESSAGE_CODE}}

*/