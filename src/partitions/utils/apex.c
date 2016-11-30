#include <apex_process.h>
#include <stm32f4xx_hal.h>


void CREATE_PROCESS (
    /*in */ PROCESS_ATTRIBUTE_TYPE   *ATTRIBUTES,
    /*out*/ PROCESS_ID_TYPE          *PROCESS_ID,
    /*out*/ RETURN_CODE_TYPE         *RETURN_CODE )
{
    __asm volatile (
        "MOV    R1, %0              \n\t"
        "MOVT   R0, #0xc0ff         \n\t"
        "MOVW   R0, #0xee0a         \n\t"
        "SVC    #0xa                \n\t"
        "NOP                        \n\t"
        "MOV    %[RETURN], R0       \n\t"
        "MOV    %[PID], R1          \n\t"
        : [RETURN] "=r" (*RETURN_CODE), [PID] "=r" (*PROCESS_ID)
        : [ATTR] "r" (ATTRIBUTES)
        :
    );
}

void GET_TIME (
    /*out*/ SYSTEM_TIME_TYPE         *SYSTEM_TIME,
    /*out*/ RETURN_CODE_TYPE         *RETURN_CODE )
{
    uint32_t time_lo, time_hi;
    __ASM volatile (
        "MOVW   R0, #0xee0b         \n\t"
        "MOVT   R0, #0xc0ff         \n\t"
        "SVC    #0x0F               \n\t"
        "MOV    %[RETURN], R0       \n\t"
        "MOV    %[TIME_LO], R1      \n\t"
        "MOV    %[TIME_HI], R2      \n\t"
        : [RETURN] "=r" (*RETURN_CODE), [TIME_LO] "=r" (time_lo), [TIME_HI] "=r" (time_hi)
        :
        :
    );
    *SYSTEM_TIME = (uint64_t) time_lo | (((uint64_t)time_hi) << 32);
}
