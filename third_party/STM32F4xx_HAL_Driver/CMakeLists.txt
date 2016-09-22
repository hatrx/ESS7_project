file(GLOB src ${STM32f4xx_HAL_Driver_DIR}/Src/*.c)
add_library(STM32f4xx_HAL_Driver ${src})
target_compile_options(STM32f4xx_HAL_Driver PRIVATE -Wno-extra -Wno-pedantic) # There is a lot of unused variable warnings in some hal function-prototypes
