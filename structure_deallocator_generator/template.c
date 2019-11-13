#include "struct.h"

data_t *data_setup(config_t *cfg)
{
	data_t *data;
	data = (data_t *)malloc(sizeof(data_t));

	// {{GENERATE_STRUCTURE_DEALLOCATOR_CODE}}
	
	return data;
}