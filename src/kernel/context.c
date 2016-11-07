#include <stm32f4xx_hal.h>

#include "context.h"

void* context_saveContext(void* stackPointer)
{
    __ASM volatile (
        "MRS    %0, PSP             \n"
        "STMDB  %0!, {R4-R11}       \n"
        "MSR    PSP, %0             \n"
    : "=r" (stackPointer)
    );
    ARM_context_state * state = (ARM_context_state*) stackPointer;
    return stackPointer;
}

void context_restoreContext(void* stackPointer)
{
}