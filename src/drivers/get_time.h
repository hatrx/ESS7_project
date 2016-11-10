#ifndef GET_TIME_H
#define GET_TIME_H

uint16_t start_time_ms();
uint16_t stop_time_ms();

uint32_t convert_cycles_to_time(uint32_t cycles);
void start_time();
uint32_t stop_time();
uint32_t getCycles();

#endif /* GET_TIME_H */