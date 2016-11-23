#ifndef GET_TIME_H
#define GET_TIME_H

uint16_t TIME_Start_ms();
uint16_t TIME_Stop_ms();

uint64_t convert_cycles_to_time(uint32_t cycles);
void TIME_Start_ns();
uint64_t TIME_Stop_ns();
uint32_t TIME_Get_Cycles();

#endif /* GET_TIME_H */