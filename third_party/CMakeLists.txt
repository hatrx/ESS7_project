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
