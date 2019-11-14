#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "./structure.h"
#include "./jsmn/jsmn.h"
#include "./can_lib/can_custom_lib.h"

#include <bson.h>
#include <mongoc.h>
#include <mosquitto.h>

//ENUM DEFINITION
typedef enum
{
	INIT = 0,
	SEND = 1,
	PAUSE = 2,
	EXIT = 3,
} state_t;

//SIGNATURES DEFINITION
void handle_signal(int s);

config_t *config_setup(const char *cfgpath);
dbhandler_t *mongo_setup(config_t *cfg);
data_t *data_setup(config_t *cfg);
mosq_t *mosquitto_setup(config_t *cfg);

int data_gather(config_t *cfg, data_t *data);
int data_elaborate(config_t *cfg, data_t *data, bson_t **sending);
int mongo_insert(bson_t *insert, dbhandler_t *handler);

int mosquitto_quit(mosq_t *handler);
int config_quit(config_t *cfg);
int mongo_quit(dbhandler_t *handler);
int data_quit(data_t *data);
struct tm *get_time();

//GLOBAL VARIABLES DEFINITION
int verbose = 1;
state_t telemetria_state = INIT;
int can_socket;
uint32_t structure_saved_id = 0;

int main(int argc, char const *argv[])
{
	if (argc != 2)
	{
		if (verbose)
			printf("No argument passed. Usage: ./pub <cfg.json>\n");
		return 1;
	}

	signal(SIGINT, handle_signal);
	signal(SIGTERM, handle_signal);

	config_t *config_file;
	dbhandler_t *mongo_handler;
	data_t *can_data;
	mosq_t *mosquitto_handler;

	while (telemetria_state != EXIT)
	{
		switch (telemetria_state)
		{
		case INIT:
			config_file = config_setup(argv[1]);
			if (config_file == NULL)
			{
				telemetria_state = EXIT;
			}
			else
			{
				//config file is correct
				mongo_handler = mongo_setup(config_file);

				if (mongo_handler == NULL)
				{
					telemetria_state = EXIT;
				}
				else
				{

					//opening a connection to the canbus
					struct sockaddr_can addr;
					char *name = (char *)malloc(sizeof(char) * 5);
					strcpy(name, "can0");
					can_socket = open_can_socket(name, &addr);

					if (can_socket <= 0)
					{
						telemetria_state = EXIT;
					}
					else
					{
						//setting up mosquitto
						mosquitto_handler = mosquitto_setup(config_file);

						//send the init message and switch to pause;
						struct tm *timeinfo = get_time();
						if (verbose)
							printf("Current local time and date: %s", asctime(timeinfo));

						bson_t *insert = BCON_NEW(
							"info", BCON_UTF8("Telemetria started up"),
							"datetime", BCON_DATE_TIME(mktime(timeinfo) * 1000));

						mongo_insert(insert, mongo_handler);
						bson_destroy(insert);
						//setting up the data structure used for gathering data
						telemetria_state = SEND;
					}
				}
			}
			break;

		case SEND:; //?
					//send()
			bson_t *sending;

			can_data = data_setup(config_file);
			data_gather(config_file, can_data);

			data_elaborate(config_file, can_data, &sending);
			mongo_insert(sending, mongo_handler);
			mosquitto_publish(mosquitto_handler->handler, NULL, mosquitto_handler->mqtt_topic, sending->len, bson_get_data(sending), 0, false);
			//data_quit(can_data);
			if (verbose)
			{
				char *str = bson_as_relaxed_extended_json(sending, NULL);
				printf("\n%s\n For a total of %d bytes\n\n", str, sending->len);
				bson_free(str);
				bson_destroy(sending);
			}
			//telemetria_state = EXIT;
			break;

		case PAUSE:
			//pause()
			telemetria_state = EXIT;
			break;
		}
	}

	//data_quit(can_data);
	mongo_quit(mongo_handler);
	config_quit(config_file);
	return 0;
}

void handle_signal(int s) { telemetria_state = EXIT; }

