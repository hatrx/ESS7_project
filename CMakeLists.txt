cmake_minimum_required(VERSION 2.8.8)

include(CMakeForceCompiler)
CMAKE_FORCE_C_COMPILER(arm-none-eabi-gcc GNU)


# Name of the OS CMake is building for.
set(CMAKE_SYSTEM_NAME Generic)


# Set a name, version, and enable languages for the entire project.
project(OS C ASM)


# Define directories
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(ARINC_DIR ${CMAKE_SOURCE_DIR}/third_party/ARINC)
set(CMSIS_DIR ${CMAKE_SOURCE_DIR}/third_party/CMSIS)
set(STM32f4xx_HAL_Driver_DIR ${CMAKE_SOURCE_DIR}/third_party/STM32F4xx_HAL_Driver)
set(LINKER_SCRIPTS_DIR ${CMAKE_SOURCE_DIR}/third_party/linker_scripts)


# Set all the compiler flags
set(gc_flags "-ffunction-sections -fdata-sections -Wl,--gc-sections") # These flags helps with dead code elimination. More info can found at http://stackoverflow.com/a/10809541
set(mcu_flags "-mcpu=cortex-m4 -mtune=cortex-m4 -mthumb -mlittle-endian -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb-interwork")
set(linker_flags "-Wl,-T${LINKER_SCRIPTS_DIR}/STM32F415RG_FLASH.ld -Wl,-gc-sections -Wl,-LTO -nostartfiles -Wl,-Map=./result.map")
set(WARNINGS "-Wall -Wextra")
set(CFLAGS "-O0 -g -ggdb -LTO -std=c99 ${WARNINGS} ${gc_flags} ${mcu_flags} ${linker_flags}")
set(CMAKE_C_FLAGS ${CFLAGS})


# Add definition
add_definitions(-DHSE_VALUE=16000000 -DSTM32F415xx)


# Use following dirs for header lookup
include_directories(
	# SRC
	${SRC_DIR}

	# ARINC APEX
	${ARINC_DIR}/APEX

	# CMSIS
	${CMSIS_DIR}/Include
	${CMSIS_DIR}/Device/ST/STM32F4xx/Include

	# STM32 HAL
	${STM32f4xx_HAL_Driver_DIR}/Inc

	# APEX
	third_party/ARINC
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

add_subdirectory(third_party)
add_subdirectory(src)
