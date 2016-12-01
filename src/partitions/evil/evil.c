#include <stdint.h>

void evil_main(void)
{
    #ifdef EVIL_POINTER_ERROR
    uint32_t* ptr = 0x20000000;
    while(1)
    {
        *ptr = 0xaabbccee;
        ptr++;
    }
    #endif

    #ifdef EVIL_PROGRAM_COUNTER_ERROR
    asm volatile (
        "LDR PC, [PC,#4]    \n\t"
        ".word 0x8012000    \n\t"
        : : :
    );
    #endif
}