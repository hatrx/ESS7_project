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

add_library(yellow_toggler
	${SRC_DIR}/partitions/yellow_toggler/yellow_toggler.c
)
target_link_libraries(yellow_toggler
	drivers
	APEX
)


add_library(red_toggler
	${SRC_DIR}/partitions/red_toggler/red_toggler.c
)
target_link_libraries(red_toggler
	drivers
	APEX
)


add_library(stdio_sys
	${SRC_DIR}/partitions/stdio_sys/stdio_sys.c
)
target_link_libraries(stdio_sys
	drivers
	APEX
)

add_library(idle_sys
	${SRC_DIR}/partitions/idle_sys/idle_sys.s
)
target_link_libraries(idle_sys
)

add_library(evil
	${SRC_DIR}/partitions/evil/evil.c
)
target_link_libraries(evil
	drivers
	APEX
)


# Compile OS
add_executable(${PROJECT_NAME}
	${SRC_DIR}/xml_data.c
	${SRC_DIR}/kernel/main.c
	${SRC_DIR}/kernel/syscalls.c
	${SRC_DIR}/kernel/context.c
	${SRC_DIR}/kernel/part_scheduler.c
	${SRC_DIR}/kernel/svc_handler.c
	${SRC_DIR}/kernel/partition.c
	${SRC_DIR}/kernel/process.c
	${SRC_DIR}/kernel/queuing_port.c
	${SRC_DIR}/kernel/sampling_port.c
	${SRC_DIR}/kernel/circular_buffer.c
	${SRC_DIR}/kernel/error_handlers.c
)
target_link_libraries(${PROJECT_NAME}
	drivers
	CMSIS
	STM32f4xx_HAL_Driver

	yellow_toggler
	red_toggler
	stdio_sys
	idle_sys
	evil
)
# Command for writing OS to target flash.
open_ocd_write_flash(${PROJECT_NAME})
