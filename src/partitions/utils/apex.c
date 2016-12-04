#include <stm32f4xx_hal.h>

#include <apex_process.h>
#include <apex_queuing.h>


void CREATE_PROCESS (
    /*in */ PROCESS_ATTRIBUTE_TYPE   *ATTRIBUTES,
    /*out*/ PROCESS_ID_TYPE          *PROCESS_ID,
    /*out*/ RETURN_CODE_TYPE         *RETURN_CODE )
{
    __asm volatile (
        "PUSH   {%[IN_1]}           \n\t"
		"PUSH   {%[IN_2]}           \n\t"
		"PUSH   {%[IN_3]}           \n\t"
		"MOV    R2, sp              \n\t"
        "MOVW   R0, #0xee0a         \n\t"
		"MOVT   R0, #0xc0ff         \n\t"
        "SVC    #0xa                \n\t"
        :
        : [IN_1] "r" (ATTRIBUTES),
		  [IN_2] "r" (PROCESS_ID),
		  [IN_3] "r" (RETURN_CODE)
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

void CREATE_QUEUING_PORT (
    /*in */ QUEUING_PORT_NAME_TYPE   QUEUING_PORT_NAME,
    /*in */ MESSAGE_SIZE_TYPE        MAX_MESSAGE_SIZE,
    /*in */ MESSAGE_RANGE_TYPE       MAX_NB_MESSAGE,
    /*in */ PORT_DIRECTION_TYPE      PORT_DIRECTION,
    /*in */ QUEUING_DISCIPLINE_TYPE  QUEUING_DISCIPLINE,
    /*out*/ QUEUING_PORT_ID_TYPE     *QUEUING_PORT_ID,
    /*out*/ RETURN_CODE_TYPE         *RETURN_CODE )
{
    __ASM volatile (
        "PUSH   {%[IN_1]}           \n\t"
        "PUSH   {%[IN_2]}           \n\t"
        "PUSH   {%[IN_3]}           \n\t"
        "PUSH   {%[IN_4]}           \n\t"
        "PUSH   {%[IN_5]}           \n\t"
        "PUSH   {%[IN_6]}           \n\t"
        "PUSH   {%[IN_7]}           \n\t"
        "MOV    R2, sp              \n\t"
        "MOVW   R0, #0xee0c         \n\t"
        "MOVT   R0, #0xc0ff         \n\t"
        "SVC    #0x0F               \n\t"
        :
        : [IN_1] "r" (QUEUING_PORT_NAME),
          [IN_2] "r" (MAX_MESSAGE_SIZE),
          [IN_3] "r" (MAX_NB_MESSAGE),
          [IN_4] "r" (PORT_DIRECTION),
          [IN_5] "r" (QUEUING_DISCIPLINE),
		  [IN_6] "r" (QUEUING_PORT_ID),
		  [IN_7] "r" (RETURN_CODE)
        :
    );
}


void RECEIVE_QUEUING_MESSAGE (
	/*in */ QUEUING_PORT_ID_TYPE     QUEUING_PORT_ID,
	/*in */ SYSTEM_TIME_TYPE         TIME_OUT,
	/*out*/ MESSAGE_ADDR_TYPE        MESSAGE_ADDR,
	/*out*/ MESSAGE_SIZE_TYPE        *LENGTH,
	/*out*/ RETURN_CODE_TYPE         *RETURN_CODE )
{
	__ASM volatile (
        "PUSH   {%[IN_1]}           \n\t"
        "PUSH   {%[IN_2]}           \n\t"
        "PUSH   {%[IN_3]}           \n\t"
        "PUSH   {%[IN_4]}           \n\t"
        "PUSH   {%[IN_5]}           \n\t"
        "MOV    R2, sp              \n\t"
        "MOVW   R0, #0xee0d         \n\t"
        "MOVT   R0, #0xc0ff         \n\t"
        "SVC    #0x0F               \n\t"
        :
        : [IN_1] "r" (QUEUING_PORT_ID),
          [IN_2] "r" (TIME_OUT),
          [IN_3] "r" (MESSAGE_ADDR),
          [IN_4] "r" (LENGTH),
          [IN_5] "r" (RETURN_CODE)
        :
    );
}


void SEND_QUEUING_MESSAGE (
	/*in */ QUEUING_PORT_ID_TYPE     QUEUING_PORT_ID,
	/*in */ MESSAGE_ADDR_TYPE        MESSAGE_ADDR,       /* by reference */
	/*in */ MESSAGE_SIZE_TYPE        LENGTH,
	/*in */ SYSTEM_TIME_TYPE         TIME_OUT,
	/*out*/ RETURN_CODE_TYPE         *RETURN_CODE)
{
	__ASM volatile (
        "PUSH   {%[IN_1]}           \n\t"
        "PUSH   {%[IN_2]}           \n\t"
        "PUSH   {%[IN_3]}           \n\t"
        "PUSH   {%[IN_4]}           \n\t"
        "PUSH   {%[IN_5]}           \n\t"
        "MOV    R2, sp              \n\t"
        "MOVW   R0, #0xee0e         \n\t"
        "MOVT   R0, #0xc0ff         \n\t"
        "SVC    #0x0F               \n\t"
        :
        : [IN_1] "r" (QUEUING_PORT_ID),
          [IN_2] "r" (MESSAGE_ADDR),
          [IN_3] "r" (LENGTH),
          [IN_4] "r" (TIME_OUT),
          [IN_5] "r" (RETURN_CODE)
        :
    );
}


void GET_QUEUING_PORT_ID (
	/*in */ QUEUING_PORT_NAME_TYPE   QUEUING_PORT_NAME,
	/*out*/ QUEUING_PORT_ID_TYPE     *QUEUING_PORT_ID,
	/*out*/ RETURN_CODE_TYPE         *RETURN_CODE )
{
	__ASM volatile (
        "PUSH   {%[IN_1]}           \n\t"
        "PUSH   {%[IN_2]}           \n\t"
        "PUSH   {%[IN_3]}           \n\t"
        "MOV    R2, sp              \n\t"
        "MOVW   R0, #0xee0f         \n\t"
        "MOVT   R0, #0xc0ff         \n\t"
        "SVC    #0x0F               \n\t"
        :
        : [IN_1] "r" (QUEUING_PORT_NAME),
          [IN_2] "r" (QUEUING_PORT_ID),
          [IN_3] "r" (RETURN_CODE)
        :
    );
}


void GET_QUEUING_PORT_STATUS (
	/*in */ QUEUING_PORT_ID_TYPE     QUEUING_PORT_ID,
	/*out*/ QUEUING_PORT_STATUS_TYPE *QUEUING_PORT_STATUS,
	/*out*/ RETURN_CODE_TYPE         *RETURN_CODE )
{
	__ASM volatile (
        "PUSH   {%[IN_1]}           \n\t"
        "PUSH   {%[IN_2]}           \n\t"
        "PUSH   {%[IN_3]}           \n\t"
        "MOV    R2, sp              \n\t"
        "MOVW   R0, #0xee10         \n\t"
        "MOVT   R0, #0xc0ff         \n\t"
        "SVC    #0x0F               \n\t"
        :
        : [IN_1] "r" (QUEUING_PORT_ID),
          [IN_2] "r" (QUEUING_PORT_STATUS),
          [IN_3] "r" (RETURN_CODE)
        :
    );
}


void STOP_SELF()
{
	__ASM volatile (
        "MOVW   R0, #0xee11         \n\t"
        "MOVT   R0, #0xc0ff         \n\t"
        "SVC    #0x0F               \n\t"
        :
        :
        :
    );
}
