#include "state_machine.h"

result_codes (*state[])() = { init_state, idle_state, enabled_state, exit_state };

transition_t state_transitions[] = {
    {   INIT,       INITIALIZED,    IDLE },
    {   INIT,       ERROR,          EXIT },
    {   IDLE,       REPEAT,         IDLE },
    {   IDLE,       TOGGLE,         ENABLED },
    {   IDLE,       ERROR,          EXIT },
    {   ENABLED,    REPEAT,         ENABLED },
    {   ENABLED,    TOGGLE,         IDLE },
    {   ENABLED,    ERROR,          EXIT }
};

result_codes init_state() {
    infoStartingUp();

    debugConfigPath();
    config_code config_outcome = handleConfig();
    if (config_outcome != CONFIG_OK) {
        errorParsingConfig();
        return ERROR;
    }

    can_code can_outcome = canSetup();
    if (can_outcome != CAN_SERVICE_OK) {
        char* message = canErrorMessage(can_outcome);
        errorGeneric(message);
        return ERROR;
    }

    mongo_code mongo_outcome = mongoSetup();
    if (mongo_outcome != MONGO_OK) {
        char* message = mongoErrorMessage(mongo_outcome);
        errorGeneric(message);
        return ERROR;
    }

    mosquitto_code mosquitto_outcome = mosquittoSetup();
    if (mosquitto_outcome != MOSQUITTO_OK) {
        char* message = mosquittoErrorMessage(mosquitto_outcome);
        errorGeneric(message);
        return ERROR;
    }
    
    successStartedUp();
    mosquittoLogStartup();

    return INITIALIZED;
}

result_codes idle_state() {
    logDebug("Resetting structure id");
    resetStructureId();
    logDebug("Creating empty structure");
    data_t *document = structureCreate();
    logDebug("Gathering data from can");
    gather_code gather_outcome = gatherStructure(document);
    logDebug("Transforming document to bson");
    bson_t *bson_document;
    structureToBson(document, &bson_document);
    logDebug("Sending to mqtt");
    mosquittoSend(bson_document);
    logDebug("Deallocating structure");
    structureDelete(document);
    logDebug("Deallocating bson message");
    bson_destroy(bson_document);

    switch (gather_outcome) {
        case GATHER_KEEP:
        case GATHER_IDLE:
            return REPEAT;
        case GATHER_ENABLE:
            logDebug("Starting new session");
            mongoStartSession();
            infoNewSession();
            mosquittoLogSession();
            logDebug("Answering to the wheel");
            canAnswerWheel(1);
            return TOGGLE;
        case GATHER_ERROR:
            return ERROR;
    }
}

result_codes enabled_state() {
    logDebug("Creating empty structure");
    data_t *document = structureCreate();
    logDebug("Gathering data from can");
    gather_code gather_outcome = gatherStructure(document);
    logDebug("Transforming document to bson");
    bson_t *bson_document;
    structureToBson(document, &bson_document);
    logDebug("Sending to mqtt");
    mosquittoSend(bson_document);
    logDebug("Inserting to mongo");
    mongoInsert(bson_document);
    size_t size; bson_as_relaxed_extended_json(bson_document, &size);
    successInsertion(size);
    mosquittoLogInsertion(size);
    logDebug("Deallocating structure");
    structureDelete(document);
    logDebug("Deallocating bson message");
    bson_destroy(bson_document);

    switch (gather_outcome) {
        case GATHER_IDLE:
            logDebug("Answering to the wheel");
            canAnswerWheel(0);
            return TOGGLE;
        case GATHER_KEEP:
        case GATHER_ENABLE:
            return REPEAT;
        case GATHER_ERROR:
            return ERROR;
    }
}

result_codes exit_state() {
    infoQuitting();
    mosquittoLogQuit();
    mongoQuit();
    mosquittoQuit();
    successQuitting();
}

state_codes lookup_transitions(state_codes current_state, result_codes result_code) {
    int n = sizeof(state_transitions) / sizeof(transition_t);
    char* state_labels[4] = { "INIT", "IDLE", "ENABLED", "EXIT" }; 
    
    transition_t transition;
    for (int i = 0; i < n; ++i) {
        transition = state_transitions[i];
        if (transition.from_state == current_state && transition.result_code == result_code) {
            infoTransition(transition.from_state, transition.to_state, state_labels);
            mosquittoLogTransition(transition.from_state, transition.to_state, state_labels);
            return transition.to_state;
        }
    }

    return EXIT;
}