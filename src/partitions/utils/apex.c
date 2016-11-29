#include <apex_process.h>

void CREATE_PROCESS (
    /*in */ PROCESS_ATTRIBUTE_TYPE   *ATTRIBUTES,
    /*out*/ PROCESS_ID_TYPE          *PROCESS_ID,
    /*out*/ RETURN_CODE_TYPE         *RETURN_CODE )
{
    __asm volatile (
        "MOV    R1, %0              \n\t"
        "MOV    R0, #0x0000000a     \n\t"
        "SVC                        \n\t"
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
    __asm volatile (
        "MOV    R0, #0xc0ffee0b     \n\t"
        "SVC                        \n\t"
        "NOP                        \n\t"
        "MOV    %[RETURN], R0       \n\t"
        "MOV    %[TIME], R1         \n\t"
        : [RETURN] "=r" (*RETURN_CODE), [TIME] "=r" (*SYSTEM_TIME)
        :
        :
    );
}