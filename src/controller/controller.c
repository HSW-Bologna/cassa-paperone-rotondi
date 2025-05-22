#include "controller.h"
#include "model/model.h"
#include "adapters/modbus_server.h"


static mut_model_t  _model = {};
static mut_model_t *model  = &_model;


void controller_init(void) {
    modbus_server_init();
}


void controller_manage(void) {
    modbus_server_manage(model);
}
