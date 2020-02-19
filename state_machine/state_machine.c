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
    config_code config_outcome = handleConfig();
    if (config_outcome != CONFIG_OK) {
        return ERROR;
    }

    can_code can_outcome = canSetup();
    if (config_outcome != CAN_SERVICE_OK) {
        return ERROR;
    }

    mongo_code mongo_outcome = mongoSetup();
    if (mongo_outcome != MONGO_OK) {
        return ERROR;
    }

    mosquitto_code mosquitto_outcome = mosquittoSetup();
    if (mosquitto_outcome != MOSQUITTO_OK) {
        return ERROR;
    }
    
    mosquittoLogStartup();

    return INITIALIZED;
}

result_codes idle_state() {
    condition.structure.id = 0;
    data_t *document = structureCreate();
    gather_code gather_outcome = gatherStructure(document);
    bson_t *bson_document;
    structureToBson(document, &bson_document);
    mosquittoSend(bson_document);
    structureDelete(document);
    bson_destroy(bson_document);

    switch (gather_outcome) {
        case GATHER_KEEP:
        case GATHER_IDLE:
            return REPEAT;
        case GATHER_ENABLE:
            mongoStartSession();
            mosquittoLogSession();
            canAnswerWheel(1);
            return TOGGLE;
        case GATHER_ERROR:
            return ERROR;
    }
}

result_codes enabled_state() {
    data_t *document = structureCreate();
    gather_code gather_outcome = gatherStructure(document);
    bson_t *bson_document;
    structureToBson(document, &bson_document);
    mosquittoSend(bson_document);
    mongoInsert(bson_document);
    size_t size; bson_as_relaxed_extended_json(bson_document, &size);
    mosquittoLogInsertion(size);
    structureDelete(document);
    bson_destroy(bson_document);

    switch (gather_outcome) {
        case GATHER_IDLE:
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
    mosquittoLogQuit();
    mongoQuit();
    mosquittoQuit();
}

state_codes lookup_transitions(state_codes current_state, result_codes result_code) {
    int n = sizeof(state_transitions) / sizeof(transition_t);
    char* state_labels[4] = { "INIT", "IDLE", "ENABLED", "EXIT" }; 
    
    transition_t transition;
    for (int i = 0; i < n; ++i) {
        transition = state_transitions[i];
        if (transition.from_state == current_state && transition.result_code == result_code) {
            mosquittoLogTransition(transition.from_state, transition.to_state, state_labels);
            return transition.to_state;
        }
    }

    return EXIT;
}