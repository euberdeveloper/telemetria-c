#ifndef TELEMETRY_MONGO_SERVICE
#define TELEMETRY_MONGO_SERVICE

/* IMPORTS */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <bson.h>
#include <mongoc.h>
#include "../../utils/misc_utils/misc_utils.h"
#include "../../state_machine/state_machine_condition.h"

/* EXTERN */

extern condition_t condition;

/* TYPES */

typedef enum {
    MONGO_OK,
    MONGO_SETUP_ERROR,
    MONGO_INSERT_ERROR
} mongo_code;

/* FUNCTIONS */

mongo_code mongoSetup();
mongo_code mongoStartSession();
mongo_code mongoInsert(bson_t* data);
void mongoQuit();

#endif