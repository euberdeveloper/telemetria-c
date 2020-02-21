#include "structure_service.h" 

data_t* structureCreate() {
	data_t* data = (data_t*) malloc(sizeof(data_t));
	data->steering_wheel.marker = 0;
	// {{GENERATE_STRUCTURE_INITIALIZER_CODE}}
	return data;
}

void structureToBson(data_t *data, bson_t** bson_document) {
	//{{GENERATE_BSON_CODE}}
}

void structureDelete(data_t *data) {
	//{{GENERATE_STRUCTURE_DEALLOCATOR_CODE}}
}

void resetStructureId() {
    condition.structure.id = 0;
}

gather_code gatherStructure(data_t *document)
{
    gather_code outcome = GATHER_KEEP;

	document->id = condition.structure.id++;
	
	double msec = 0, end = 0;
	struct timespec tstart = {0, 0}, tend = {0, 0};
	clock_gettime(CLOCK_MONOTONIC, &tstart);
	end = ((double)tstart.tv_sec * 1000 + 1.0e-6 * tstart.tv_nsec);

	document->timestamp = end;

	int id, data_left, data_right;
	int lat_done = 0, lon_done = 0;

	do
	{
		id = 0;
        data_left = 0;
        data_right = 0;
		canRead(&id, &data_left, &data_right);

		struct timespec tmessage = {0, 0};
		clock_gettime(CLOCK_MONOTONIC, &tmessage);
		size_t message_timestamp = ((double) tmessage.tv_sec * 1000 + 1.0e-6 * tmessage.tv_nsec);

		int first_byte = ((data_left >> 24) & 255);

		switch (id)
		{

            case (INVERTER_RIGHT_ID):
                document->inverterLeft[document->inverterLeft_count].timestamp = message_timestamp;
                document->inverterLeft[document->inverterLeft_count].value.data_left = data_left;
                document->inverterLeft[document->inverterLeft_count++].value.data_right = data_right;
                break;

            case (INVERTER_LEFT_ID):
                document->inverterRight[document->inverterRight_count].timestamp = message_timestamp;
                document->inverterRight[document->inverterRight_count].value.data_left = data_left;
                document->inverterRight[document->inverterRight_count++].value.data_right = data_right;
                break;

            case (BMS_HV_ID):
                switch (first_byte)
                {
                case VOLTAGE_FB:
                    document->bms_hv.voltage[document->bms_hv.voltage_count].timestamp = message_timestamp;
                    document->bms_hv.voltage[document->bms_hv.voltage_count].value.total = (double)(data_left & 0x00FFFFFF) / 10000;
                    document->bms_hv.voltage[document->bms_hv.voltage_count].value.max = (double)((data_right >> 16) & 0x0000FFFF) / 10000;
                    document->bms_hv.voltage[document->bms_hv.voltage_count++].value.min = (double)(data_right & 0x0000FFFF) / 10000;
                    break;

                case TEMPERATURE_FB:
                    document->bms_hv.temperature[document->bms_hv.temperature_count].timestamp = message_timestamp;
                    document->bms_hv.temperature[document->bms_hv.temperature_count].value.average = ((data_left >> 8) & 0x0000FFFF) / 100;
                    document->bms_hv.temperature[document->bms_hv.temperature_count].value.max = (((data_left & 0x000000FF) * 256 + ((data_right >> 24) & 0x000000FF))) / 100;
                    document->bms_hv.temperature[document->bms_hv.temperature_count++].value.min = ((data_right >> 8) & 0x0000FFFF) / 100;
                    break;

                case CURRENT_FB:
                    document->bms_hv.current[document->bms_hv.current_count].timestamp = message_timestamp;
                    document->bms_hv.current[document->bms_hv.current_count].value.current = (double)((data_left >> 8) & 0x0000FFFF) / 10;
                    document->bms_hv.current[document->bms_hv.current_count++].value.pow = (double)((data_left & 0x000000FF) * 256 + ((data_right >> 24) & 0x000000FF));
                    break;

                case ERRORS_FB:
                    document->bms_hv.errors[document->bms_hv.errors_count].timestamp = message_timestamp;
                    document->bms_hv.errors[document->bms_hv.errors_count].value.fault_id = ((data_left >> 16) & 0x000000FF);
                    document->bms_hv.errors[document->bms_hv.errors_count++].value.fault_index = ((data_left >> 8) & 0x000000FF) / 10;
                    break;

                case WARNINGS_FB:
                    document->bms_hv.warnings[document->bms_hv.warnings_count].timestamp = message_timestamp;
                    document->bms_hv.warnings[document->bms_hv.warnings_count].value.fault_id = ((data_left >> 16) & 0x000000FF);
                    document->bms_hv.warnings[document->bms_hv.warnings_count++].value.fault_index = ((data_left >> 8) & 0x000000FF) / 10;
                    break;
                }
                break;

            case (PEDALS_ID):
                if (first_byte == THROTTLE_FB)
                {
                    document->throttle[document->throttle_count].timestamp = message_timestamp;
                    document->throttle[document->throttle_count].value = ((data_left >> 16) & 255);

                    document->throttle_count++;
                }
                else if (first_byte = BRAKE_FB)
                {
                    document->brake[document->brake_count].timestamp = message_timestamp;
                    document->brake[document->brake_count].value = ((data_left >> 16) & 255);

                    document->brake_count++;
                }
                break;

            case (IMU_SWE_ID):
                switch (first_byte)
                {
                case IMU_GYRO_FB:
                    document->imu_gyro[document->imu_gyro_count].timestamp = message_timestamp;
                    document->imu_gyro[document->imu_gyro_count].value.x = (double)((data_left >> 8) & 0x0000FFFF);
                    document->imu_gyro[document->imu_gyro_count].value.y = (double)((data_left & 0x000000FF) * 0xFF) + ((data_right >> 24) & 0x000000FF);
                    document->imu_gyro[document->imu_gyro_count].value.z = (double)((data_right >> 8) & 0x0000FFFF);
                    document->imu_gyro[document->imu_gyro_count].value.scale = ((data_right)&0x000000FF) * 10;

                    document->imu_gyro[document->imu_gyro_count].value.x /= 10.0;
                    document->imu_gyro[document->imu_gyro_count].value.y /= 10.0;
                    document->imu_gyro[document->imu_gyro_count].value.z /= 10.0;

                    document->imu_gyro[document->imu_gyro_count].value.x -= document->imu_gyro[document->imu_gyro_count].value.scale;
                    document->imu_gyro[document->imu_gyro_count].value.y -= document->imu_gyro[document->imu_gyro_count].value.scale;
                    document->imu_gyro[document->imu_gyro_count].value.z -= document->imu_gyro[document->imu_gyro_count].value.scale;

                    document->imu_gyro_count++;
                    break;

                case IMU_ACCEL_FB:
                    document->imu_accel[document->imu_accel_count].timestamp = message_timestamp;
                    document->imu_accel[document->imu_accel_count].value.x = (double)((data_left >> 8) & 0x0000FFFF);
                    document->imu_accel[document->imu_accel_count].value.y = (double)((data_left & 0x000000FF) * 0xFF) + ((data_right >> 24) & 0x000000FF);
                    document->imu_accel[document->imu_accel_count].value.z = (double)((data_right >> 8) & 0x0000FFFF);
                    document->imu_accel[document->imu_accel_count].value.scale = (data_right)&0x000000FF;

                    document->imu_accel[document->imu_accel_count].value.x /= 100.0;
                    document->imu_accel[document->imu_accel_count].value.y /= 100.0;
                    document->imu_accel[document->imu_accel_count].value.z /= 100.0;

                    document->imu_accel[document->imu_accel_count].value.x -= document->imu_accel[document->imu_accel_count].value.scale;
                    document->imu_accel[document->imu_accel_count].value.y -= document->imu_accel[document->imu_accel_count].value.scale;
                    document->imu_accel[document->imu_accel_count].value.z -= document->imu_accel[document->imu_accel_count].value.scale;

                    document->imu_accel_count++;
                    break;

                case SWE_FB:
                    document->steering_wheel.encoder[document->steering_wheel.encoder_count].timestamp = message_timestamp;
                    document->steering_wheel.encoder[document->steering_wheel.encoder_count++].value = ((data_left >> 16) & 255);
                    break;
                }
                break;

            case (GPS_FWE_ID):
                switch (first_byte)
                {
                case LATSPD_FB:
                    if (lat_done)
                    {
                        document->gps.old.location_count++;

                        document->gps.old.location[document->gps.old.location_count].timestamp = message_timestamp;
                        document->gps.old.location[document->gps.old.location_count].value.latitude_m = (double)(((((data_left >> 8) & 0x0000FFFF) << 8) * 10000) + (((data_left & 0x000000FF) * 0xFF) << 8) + ((data_right >> 24) & 0x000000FF)) / 10000.0;
                        document->gps.old.location[document->gps.old.location_count].value.latitude_o = (data_right >> 16) & 0x000000FF;
                        document->gps.old.location[document->gps.old.location_count].value.speed = data_right & 0x0000FFFF;

                        document->gps.old.location[document->gps.old.location_count].value.longitude_m = 0;
                        document->gps.old.location[document->gps.old.location_count].value.longitude_o = 0;
                        document->gps.old.location[document->gps.old.location_count].value.altitude = 0;

                        lat_done = 1;
                        lon_done = 0;
                    }
                    else if (lon_done)
                    {
                        if (abs(document->gps.old.location[document->gps.old.location_count].timestamp - message_timestamp) < 25)
                        {
                            document->gps.old.location[document->gps.old.location_count].value.latitude_m = (double)(((((data_left >> 8) & 0x0000FFFF) << 8) * 10000) + (((data_left & 0x000000FF) * 0xFF) << 8) + ((data_right >> 24) & 0x000000FF)) / 10000.0;
                            document->gps.old.location[document->gps.old.location_count].value.latitude_o = (data_right >> 16) & 0x000000FF;
                            document->gps.old.location[document->gps.old.location_count].value.speed = data_right & 0x0000FFFF;

                            document->gps.old.location_count++;

                            lat_done = 0;
                            lon_done = 0;
                        }
                        else
                        {
                            document->gps.old.location_count++;

                            document->gps.old.location[document->gps.old.location_count].timestamp = message_timestamp;
                            document->gps.old.location[document->gps.old.location_count].value.latitude_m = (double)(((((data_left >> 8) & 0x0000FFFF) << 8) * 10000) + (((data_left & 0x000000FF) * 0xFF) << 8) + ((data_right >> 24) & 0x000000FF)) / 10000.0;
                            document->gps.old.location[document->gps.old.location_count].value.latitude_o = (data_right >> 16) & 0x000000FF;
                            document->gps.old.location[document->gps.old.location_count].value.speed = data_right & 0x0000FFFF;

                            document->gps.old.location[document->gps.old.location_count].value.longitude_m = 0;
                            document->gps.old.location[document->gps.old.location_count].value.longitude_o = 0;
                            document->gps.old.location[document->gps.old.location_count].value.altitude = 0;

                            lat_done = 1;
                            lon_done = 0;
                        }
                    }
                    else {
                        document->gps.old.location[document->gps.old.location_count].timestamp = message_timestamp;
                        document->gps.old.location[document->gps.old.location_count].value.latitude_m = (double)(((((data_left >> 8) & 0x0000FFFF) << 8) * 10000) + (((data_left & 0x000000FF) * 0xFF) << 8) + ((data_right >> 24) & 0x000000FF)) / 10000.0;
                        document->gps.old.location[document->gps.old.location_count].value.latitude_o = (data_right >> 16) & 0x000000FF;
                        document->gps.old.location[document->gps.old.location_count].value.speed = data_right & 0x0000FFFF;

                        document->gps.old.location[document->gps.old.location_count].value.longitude_m = 0;
                        document->gps.old.location[document->gps.old.location_count].value.longitude_o = 0;
                        document->gps.old.location[document->gps.old.location_count].value.altitude = 0;

                        lat_done = 1;
                        lon_done = 0;
                    }
                    break;

                case LONALT_FB:
                    if (lon_done)
                    {
                        document->gps.old.location_count++;

                        document->gps.old.location[document->gps.old.location_count].timestamp = message_timestamp;
                        document->gps.old.location[document->gps.old.location_count].value.longitude_m = (double)(((((data_left >> 8) & 0x0000FFFF) << 8) * 10000) + (((data_left & 0x000000FF) * 0xFF) << 8) + ((data_right >> 24) & 0x000000FF)) / 10000.0;
                        document->gps.old.location[document->gps.old.location_count].value.longitude_o = (data_right >> 16) & 0x000000FF;
                        document->gps.old.location[document->gps.old.location_count].value.altitude = data_right & 0x0000FFFF;

                        document->gps.old.location[document->gps.old.location_count].value.latitude_m = 0;
                        document->gps.old.location[document->gps.old.location_count].value.latitude_o = 0;
                        document->gps.old.location[document->gps.old.location_count].value.speed = 0;

                        lat_done = 0;
                        lon_done = 1;
                    }
                    else if (lat_done)
                    {
                        if (abs(document->gps.old.location[document->gps.old.location_count].timestamp - message_timestamp) < 25)
                        {
                            document->gps.old.location[document->gps.old.location_count].value.longitude_m = (double)(((((data_left >> 8) & 0x0000FFFF) << 8) * 10000) + (((data_left & 0x000000FF) * 0xFF) << 8) + ((data_right >> 24) & 0x000000FF)) / 10000.0;
                            document->gps.old.location[document->gps.old.location_count].value.longitude_o = (data_right >> 16) & 0x000000FF;
                            document->gps.old.location[document->gps.old.location_count].value.altitude = data_right & 0x0000FFFF;

                            document->gps.old.location_count++;

                            lat_done = 0;
                            lon_done = 0;
                        }
                        else
                        {
                            document->gps.old.location_count++;

                            document->gps.old.location[document->gps.old.location_count].timestamp = message_timestamp;
                            document->gps.old.location[document->gps.old.location_count].value.longitude_m = (double)(((((data_left >> 8) & 0x0000FFFF) << 8) * 10000) + (((data_left & 0x000000FF) * 0xFF) << 8) + ((data_right >> 24) & 0x000000FF)) / 10000.0;
                            document->gps.old.location[document->gps.old.location_count].value.longitude_o = (data_right >> 16) & 0x000000FF;
                            document->gps.old.location[document->gps.old.location_count].value.altitude = data_right & 0x0000FFFF;

                            document->gps.old.location[document->gps.old.location_count].value.latitude_m = 0;
                            document->gps.old.location[document->gps.old.location_count].value.latitude_o = 0;
                            document->gps.old.location[document->gps.old.location_count].value.speed = 0;

                            lon_done = 1;
                            lat_done = 0;
                        }
                    }
                    else {
                        document->gps.old.location[document->gps.old.location_count].timestamp = message_timestamp;
                        document->gps.old.location[document->gps.old.location_count].value.longitude_m = (double)(((((data_left >> 8) & 0x0000FFFF) << 8) * 10000) + (((data_left & 0x000000FF) * 0xFF) << 8) + ((data_right >> 24) & 0x000000FF)) / 10000.0;
                        document->gps.old.location[document->gps.old.location_count].value.longitude_o = (data_right >> 16) & 0x000000FF;
                        document->gps.old.location[document->gps.old.location_count].value.altitude = data_right & 0x0000FFFF;

                        document->gps.old.location[document->gps.old.location_count].value.latitude_m = 0;
                        document->gps.old.location[document->gps.old.location_count].value.latitude_o = 0;
                        document->gps.old.location[document->gps.old.location_count].value.speed = 0;

                        lon_done = 1;
                        lat_done = 0;
                    }
                    break;

                case TIME_FB:
                    document->gps.old.time[document->gps.old.time_count].timestamp = message_timestamp;
                    document->gps.old.time[document->gps.old.time_count].value.hours = ((((data_left >> 16) & 0x000000FF) - 48) * 10) + (((data_left >> 8) & 0x000000FF) - 48);
                    document->gps.old.time[document->gps.old.time_count].value.minutes = (((data_left & 0x000000FF) - 48) * 10) + (((data_right >> 24) & 0x000000FF) - 48);
                    document->gps.old.time[document->gps.old.time_count++].value.seconds = ((((data_right >> 16) & 0x000000FF) - 48) * 10) + (((data_right >> 8) & 0x000000FF) - 48);
                    break;

                case TTM_FB:
                    document->gps.old.true_track_mode[document->gps.old.true_track_mode_count].timestamp = message_timestamp;
                    document->gps.old.true_track_mode[document->gps.old.true_track_mode_count++].value = (data_left >> 8) & 0x0000FFFF;
                    break;

                case FRONT_WHEELS_FB:
                    document->front_wheels_encoder[document->front_wheels_encoder_count].timestamp = message_timestamp;
                    document->front_wheels_encoder[document->front_wheels_encoder_count].value.speed = ((data_left >> 8) & 0x0000FFFF) * ((data_left & 0x000000FF) == 0 ? 1 : -1);
                    document->front_wheels_encoder[document->front_wheels_encoder_count].value.speedms = (((data_right >> 16) & 0x0000FFFF) * ((data_left & 0x000000FF) == 0 ? 1 : -1)) / 100;

                    document->front_wheels_encoder_count++;
                    break;

                case DISTANCE_FB:
                    document->distance[document->distance_count].timestamp = message_timestamp;
                    document->distance[document->distance_count].value.meters = (data_left >> 8) & 0x0000FFFF;
                    document->distance[document->distance_count].value.rotations = ((data_left & 0x000000FF) * 0xFF) + ((data_right >> 24) & 0x000000FF);
                    document->distance[document->distance_count].value.angle = (data_right >> 16) & 0x000000F;
                    document->distance[document->distance_count++].value.clock_period = (data_right >> 8) & 0x000000F;
                    break;
                }
                break;

            case (BMS_LV_ID):
                document->bms_lv.values[document->bms_lv.values_count].timestamp = message_timestamp;
                document->bms_lv.values[document->bms_lv.values_count].value.voltage = (double)((data_left >> 24) & 255) / 10.0;
                document->bms_lv.values[document->bms_lv.values_count++].value.temperature = (double)((data_left >> 8) & 255) / 5.0;
                break;

            case (WHEEL_ID):
                if (first_byte == GEARS_FB)
                {
                    document->steering_wheel.gears[document->steering_wheel.gears_count].timestamp = message_timestamp;
                    document->steering_wheel.gears[document->steering_wheel.gears_count].value.control = (data_left >> 16) & 0xFF;
                    document->steering_wheel.gears[document->steering_wheel.gears_count].value.cooling = (data_left >> 8) & 0xFF;
                    document->steering_wheel.gears[document->steering_wheel.gears_count].value.map = (data_left)&0xFF;
                }
                else if (first_byte == MARKER_FB)
                {
                    document->steering_wheel.marker = 1;
                }
                else if (first_byte == SIGNAL_FB)
                {
                    int status = (data_left >> 16) & 0xFF;
                    int pilot_index = (data_left >> 8) & 0xFF;
                    int race_index = data_left & 0xFF;

                    switch (status) {
                        case 0:
                            outcome = GATHER_IDLE;
                            break;
                        case 1:
                            if (pilot_index >= condition.session.pilots_count) {
                                logWarning("Error in structure: invalid pilot from wheel");
                                outcome = GATHER_ERROR;
                            }
                            else if (race_index >= condition.session.races_count) {
                                logWarning("Error in structure: invalid race from wheel");
                                outcome = GATHER_ERROR;
                            }
                            else {
                                condition.session.selected_pilot = pilot_index;
                                condition.session.selected_race = race_index;
                                outcome = GATHER_ENABLE;
                            }
                            break;
                        default:
                            logWarning("Error in structure: invalid status from wheel");
                            outcome = GATHER_ERROR;
                            break;
                    }
                }
                break;
            }

        // Read GPS
        gps_struct* gps_data = readGPS();

		clock_gettime(CLOCK_MONOTONIC, &tend);
		msec = (((double)tend.tv_sec * 1000 + 1.0e-6 * tend.tv_nsec) - end);
	} while (msec < condition.structure.sending_rate);

	return outcome;
}