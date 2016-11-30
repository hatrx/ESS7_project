# Compile drivers
add_library(drivers
	${SRC_DIR}/drivers/uart.c
	${SRC_DIR}/drivers/mpu.c
	${SRC_DIR}/drivers/onboard_leds.c
	${SRC_DIR}/drivers/system_clock.c
	${SRC_DIR}/drivers/watchdog.c
	${SRC_DIR}/drivers/time_get.c
	${SRC_DIR}/drivers/utils.c
	${SRC_DIR}/drivers/rtc.c
)
target_link_libraries(drivers
	CMSIS
	STM32f4xx_HAL_Driver
)

add_library(APEX
	${SRC_DIR}/partitions/utils/apex.c
)
target_link_libraries(APEX
	CMSIS
	STM32f4xx_HAL_Driver
)

add_library(dummy1
	${SRC_DIR}/partitions/dummy1/dummy1.c
)
target_link_libraries(dummy1
	drivers
)


add_library(dummy2
	${SRC_DIR}/partitions/dummy2/dummy2.c
)
target_link_libraries(dummy2
	drivers
	APEX
)


# Compile OS
add_executable(${PROJECT_NAME}
	${SRC_DIR}/hedder.c
	${SRC_DIR}/kernel/main.c
	${SRC_DIR}/kernel/syscalls.c
	${SRC_DIR}/kernel/context.c
	${SRC_DIR}/kernel/scheduler.c
	${SRC_DIR}/kernel/svc_handler.c
	${SRC_DIR}/kernel/arinc/partition.c
	${SRC_DIR}/kernel/arinc/process.c

)
target_link_libraries(${PROJECT_NAME}
	drivers
	CMSIS
	STM32f4xx_HAL_Driver

	dummy1
	dummy2
)
# Command for writing OS to target flash.
open_ocd_write_flash(${PROJECT_NAME})
