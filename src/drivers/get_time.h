#ifndef GET_TIME_H
#define GET_TIME_H

uint32_t start_time_ms();
uint32_t stop_time_ms();

float convert_cycles_to_time(unsigned int cycles);
void start_time();
float stop_time();
unsigned int getCycles();

#endif /* GET_TIME_H */