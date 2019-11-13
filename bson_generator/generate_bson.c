#include <bson.h>
#include "struct.h"

int generate_bson(config_t *cfg, data_t *data, bson_t **sending)
{
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
    	BSON_APPEND_INT32(&children[3], "code", data->bms_hv.errors[i].value.code);
    	BSON_APPEND_INT32(&children[3], "index", data->bms_hv.errors[i].value.index);
    	BSON_APPEND_INT32(&children[3], "value", data->bms_hv.errors[i].value.value);
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
    	BSON_APPEND_INT32(&children[2], "value", data->bms_hv.warnings[i].value);
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
    	BSON_APPEND_DOCUMENT_BEGIN(&children[2], "values", &children[3]);
    	BSON_APPEND_DOUBLE(&children[3], "voltage", data->bms_lv.values[i].values.voltage);
    	BSON_APPEND_DOUBLE(&children[3], "temperature_avg", data->bms_lv.values[i].values.temperature_avg);
    	BSON_APPEND_DOUBLE(&children[3], "temperature_max", data->bms_lv.values[i].values.temperature_max);
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
    BSON_APPEND_ARRAY_BEGIN(*sending, "imu_gyro", &children[0]);
    for (int i = 0; i < (data->imu_gyro_count); i++)
    {
    	BSON_APPEND_DOCUMENT_BEGIN(&children[0], "0", &children[1]);
    	BSON_APPEND_INT64(&children[1], "timestamp", data->imu_gyro[i].timestamp);
    	BSON_APPEND_DOCUMENT_BEGIN(&children[1], "value", &children[2]);
    	BSON_APPEND_DOUBLE(&children[2], "x", data->imu_gyro[i].value.x);
    	BSON_APPEND_DOUBLE(&children[2], "y", data->imu_gyro[i].value.y);
    	BSON_APPEND_DOUBLE(&children[2], "z", data->imu_gyro[i].value.z);
    	bson_append_document_end(&children[1], &children[2]);
    	bson_destroy(&children[2]);
    	bson_append_document_end(&children[0], &children[1]);
    	bson_destroy(&children[1]);
    }
    bson_append_array_end(*sending, &children[0]);
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