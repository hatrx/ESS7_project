# Compile drivers
add_library(drivers
	${SRC_DIR}/drivers/uart.c
	${SRC_DIR}/drivers/mpu.c
	${SRC_DIR}/drivers/onboard_leds.c
	${SRC_DIR}/drivers/system_clock.c
	${SRC_DIR}/drivers/watchdog.c
	${SRC_DIR}/drivers/get_time.c
)
target_link_libraries(drivers
	CMSIS
	STM32f4xx_HAL_Driver
)


# Compile OS
add_executable(${PROJECT_NAME}
	${SRC_DIR}/kernel/main.c
	${SRC_DIR}/kernel/syscalls.c
)
target_link_libraries(${PROJECT_NAME}
	drivers
	CMSIS
	STM32f4xx_HAL_Driver
)
# Command for writing OS to target flash.
open_ocd_write_flash(${PROJECT_NAME})