config_t *config_setup(const char *cfgpath)
{
	//set the pointers to the file and to the configure structure
	FILE *fptr;
	config_t *toRtn;

	//try to open the file
	if (verbose)
		printf("Setup procedure started. Opening file %s.\n", cfgpath);
	fptr = fopen(cfgpath, "r");

	if (fptr == NULL)
	{
		if (verbose)
			printf("%s opening procedure failed.\n", cfgpath);
		return NULL;
	}

	if (verbose)
		printf("File %s opened with success. Parsing procedure started.\n", cfgpath);

	//if we are here the file has been opened with success
	//then we put the whole file into one string, reallocating it as needed
	char *json;
	char *line;
	size_t len = 0;
	int totlen = 0;

	getline(&line, &len, fptr);
	totlen += len;

	json = (char *)malloc(sizeof(char) * totlen);
	strcpy(json, line);

	while (getline(&line, &len, fptr) != -1)
	{
		totlen += len;

		json = (char *)realloc(json, sizeof(char) * totlen);
		strcat(json, line);
	}
	fclose(fptr);

	if (verbose)
		printf("Passed JSON(%s) : %s\n\n", cfgpath, json);

	//we start the parsing procedure and we split the data in tokens.
	int result;
	jsmn_parser p;
	jsmntok_t tokens[64]; /* We expect no more than 64(39) JSON tokens */

	jsmn_init(&p);
	result = jsmn_parse(&p, json, strlen(json), tokens, 64);

	if (verbose)
		printf("Parsed #%d entries.\n", result - 1);

	toRtn = (config_t *)malloc(sizeof(config_t));
	for (int i = 1; i < result; i += 2)
	{
		jsmntok_t key = tokens[i];
		jsmntok_t value = tokens[i + 1];

		unsigned int lenght = key.end - key.start;
		char keyString[lenght + 1];
		memcpy(keyString, &json[key.start], lenght);
		keyString[lenght] = '\0';

		lenght = value.end - value.start;
		char valueString[lenght + 1];
		memcpy(valueString, &json[value.start], lenght);
		valueString[lenght] = '\0';

		if (verbose)
			printf("\t%s:%s\n", keyString, valueString);

		if (strcmp(keyString, "broker_host") == 0)
		{
			toRtn->broker_host = (char *)malloc(sizeof(char) * lenght);
			strcpy(toRtn->broker_host, valueString);
		}
		else if (strcmp(keyString, "broker_port") == 0)
		{
			toRtn->broker_port = atoi(valueString);
		}
		else if (strcmp(keyString, "mqtt_topic") == 0)
		{
			toRtn->mqtt_topic = (char *)malloc(sizeof(char) * lenght);
			strcpy(toRtn->mqtt_topic, valueString);
		}

		else if (strcmp(keyString, "mongo_host") == 0)
		{
			toRtn->mongo_host = (char *)malloc(sizeof(char) * lenght);
			strcpy(toRtn->mongo_host, valueString);
		}
		else if (strcmp(keyString, "mongo_port") == 0)
		{
			toRtn->mongo_port = atoi(valueString);
		}
		else if (strcmp(keyString, "mongo_db") == 0)
		{
			toRtn->mongo_db = (char *)malloc(sizeof(char) * lenght);
			strcpy(toRtn->mongo_db, valueString);
		}
		else if (strcmp(keyString, "mongo_collection") == 0)
		{
			toRtn->mongo_collection = (char *)malloc(sizeof(char) * lenght);
			strcpy(toRtn->mongo_collection, valueString);
		}
		else if (strcmp(keyString, "sending_time") == 0)
		{
			toRtn->sending_time = atoi(valueString);
		}
		//{{GENERATE_CONFIG_INITIALIZER_CODE}}
	}
	if (verbose)
		printf("%s has generated a correct set of configurations.\n\n", cfgpath);
	return toRtn;
}
dbhandler_t *mongo_setup(config_t *cfg)
{
	dbhandler_t *toRtn;
	char *uri_string;

	if (verbose)
		printf("Mongo setup procedure started.\n");

	//lenght of the port
	int port = cfg->mongo_port;
	char *port_string = (char *)malloc(sizeof(char) * 6);
	for (int i = 4; i >= 0; --i)
	{
		port_string[i] = ((char)(port % 10)) + '0';
		port /= 10;
	}
	port_string[5] = '\0';
	while (port_string[0] == '0')
	{
		for (int i = 0; i <= 4; ++i)
			port_string[i] = port_string[i + 1];
		port_string[5] = '\0';
	}

	//lenght of the hostname, lenght of the port + 10 for "mongodb://" + 1 for ":" + 1 for "\0"
	int lenght = strlen(cfg->mongo_host) + strlen(port_string) + 12;
	uri_string = (char *)malloc(sizeof(char) * lenght);

	strcpy(uri_string, "mongodb://");
	strcat(uri_string, cfg->mongo_host);
	strcat(uri_string, ":");
	strcat(uri_string, port_string);

	if (verbose)
		printf("Mongo uri designed: %s\n", uri_string);

	toRtn = (dbhandler_t *)malloc(sizeof(dbhandler_t));

	bson_error_t error;
	toRtn->uri = mongoc_uri_new_with_error(uri_string, &error);
	if (!toRtn->uri)
	{
		if (verbose)
			printf("Establishing a connection to \"%s\" failed.\nError message: %s.\n", uri_string, error.message);
		return NULL;
	}

	toRtn->client = mongoc_client_new_from_uri(toRtn->uri);
	if (!toRtn->client)
	{
		if (verbose)
			printf("Client process startup failed.\n");
		return NULL;
	}

	mongoc_client_set_appname(toRtn->client, "E-agle racing team - Telemetria");

	toRtn->database = mongoc_client_get_database(toRtn->client, cfg->mongo_db);
	toRtn->collection = mongoc_client_get_collection(toRtn->client, cfg->mongo_db, cfg->mongo_collection);

	if (verbose)
		printf("Mongo started up with success.\n\n");

	return toRtn;
}
data_t *data_setup(config_t *config)
{
	data_t *data;
	data = (data_t *)malloc(sizeof(data_t));
	//{{GENERATE_STRUCTURE_INITIALIZER_CODE}}
	return data;
}
mosq_t *mosquitto_setup(config_t *cfg)
{
	mosq_t *toRtn;
	toRtn = (mosq_t *)malloc(sizeof(mosq_t));

	//signal(SIGINT, handle_signal);
	//signal(SIGTERM, handle_signal);
	if (verbose)
		printf("Mosquitto setup procedure started\n");

	mosquitto_lib_init();

	toRtn->handler = mosquitto_new("Eagle racing team - Telemetria", false, NULL);
	if (!toRtn->handler)
	{
		free(toRtn);
		if (verbose)
			printf("Something went wrong during the mosq client startup.\n");
		return NULL;
	}

	toRtn->broker_host = (char *)malloc(sizeof(char) * strlen(cfg->broker_host));
	strcpy(toRtn->broker_host, cfg->broker_host);
	toRtn->broker_port = cfg->broker_port;
	toRtn->mqtt_topic = (char *)malloc(sizeof(char) * strlen(cfg->mqtt_topic));
	strcpy(toRtn->mqtt_topic, cfg->mqtt_topic);

	mosquitto_connect(toRtn->handler, toRtn->broker_host, toRtn->broker_port, 60);
	return toRtn;
}

