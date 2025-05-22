#ifndef MODBUS_SERVER_H_INCLUDED
#define MODBUS_SERVER_H_INCLUDED


#include "model/model.h"


void modbus_server_init(void);
void modbus_server_manage(mut_model_t *model);


#endif
