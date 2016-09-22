file(GLOB src ${FATFS_DIR}/src/*.c)
add_library(FATFS
    ${src}
    ${FATFS_DIR}/src/drivers/sd_diskio.c
)
target_compile_options(FATFS PRIVATE -Wno-unused-parameter -Wno-pedantic) # There is a lot of unused variable warnings in this library
