#ifndef TIME_GET_H
#define TIME_GET_H

uint16_t TIME_Start_ms();
uint16_t TIME_Stop_ms();

int64_t convert_cycles_to_time(uint32_t cycles);
void TIME_Start_ns();
int64_t TIME_Stop_ns();
void TIME_Add_Count();
int64_t TIME_Get_Total();
uint32_t TIME_Get_Cycles();

#endif /* TIME_GET_H */