int data_gather(config_t *cfg, data_t *data)
{
	double msec = 0, trigger = cfg->sending_time, end = 0;
	struct timespec tstart = {0, 0}, tend = {0, 0};
	clock_gettime(CLOCK_MONOTONIC, &tstart);
	end = ((double)tstart.tv_sec * 1000 + 1.0e-6 * tstart.tv_nsec);

	int id, data1, data2;

	do
	{
		id = data1 = data2 = 0;
		receive_can_compact(can_socket, &id, &data1, &data2);

		struct timespec tmessage = {0, 0};
		clock_gettime(CLOCK_MONOTONIC, &tmessage);
		size_t message_timestamp = ((double)tmessage.tv_sec * 1000 + 1.0e-6 * tmessage.tv_nsec);

		int firstByte = ((data1 >> 24) & 255);

		switch (id)
		{
		case (0xAA): //BMS HV
			switch (firstByte)
			{
			case 0x01: //voltage
				data->bms_hv.voltage[data->bms_hv.voltage_count].timestamp = message_timestamp;
				data->bms_hv.voltage[data->bms_hv.voltage_count].value.total = (data1 & 0x00FFFFFF) / 10000;
				data->bms_hv.voltage[data->bms_hv.voltage_count].value.max = ((data2 >> 16) & 0x0000FFFF) / 10000;
				data->bms_hv.voltage[data->bms_hv.voltage_count++].value.min = (data2 & 0x0000FFFF) / 10000;
				break;

			case 0x0A: //temperature
				data->bms_hv.temperature[data->bms_hv.temperature_count].timestamp = message_timestamp;
				data->bms_hv.temperature[data->bms_hv.temperature_count].value.average = ((data1 >> 8) & 0x0000FFFF) / 100;
				data->bms_hv.temperature[data->bms_hv.temperature_count].value.max = ((data1 & 0x000000FF) * 256 + ((data2 >> 24) & 0x000000FF)) / 100;
				data->bms_hv.temperature[data->bms_hv.temperature_count++].value.min = ((data2 >> 16) & 0x0000FFFF) / 100;
				break;

			case 0x05: //current
				data->bms_hv.current[data->bms_hv.current_count].timestamp = message_timestamp;
				data->bms_hv.current[data->bms_hv.current_count].value.current = ((data1 >> 8) & 0x0000FFFF) / 10;
				data->bms_hv.current[data->bms_hv.current_count++].value.pow = ((data1 & 0x000000FF) * 256 + ((data2 >> 24) & 0x000000FF));
				break;

			case 0x08: //errors
				data->bms_hv.errors[data->bms_hv.errors_count].timestamp = message_timestamp;
				data->bms_hv.errors[data->bms_hv.errors_count].value.code = ((data1 >> 16) & 0x000000FF);
				data->bms_hv.errors[data->bms_hv.errors_count].value.index = ((data1 >> 8) & 0x000000FF) / 10;
				data->bms_hv.errors[data->bms_hv.errors_count++].value.value = (((data1 & 0x000000FF) * 256) + ((data2 >> 24) & 0x000000FF));
				break;

			case 0x09: //warnings
				data->bms_hv.warnings[data->bms_hv.warnings_count].timestamp = message_timestamp;
				data->bms_hv.warnings[data->bms_hv.warnings_count].value = (data1 >> 16) & 0x000000FF;
				break;
			}
			break;

		case (0xB0): //Pedals
			if (firstByte == 0x01)
			{
				data->throttle[data->throttle_count].timestamp = message_timestamp;
				data->throttle[data->throttle_count++].value = ((data1 >> 16) & 255);
			}
			else if (firstByte = 0x02)
			{
				data->brake[data->brake_count].timestamp = message_timestamp;
				data->brake[data->brake_count++].value = ((data1 >> 16) & 255);
			}
			break;

		case (0xC0): //IMU and SWE
			switch (firstByte)
			{
			case 0x03: //XY
				data->imu_gyro.xy[data->imu_gyro.xy_count].timestamp = message_timestamp;
				data->imu_gyro.xy[data->imu_gyro.xy_count].value.x = (data1 >> 8) & 0x0000FFFF;
				data->imu_gyro.xy[data->imu_gyro.xy_count++].value.y = (data2 >> 16) & 0x0000FFFF;
				break;

			case 0x04: //Z
				data->imu_gyro.z[data->imu_gyro.z_count].timestamp = message_timestamp;
				data->imu_gyro.z[data->imu_gyro.z_count++].value = (data1 >> 8) & 0x0000FFFF;
				break;

			case 0x05: //axel
				data->imu_axel[data->imu_axel_count].timestamp = message_timestamp;
				data->imu_axel[data->imu_axel_count].value.x = ((data1 >> 16) & 255) * 256 + ((data1 >> 8) & 255);
				data->imu_axel[data->imu_axel_count].value.y = ((data1)&255) * 256 + ((data2 >> 24) & 255);
				data->imu_axel[data->imu_axel_count++].value.z = ((data2 >> 16) & 255) * 256 + ((data2 >> 8) & 255);
				break;

			case 0x02: //steering wheel
				data->steering_wheel_encoder[data->steering_wheel_encoder_count].timestamp = message_timestamp;
				data->steering_wheel_encoder[data->steering_wheel_encoder_count++].value = ((data1 >> 16) & 255);
				break;
			}
			break;

		case (0xD0): //GPS and FWE
			switch (firstByte)
			{
			case 0x01: //lat and speed
				data->gps.latspd[data->gps.latspd_count].timestamp = message_timestamp;
				data->gps.latspd[data->gps.latspd_count].value.latitude = (((data1 >> 16) & 255) * 256 + ((data1 >> 8) & 255)) * 100000 + ((data1 & 255) * 256 + ((data2 >> 24) & 255));
				data->gps.latspd[data->gps.latspd_count].value.lat_o = (((data2 >> 8) & 255) * 256) + (data2 & 255);
				data->gps.latspd[data->gps.latspd_count++].value.speed = data2 >> 16 & 255;
				break;

			case 0x02: //lon and altitude
				data->gps.lonalt[data->gps.lonalt_count].timestamp = message_timestamp;
				data->gps.lonalt[data->gps.lonalt_count].value.longitude = (((data1 >> 16) & 255) * 256 + ((data1 >> 8) & 255)) * 100000 + ((data1 & 255) * 256 + ((data2 >> 24) & 255));
				data->gps.lonalt[data->gps.lonalt_count].value.lon_o = (((data2 >> 8) & 255) * 256) + (data2 & 255);
				data->gps.lonalt[data->gps.lonalt_count++].value.altitude = data2 >> 16 & 255;
				break;

			case 0x06: //front wheels
				data->front_wheels_encoder[data->front_wheels_encoder_count].timestamp = message_timestamp;
				data->front_wheels_encoder[data->front_wheels_encoder_count++].value = ((data1 >> 16) & 255) * 256 + ((data1 >> 8) & 255);
				break;
			}
			break;

		case (0xFF): //BMS LV
			data->bms_lv.values[data->bms_lv.values_count].timestamp = message_timestamp;
			data->bms_lv.values[data->bms_lv.values_count].value.voltage = ((data1 >> 24) & 255) / 10.0;
			data->bms_lv.values[data->bms_lv.values_count].value.temperature_avg = message_timestamp;
			data->bms_lv.values[data->bms_lv.values_count++].value.temperature_max = ((data1 >> 8) & 255) / 5.0;
			break;

		case (0xAB): //Marker
			data->marker = 1;
			break;
		}

		clock_gettime(CLOCK_MONOTONIC, &tend);
		msec = (((double)tend.tv_sec * 1000 + 1.0e-6 * tend.tv_nsec) - end);
	} while (msec < trigger);
	return 0;
}
int data_elaborate(config_t *cfg, data_t *data, bson_t **sending)
{
	struct timespec time;
	if (clock_gettime(CLOCK_MONOTONIC, &time) == -1)
	{
		return -1;
	}
	data->id = structure_saved_id++;
	data->timestamp = time.tv_sec * 1000 + time.tv_nsec / 1E6;

	//{{GENERATE_BSON_CODE}}
	return 0;
}
int mongo_insert(bson_t *insert, dbhandler_t *handler)
{
	bson_error_t error;

	if (!mongoc_collection_insert_one(handler->collection, insert, NULL, NULL, &error))
	{
		if (verbose)
			printf("%s\n", error.message);
		return 1;
	}
	if (verbose)
		printf("The doc was successfully inserted inside the designed collection.\n");

	return 0;
}

int mosquitto_quit(mosq_t *handler)
{
	mosquitto_destroy(handler->handler);
	free(handler->broker_host);
	free(handler->mqtt_topic);

	free(handler);
	mosquitto_lib_cleanup();
	return 0;
}
int config_quit(config_t *cfg)
{
	free(cfg->broker_host);
	free(cfg->mqtt_topic);
	free(cfg->mongo_host);
	free(cfg->mongo_db);
	free(cfg->mongo_collection);

	free(cfg);
	return 0;
}
int mongo_quit(dbhandler_t *handler)
{
	mongoc_collection_destroy(handler->collection);
	mongoc_database_destroy(handler->database);
	mongoc_client_destroy(handler->client);
	mongoc_uri_destroy(handler->uri);
	mongoc_cleanup();

	free(handler);
	return 0;
}
int data_quit(data_t *data)
{
	//{{GENERATE_STRUCTURE_DEALLOCATOR_CODE}}
	return 0;
}

struct tm *get_time()
{
	time_t rawtime;
	struct tm *timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	return timeinfo;
}
