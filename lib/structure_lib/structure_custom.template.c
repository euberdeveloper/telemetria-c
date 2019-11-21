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

	//{{GENERATE_PARSE_CAN_MESSAGE_CODE}}

*/