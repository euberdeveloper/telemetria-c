#include "mosquitto_service.h"

/* INTERNAL */

static const char* MQTT_ID = "Eagle racing team - Telemetria";
static const int MQTT_KEEP_ALIVE = 60;

/* EXPORTED */

mosquitto_code mosquittoSetup() {
  mosquitto_lib_init();

  condition.mqtt.instance = mosquitto_new(MQTT_ID, false, NULL);
  if (!condition.mqtt.instance) {
    printf("Error in creating mqtt instance\n");
    return MOSQUITTO_CREATION_ERROR;
  }

  int connected = mosquitto_connect(condition.mqtt.instance, condition.mqtt.host, condition.mqtt.port, MQTT_KEEP_ALIVE);
  if (connected == MOSQ_ERR_SUCCESS) {
    return MOSQUITTO_OK;
  }
  else {
    printf("Error in connecting to mqtt\n");
    return MOSQUITTO_CONNECTION_ERROR;
  }
}

mosquitto_code mosquittoSend(const bson_t* message) {
  int outcome = mosquitto_publish(condition.mqtt.instance, NULL, condition.mqtt.data_topic, message->len, bson_get_data(message), 0, false);
  
  if (outcome == MOSQ_ERR_SUCCESS) {
    return MOSQUITTO_OK;
  }
  else {
    printf("Error in sending data via mqtt");
    return MOSQUITTO_PUBLISH_ERROR;
  }
}

mosquitto_code mosquittoLog(const char* message) {
  int size = strlen(message);
  int outcome = mosquitto_publish(condition.mqtt.instance, NULL, condition.mqtt.log_topic, size, message, 0, false);
  
  if (outcome == MOSQ_ERR_SUCCESS) {
    return MOSQUITTO_OK;
  }
  else {
    printf("Error in logging data via mqtt");
    return MOSQUITTO_PUBLISH_ERROR;
  }
}

void mosquittoQuit() {
  mosquitto_destroy(condition.mqtt.instance);
  mosquitto_lib_cleanup();
}

mosquitto_code mosquittoLogStartup() {
  char* message = strdup("[INFO] Started up succesfully");
  mosquittoLog(message);
  free(message);
}

mosquitto_code mosquittoLogTransition(int from, int to, char** labels) {
  mosquitto_code outcome = MOSQUITTO_OK;
  
  if (from != to) {
    char* from_label = strdup(labels[from]);
    char* to_label = strdup(labels[to]);
    char* message;
    asprintf(&message, "[INFO] Transition from %s to %s", from_label, to_label);    
    mosquitto_code outcome = mosquittoLog(message);
    free(from_label);
    free(to_label);
    free(message);
  }
  
  return outcome;
}

mosquitto_code mosquittoLogSession() {
  char* message;
	asprintf(&message, "[INFO] New session created %s", condition.mongodb.instance->session_name);
	
  mosquitto_code outcome = mosquittoLog(message);
  free(message);
  return outcome;
}

mosquitto_code mosquittoLogInsertion(int size) {
  char* message;
	asprintf(&message, "[INFO] Inserted document, size: %d B", size);
	
  mosquitto_code outcome = mosquittoLog(message);
  free(message);
  return outcome;
}

mosquitto_code mosquittoLogQuit() {
  char* message = strdup("[INFO] Quitting telemetry");
  mosquittoLog(message);
  free(message);
}