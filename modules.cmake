include_directories(
        modules/FreeRTOS-Plus-CLI
        modules/FreertosIO
        modules/SX1278/driver
        modules/LoRa
        modules/CLI_Task_commands
        )
file(GLOB_RECURSE SRC "modules/*.c")
list(APPEND SOURCES ${SRC})