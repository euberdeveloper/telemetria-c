#include "mongo_service.h"

/* INTERNAL FUNCTIONS AND CONSTANTS */

static const char* MONGODB_PREFIX = "mongodb://";
static const char* MONGODB_APP_NAME = "E-agle racing team - Telemetria";
static char* getStringPort(int port);
static char* getUri(char* host, char* port);
static mongodb_instance_t* getInstance(char* uri, char* db);
static char* itostr(int value);
static char* getFormattedTimestamp(int timestamp);
static char* getSessionName(const char* formatted_timestamp, const char* pilot, const char* race);

static char* getStringPort(int port) {
	int length = digitsCount(port) + 1;
	char *port_string = (char*) malloc(sizeof(char) * length);

	for (int i = 4; i >= 0; --i) {
		port_string[i] = ((char) (port % 10)) + '0';
		port /= 10;
	}
	port_string[length - 1] = '\0';

	return port_string;
}

static char* getUri(char* host, char* port) {
	int length = strlen(MONGODB_PREFIX) + strlen(host) + 1 + strlen(port) + 1;
	char* uri = (char*) malloc(sizeof(char) * length);

	strcpy(uri, MONGODB_PREFIX);
	strcat(uri, host);
	strcat(uri, ":");
	strcat(uri, port);
	
	return uri;
}

static mongodb_instance_t* getInstance(char* uri, char* db) {
	mongodb_instance_t* instance = 
		(mongodb_instance_t*) malloc(sizeof(mongodb_instance_t));

	bson_error_t error;
	instance->uri = mongoc_uri_new_with_error(uri, &error);
	if (!instance->uri) {
		printf("Error in mongo: %s\n", error.message);
		return NULL;
	}

	instance->client = mongoc_client_new_from_uri(instance->uri);
	if (!instance->client) {
		printf("Error in mongo: cannot create client\n");
		return NULL;
	}

	mongoc_client_set_appname(instance->client, MONGODB_APP_NAME);
	instance->database = mongoc_client_get_database(instance->client, db);

	return instance;
}

static char* itostr(int value) {
	char* toRtn = (char*) malloc(sizeof(char) * 15);
	int reverted = 0;

	int times = 0;
	while (value != 0) {
		reverted = (reverted*10) + value%10;
		value /= 10;
		times++;
	}

	int pos = 0;
	while (times-- > 0) {
		toRtn[pos++] = (reverted % 10) + '0';
		reverted /= 10;
	}

	toRtn[pos] = 0;
	return toRtn;
}

static char* getFormattedTimestamp(int timestamp) {
	char* result;

	time_t t = timestamp;
	struct tm *formatted = localtime(&t);

	int date = formatted->tm_mday + (formatted->tm_mon + 1) * 100 + (formatted->tm_year + 1900) * 10000;
	char* dateString = itostr(date);

	int time = (formatted->tm_sec) + (formatted->tm_min) * 100 + (formatted->tm_hour) * 10000;
	char* timeString = itostr(time);

	int length = strlen(dateString) + 1 + strlen(timeString) + 1;
	result = (char*) malloc(sizeof(char) * length);
	strcpy(result, dateString);
	strcat(result, "_");
	strcat(result, timeString);

	free(dateString);
	free(timeString);

	return result;
}

static char* getSessionName(const char* formatted_timestamp, const char* pilot, const char* race) {
	const int length = strlen(formatted_timestamp) + 1 + strlen(pilot) + 1 + strlen(race) + 1;
	char* session_name = (char*) malloc(sizeof(char) * length);

	strcpy(session_name, formatted_timestamp);
	strcat(session_name, "_");
	strcat(session_name, pilot);
	strcat(session_name, "_");
	strcat(session_name, race);

	return session_name;
}

/* EXPORTED FUNCTIONS */

mongo_code mongoSetup() {
	char* host = condition.mongodb.host;
	char* port = getStringPort(condition.mongodb.port);
	char* db = condition.mongodb.db;
	char* uri = getUri(host, port);

	condition.mongodb.instance = getInstance(uri, db);
	if (condition.mongodb.instance == NULL) {
		return MONGO_SETUP_ERROR;
	}
	else {
		return MONGO_OK;
	}
}

mongo_code mongoStartSession() {
	int timestamp = time(NULL);
	const char* formatted_timestamp = getFormattedTimestamp(timestamp);
	const char* pilot = condition.session.pilots[condition.session.selected_pilot];
	const char* race = condition.session.races[condition.session.selected_race];
	char* session_name = getSessionName(formatted_timestamp, pilot, race);
	
	condition.mongodb.instance->session_name = session_name;
	condition.mongodb.instance->collection = 
		mongoc_client_get_collection(condition.mongodb.instance->client, mongoc_database_get_name(condition.mongodb.instance->database), session_name);

	return MONGO_OK;
}

mongo_code mongoInsert(bson_t *data) {
	bson_error_t error;
	const int outcome = mongoc_collection_insert_one(condition.mongodb.instance->collection, data, NULL, NULL, &error);
	
	if (!outcome) {
		printf ("Error in inserting in mongodb: %s\n", error.message);
		return MONGO_INSERT_ERROR;
	}
	else {
		return MONGO_OK;
	}
}

void mongoQuit() {
	mongoc_collection_destroy(condition.mongodb.instance->collection);
   	mongoc_database_destroy(condition.mongodb.instance->database);
   	mongoc_client_destroy(condition.mongodb.instance->client);
   	mongoc_uri_destroy(condition.mongodb.instance->uri);
   	mongoc_cleanup();
}
