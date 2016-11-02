cmake_minimum_required(VERSION 2.8.8)

include(CMakeForceCompiler)
CMAKE_FORCE_C_COMPILER(arm-none-eabi-gcc GNU)


# Name of the OS CMake is building for.
set(CMAKE_SYSTEM_NAME Generic)


# Set a name, version, and enable languages for the entire project.
project(DemoBoard C ASM)


# Set all the compiler flags
set(gc_flags "-ffunction-sections -fdata-sections -Wl,--gc-sections") # These flags helps with dead code elimination. More info can found at http://stackoverflow.com/a/10809541
set(mcu_flags "-mcpu=cortex-m4 -mtune=cortex-m4 -mthumb -mlittle-endian -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb-interwork")
set(linker_flags "-Wl,-T${CMAKE_SOURCE_DIR}/STM32F415RG_FLASH.ld -Wl,-gc-sections -Wl,-LTO -nostartfiles")
set(WARNINGS "-Wall -Wextra")
set(CFLAGS "-O0 -g -ggdb -LTO -std=c99 ${WARNINGS} ${gc_flags} ${mcu_flags} ${linker_flags}")
set(CMAKE_C_FLAGS ${CFLAGS})


# Add definition
add_definitions(-DHSE_VALUE=16000000 -DSTM32F407xx)


# Define directories
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(CMSIS_DIR ${CMAKE_SOURCE_DIR}/third_party/CMSIS)
set(STM32f4xx_HAL_Driver_DIR ${CMAKE_SOURCE_DIR}/third_party/STM32F4xx_HAL_Driver)


# Use following dirs for header lookup
include_directories(
	# CMSIS
	${CMSIS_DIR}/Include
	${CMSIS_DIR}/Device/ST/STM32F4xx/Include

	# STM32 HAL
	${STM32f4xx_HAL_Driver_DIR}/Inc
)


# Make it possible to Flash using openOCD
find_program(OPEN_OCD openocd)
get_filename_component(open_ocd_path ${OPEN_OCD} DIRECTORY)
set(OPEN_OCD_CONFIG "${open_ocd_path}/../share/openocd/scripts/board/stm32f4discovery.cfg")
function(open_ocd_write_flash elf_file)
	add_custom_target(${elf_file}_writeflash
		COMMAND ${OPEN_OCD}
					-f ${OPEN_OCD_CONFIG}
					-c "init"
					-c "reset halt"
					-c "flash write_image erase ${elf_file}"
					-c "reset run"
		DEPENDS ${elf_file}
		VERBATIM
	)
endfunction()


# Compile CMSIS lib.
add_library(CMSIS
	${CMSIS_DIR}/Device/ST/STM32F4xx/Source/startup_stm32f415xx.s
	${CMSIS_DIR}/Device/ST/STM32F4xx/Source/system_stm32f4xx.c
)
target_include_directories(CMSIS PRIVATE ${CMSIS_INCLUDE_DIRS})
target_compile_options(CMSIS PRIVATE -Wno-extra -Wno-pedantic) # There is a lot of unused variable warnings in some hal function-prototypes


# Compile HAL lib.
file(GLOB src ${STM32f4xx_HAL_Driver_DIR}/Src/*.c)
add_library(STM32f4xx_HAL_Driver ${src})
target_compile_options(STM32f4xx_HAL_Driver PRIVATE -Wno-extra -Wno-pedantic) # There is a lot of unused variable warnings in some hal function-prototypes


# Compile OS
set(SRC_FILES
	${SRC_DIR}/main.c
	${SRC_DIR}/syscalls.c
	${SRC_DIR}/uart.c
	${SRC_DIR}/mpu.c
)
add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME}
	CMSIS
	STM32f4xx_HAL_Driver
)


# Command for writing OS to target flash.
open_ocd_write_flash(${PROJECT_NAME})
