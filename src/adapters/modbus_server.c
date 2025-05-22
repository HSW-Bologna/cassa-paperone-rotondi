#define LIGHTMODBUS_SLAVE_FULL
#define LIGHTMODBUS_DEBUG
#define LIGHTMODBUS_IMPL
#include "lightmodbus/lightmodbus.h"
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "model/model.h"
#include "bsp/rs232_client.h"


static ModbusError register_callback(const ModbusSlave *server, const ModbusRegisterCallbackArgs *args,
                                     ModbusRegisterCallbackResult *result);
static ModbusError exception_callback(const ModbusSlave *server, uint8_t function, ModbusExceptionCode code);

static ModbusSlave server = {0};


void modbus_server_init(void) {
    ModbusErrorInfo err;
    err = modbusSlaveInit(&server, register_callback, exception_callback, modbusDefaultAllocator,
                          modbusSlaveDefaultFunctions, modbusSlaveDefaultFunctionCount);

    // Check for errors
    assert(modbusIsOk(err) && "modbusSlaveInit() failed!");
}


void modbus_server_manage(mut_model_t *model) {
    uint8_t request[256]   = {0};
    size_t  request_length = bsp_rs232_client_recv(request, sizeof(request), 20);

    if (request_length > 0) {
        modbusSlaveSetUserPointer(&server, model);
        ModbusErrorInfo err = modbusParseRequestRTU(&server, 1, request, (uint8_t)request_length);

        if (modbusIsOk(err)) {
            const uint8_t *response        = modbusSlaveGetResponse(&server);
            uint16_t       response_length = modbusSlaveGetResponseLength(&server);
            bsp_rs232_client_send((uint8_t *)response, response_length);
        }

        bsp_rs232_client_flush();
    } else {
        // No data
    }
}


static ModbusError register_callback(const ModbusSlave *server, const ModbusRegisterCallbackArgs *args,
                                     ModbusRegisterCallbackResult *result) {
    printf("Register query:\n"
           "\tquery: %s\n"
           "\t type: %s\n"
           "\t   id: %d\n"
           "\tvalue: %d\n"
           "\t  fun: %d\n",
           modbusRegisterQueryStr(args->query), modbusDataTypeStr(args->type), args->index, args->value,
           args->function);

    switch (args->query) {
        // Pretend to allow all access
        case MODBUS_REGQ_R_CHECK:
        case MODBUS_REGQ_W_CHECK:
            result->exceptionCode = MODBUS_EXCEP_NONE;
            break;

        case MODBUS_REGQ_R:
            break;

        default:
            break;
    }

    return MODBUS_OK;
}


static ModbusError exception_callback(const ModbusSlave *server, uint8_t function, ModbusExceptionCode code) {
    printf("Slave exception %s (function %d)\n", modbusExceptionCodeStr(code), function);
    return MODBUS_OK;
}
