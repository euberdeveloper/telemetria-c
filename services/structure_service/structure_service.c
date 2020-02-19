#include "structure_service.h" 

/* HELPERS SIGNATURES */

/* EXPORTED */

data_t* structureCreate() {
	data_t* data = (data_t*) malloc(sizeof(data_t));
	data->steering_wheel.marker = 0;
	data->inverterRight = (inverterRight_data*)malloc(sizeof(inverterRight_data) * 500);
	data->inverterRight_count = 0;
	data->inverterLeft = (inverterLeft_data*)malloc(sizeof(inverterLeft_data) * 500);
	data->inverterLeft_count = 0;
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
	data->gps.old.location = (gps_old_location_data*)malloc(sizeof(gps_old_location_data) * 500);
	data->gps.old.location_count = 0;
	data->gps.old.time = (gps_old_time_data*)malloc(sizeof(gps_old_time_data) * 500);
	data->gps.old.time_count = 0;
	data->gps.old.true_track_mode = (gps_old_true_track_mode_data*)malloc(sizeof(gps_old_true_track_mode_data) * 500);
	data->gps.old.true_track_mode_count = 0;
	data->imu_gyro = (imu_gyro_data*)malloc(sizeof(imu_gyro_data) * 500);
	data->imu_gyro_count = 0;
	data->imu_accel = (imu_accel_data*)malloc(sizeof(imu_accel_data) * 500);
	data->imu_accel_count = 0;
	data->front_wheels_encoder = (front_wheels_encoder_data*)malloc(sizeof(front_wheels_encoder_data) * 500);
	data->front_wheels_encoder_count = 0;
	data->distance = (distance_data*)malloc(sizeof(distance_data) * 500);
	data->distance_count = 0;
	data->throttle = (throttle_data*)malloc(sizeof(throttle_data) * 500);
	data->throttle_count = 0;
	data->brake = (brake_data*)malloc(sizeof(brake_data) * 500);
	data->brake_count = 0;
	data->steering_wheel.encoder = (steering_wheel_encoder_data*)malloc(sizeof(steering_wheel_encoder_data) * 500);
	data->steering_wheel.encoder_count = 0;
	data->steering_wheel.gears = (steering_wheel_gears_data*)malloc(sizeof(steering_wheel_gears_data) * 500);
	data->steering_wheel.gears_count = 0;
	
	return data;
}

