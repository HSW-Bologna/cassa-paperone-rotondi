cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr)
set(PROJECT_NAME cassa_rotondi)
project(${PROJECT_NAME})


set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")
set(LIB_DIR "${PROJECT_SOURCE_DIR}/lib")

FILE(GLOB SOURCES "${SOURCE_DIR}/*.c" "${SOURCE_DIR}/bsp/*.c" "${SOURCE_DIR}/adapters/*.c" "${SOURCE_DIR}/controller/*c" "${SOURCE_DIR}/model/*.c" "${LIB_DIR}/mfrc522/src/*.c")

target_include_directories(app PRIVATE ${SOURCE_DIR} "${LIB_DIR}/liblightmodbus/include" "${LIB_DIR}/mfrc522/src/")
target_sources(app PRIVATE ${SOURCES})
