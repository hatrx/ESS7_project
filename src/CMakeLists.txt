# Compile OS
set(SRC_FILES
	${SRC_DIR}/main.c
	${SRC_DIR}/syscalls.c
	${SRC_DIR}/uart.c
	${SRC_DIR}/mpu.c
	${SRC_DIR}/onboard_leds.c
	${SRC_DIR}/system_clock.c
)
add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME}
	CMSIS
	STM32f4xx_HAL_Driver
)
# Command for writing OS to target flash.
open_ocd_write_flash(${PROJECT_NAME})
