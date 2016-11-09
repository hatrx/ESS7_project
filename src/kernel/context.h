#ifndef CONTEXT_H
#define CONTEXT_H


#include <stdint.h>


typedef struct {
	uint32_t R0;    //General purpose R0 Register
	uint32_t R1;    //General purpose R1 Register
	uint32_t R2;    //General purpose R2 Register
	uint32_t R3;    //General purpose R3 Register
	uint32_t R12;   //General purpose R12 Register
	uint32_t LR;    //Link Register
	uint32_t PC;    //Program Counter Register
	uint32_t PSR;   //Program Status Register
} ARM_HW_context_state;

typedef struct {
	uint32_t R4;    //General purpose R4 Register
	uint32_t R5;    //General purpose R5 Register
	uint32_t R6;    //General purpose R6 Register
	uint32_t R7;    //General purpose R7 Register
	uint32_t R8;    //General purpose R8 Register
	uint32_t R9;    //General purpose R9 Register
	uint32_t R10;   //General purpose R10 Register
	uint32_t R11;   //General purpose R11 Register
} ARM_SW_context_state;

typedef struct {
	ARM_SW_context_state sw_stack;
	ARM_HW_context_state hw_stack;
} ARM_context_state;


#endif /* CONTEXT_H */