void structureToBson(data_t *data, bson_t** bson_document) {
	*bson_document = bson_new();
	bson_t *children = (bson_t*)malloc(sizeof(bson_t) * 5);
	BSON_APPEND_INT32(*bson_document, "id", data->id);
	BSON_APPEND_INT64(*bson_document, "timestamp", data->timestamp);
	BSON_APPEND_UTF8(*bson_document, "sessionName", data->sessionName);
	BSON_APPEND_ARRAY_BEGIN(*bson_document, "inverterRight", &children[0]);
	for (int i = 0; i < (data->inverterRight_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[0], "0", &children[1]);
		BSON_APPEND_INT64(&children[1], "timestamp", data->inverterRight[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "value", &children[2]);
		BSON_APPEND_INT32(&children[2], "data_left", data->inverterRight[i].value.data_left);
		BSON_APPEND_INT32(&children[2], "data_right", data->inverterRight[i].value.data_right);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
		bson_append_document_end(&children[0], &children[1]);
		bson_destroy(&children[1]);
	}
	bson_append_array_end(*bson_document, &children[0]);
	bson_destroy(&children[0]);
	BSON_APPEND_ARRAY_BEGIN(*bson_document, "inverterLeft", &children[0]);
	for (int i = 0; i < (data->inverterLeft_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[0], "0", &children[1]);
		BSON_APPEND_INT64(&children[1], "timestamp", data->inverterLeft[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "value", &children[2]);
		BSON_APPEND_INT32(&children[2], "data_left", data->inverterLeft[i].value.data_left);
		BSON_APPEND_INT32(&children[2], "data_right", data->inverterLeft[i].value.data_right);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
		bson_append_document_end(&children[0], &children[1]);
		bson_destroy(&children[1]);
	}
	bson_append_array_end(*bson_document, &children[0]);
	bson_destroy(&children[0]);
	BSON_APPEND_DOCUMENT_BEGIN(*bson_document, "bms_hv", &children[0]);
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
	bson_append_document_end(*bson_document, &children[0]);
	bson_destroy(&children[0]);
	BSON_APPEND_DOCUMENT_BEGIN(*bson_document, "bms_lv", &children[0]);
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
	bson_append_document_end(*bson_document, &children[0]);
	bson_destroy(&children[0]);
	BSON_APPEND_DOCUMENT_BEGIN(*bson_document, "gps", &children[0]);
	BSON_APPEND_INT64(&children[0], "timestamp", data->gps.timestamp);
	BSON_APPEND_DOUBLE(&children[0], "latitude", data->gps.latitude);
	BSON_APPEND_DOUBLE(&children[0], "longitude", data->gps.longitude);
	BSON_APPEND_DOUBLE(&children[0], "altitude", data->gps.altitude);
	BSON_APPEND_INT32(&children[0], "ns_indicator", data->gps.ns_indicator);
	BSON_APPEND_INT32(&children[0], "ew_indicator", data->gps.ew_indicator);
	BSON_APPEND_DOCUMENT_BEGIN(&children[0], "old", &children[1]);
	BSON_APPEND_ARRAY_BEGIN(&children[1], "location", &children[2]);
	for (int i = 0; i < (data->gps.old.location_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "0", &children[3]);
		BSON_APPEND_INT64(&children[3], "timestamp", data->gps.old.location[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[3], "value", &children[4]);
		BSON_APPEND_DOUBLE(&children[4], "latitude_m", data->gps.old.location[i].value.latitude_m);
		BSON_APPEND_INT32(&children[4], "latitude_o", data->gps.old.location[i].value.latitude_o);
		BSON_APPEND_DOUBLE(&children[4], "longitude_m", data->gps.old.location[i].value.longitude_m);
		BSON_APPEND_INT32(&children[4], "longitude_o", data->gps.old.location[i].value.longitude_o);
		BSON_APPEND_DOUBLE(&children[4], "speed", data->gps.old.location[i].value.speed);
		BSON_APPEND_DOUBLE(&children[4], "altitude", data->gps.old.location[i].value.altitude);
		bson_append_document_end(&children[3], &children[4]);
		bson_destroy(&children[4]);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
	}
	bson_append_array_end(&children[1], &children[2]);
	bson_destroy(&children[2]);
	BSON_APPEND_ARRAY_BEGIN(&children[1], "time", &children[2]);
	for (int i = 0; i < (data->gps.old.time_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "0", &children[3]);
		BSON_APPEND_INT64(&children[3], "timestamp", data->gps.old.time[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[3], "value", &children[4]);
		BSON_APPEND_INT32(&children[4], "hours", data->gps.old.time[i].value.hours);
		BSON_APPEND_INT32(&children[4], "minutes", data->gps.old.time[i].value.minutes);
		BSON_APPEND_INT32(&children[4], "seconds", data->gps.old.time[i].value.seconds);
		bson_append_document_end(&children[3], &children[4]);
		bson_destroy(&children[4]);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
	}
	bson_append_array_end(&children[1], &children[2]);
	bson_destroy(&children[2]);
	BSON_APPEND_ARRAY_BEGIN(&children[1], "true_track_mode", &children[2]);
	for (int i = 0; i < (data->gps.old.true_track_mode_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "0", &children[3]);
		BSON_APPEND_INT64(&children[3], "timestamp", data->gps.old.true_track_mode[i].timestamp);
		BSON_APPEND_INT32(&children[3], "value", data->gps.old.true_track_mode[i].value);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
	}
	bson_append_array_end(&children[1], &children[2]);
	bson_destroy(&children[2]);
	bson_append_document_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	bson_append_document_end(*bson_document, &children[0]);
	bson_destroy(&children[0]);
	BSON_APPEND_ARRAY_BEGIN(*bson_document, "imu_gyro", &children[0]);
	for (int i = 0; i < (data->imu_gyro_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[0], "0", &children[1]);
		BSON_APPEND_INT64(&children[1], "timestamp", data->imu_gyro[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "value", &children[2]);
		BSON_APPEND_DOUBLE(&children[2], "x", data->imu_gyro[i].value.x);
		BSON_APPEND_DOUBLE(&children[2], "y", data->imu_gyro[i].value.y);
		BSON_APPEND_DOUBLE(&children[2], "z", data->imu_gyro[i].value.z);
		BSON_APPEND_DOUBLE(&children[2], "scale", data->imu_gyro[i].value.scale);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
		bson_append_document_end(&children[0], &children[1]);
		bson_destroy(&children[1]);
	}
	bson_append_array_end(*bson_document, &children[0]);
	bson_destroy(&children[0]);
	BSON_APPEND_ARRAY_BEGIN(*bson_document, "imu_accel", &children[0]);
	for (int i = 0; i < (data->imu_accel_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[0], "0", &children[1]);
		BSON_APPEND_INT64(&children[1], "timestamp", data->imu_accel[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "value", &children[2]);
		BSON_APPEND_DOUBLE(&children[2], "x", data->imu_accel[i].value.x);
		BSON_APPEND_DOUBLE(&children[2], "y", data->imu_accel[i].value.y);
		BSON_APPEND_DOUBLE(&children[2], "z", data->imu_accel[i].value.z);
		BSON_APPEND_DOUBLE(&children[2], "scale", data->imu_accel[i].value.scale);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
		bson_append_document_end(&children[0], &children[1]);
		bson_destroy(&children[1]);
	}
	bson_append_array_end(*bson_document, &children[0]);
	bson_destroy(&children[0]);
	BSON_APPEND_ARRAY_BEGIN(*bson_document, "front_wheels_encoder", &children[0]);
	for (int i = 0; i < (data->front_wheels_encoder_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[0], "0", &children[1]);
		BSON_APPEND_INT64(&children[1], "timestamp", data->front_wheels_encoder[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "value", &children[2]);
		BSON_APPEND_DOUBLE(&children[2], "speed", data->front_wheels_encoder[i].value.speed);
		BSON_APPEND_DOUBLE(&children[2], "speedms", data->front_wheels_encoder[i].value.speedms);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
		bson_append_document_end(&children[0], &children[1]);
		bson_destroy(&children[1]);
	}
	bson_append_array_end(*bson_document, &children[0]);
	bson_destroy(&children[0]);
	BSON_APPEND_ARRAY_BEGIN(*bson_document, "distance", &children[0]);
	for (int i = 0; i < (data->distance_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[0], "0", &children[1]);
		BSON_APPEND_INT64(&children[1], "timestamp", data->distance[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "value", &children[2]);
		BSON_APPEND_DOUBLE(&children[2], "meters", data->distance[i].value.meters);
		BSON_APPEND_DOUBLE(&children[2], "rotations", data->distance[i].value.rotations);
		BSON_APPEND_DOUBLE(&children[2], "angle", data->distance[i].value.angle);
		BSON_APPEND_DOUBLE(&children[2], "clock_period", data->distance[i].value.clock_period);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
		bson_append_document_end(&children[0], &children[1]);
		bson_destroy(&children[1]);
	}
	bson_append_array_end(*bson_document, &children[0]);
	bson_destroy(&children[0]);
	BSON_APPEND_ARRAY_BEGIN(*bson_document, "throttle", &children[0]);
	for (int i = 0; i < (data->throttle_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[0], "0", &children[1]);
		BSON_APPEND_INT64(&children[1], "timestamp", data->throttle[i].timestamp);
		BSON_APPEND_DOUBLE(&children[1], "value", data->throttle[i].value);
		bson_append_document_end(&children[0], &children[1]);
		bson_destroy(&children[1]);
	}
	bson_append_array_end(*bson_document, &children[0]);
	bson_destroy(&children[0]);
	BSON_APPEND_ARRAY_BEGIN(*bson_document, "brake", &children[0]);
	for (int i = 0; i < (data->brake_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[0], "0", &children[1]);
		BSON_APPEND_INT64(&children[1], "timestamp", data->brake[i].timestamp);
		BSON_APPEND_DOUBLE(&children[1], "value", data->brake[i].value);
		bson_append_document_end(&children[0], &children[1]);
		bson_destroy(&children[1]);
	}
	bson_append_array_end(*bson_document, &children[0]);
	bson_destroy(&children[0]);
	BSON_APPEND_DOCUMENT_BEGIN(*bson_document, "steering_wheel", &children[0]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "encoder", &children[1]);
	for (int i = 0; i < (data->steering_wheel.encoder_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->steering_wheel.encoder[i].timestamp);
		BSON_APPEND_DOUBLE(&children[2], "value", data->steering_wheel.encoder[i].value);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	BSON_APPEND_ARRAY_BEGIN(&children[0], "gears", &children[1]);
	for (int i = 0; i < (data->steering_wheel.gears_count); i++)
	{
		BSON_APPEND_DOCUMENT_BEGIN(&children[1], "0", &children[2]);
		BSON_APPEND_INT64(&children[2], "timestamp", data->steering_wheel.gears[i].timestamp);
		BSON_APPEND_DOCUMENT_BEGIN(&children[2], "value", &children[3]);
		BSON_APPEND_INT32(&children[3], "control", data->steering_wheel.gears[i].value.control);
		BSON_APPEND_INT32(&children[3], "cooling", data->steering_wheel.gears[i].value.cooling);
		BSON_APPEND_INT32(&children[3], "map", data->steering_wheel.gears[i].value.map);
		bson_append_document_end(&children[2], &children[3]);
		bson_destroy(&children[3]);
		bson_append_document_end(&children[1], &children[2]);
		bson_destroy(&children[2]);
	}
	bson_append_array_end(&children[0], &children[1]);
	bson_destroy(&children[1]);
	BSON_APPEND_INT32(&children[0], "marker", data->steering_wheel.marker);
	bson_append_document_end(*bson_document, &children[0]);
	bson_destroy(&children[0]);
	
}

void structureDelete(data_t *data) {
	free(data->inverterRight);
	free(data->inverterLeft);
	free(data->bms_hv.temperature);
	free(data->bms_hv.voltage);
	free(data->bms_hv.current);
	free(data->bms_hv.errors);
	free(data->bms_hv.warnings);
	free(data->bms_lv.values);
	free(data->bms_lv.errors);
	free(data->gps.old.location);
	free(data->gps.old.time);
	free(data->gps.old.true_track_mode);
	free(data->imu_gyro);
	free(data->imu_accel);
	free(data->front_wheels_encoder);
	free(data->distance);
	free(data->throttle);
	free(data->brake);
	free(data->steering_wheel.encoder);
	free(data->steering_wheel.gears);
	free(data);
	
}

gather_code gatherStructure(data_t *document)
{
    gather_code outcome = GATHER_KEEP;

	document->id = condition.structure.id++;
    document->sessionName = condition.mongodb.instance->session_name;
	
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
                            condition.session.selected_pilot = pilot_index;
                            condition.session.selected_race = race_index;
                            outcome = GATHER_ENABLE;
                            break;
                        default:
                            printf("Error in structure: telemetry status with no sense\n");
                            outcome = GATHER_ERROR;
                            break;
                    }
                }
                break;
            }

		clock_gettime(CLOCK_MONOTONIC, &tend);
		msec = (((double)tend.tv_sec * 1000 + 1.0e-6 * tend.tv_nsec) - end);
	} while (msec < condition.structure.sending_rate);

	return outcome;
}

/* HELPERS DEFINITIONS */