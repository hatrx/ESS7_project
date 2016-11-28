#ifndef HEDDER_H
#define HEDDER_H

#include <stdbool.h>

typedef struct {
	 char portname[32];
	 int maxmessagesize;
	 char direction[32];
	 int maxnbmessages;
	} queuing_port;

typedef struct {
	 char portname[32];
	 int maxmessagesize;
	 char direction[32];
	 float refreshrateseconds;
	} sampling_port;

typedef struct{
	 int partitionidentifier;
	 char partitionname[32];
	 char criticality[32];
	 bool systempartion;
	 char entrypoint[32];
	 const queuing_port *queue_arr;
	 const sampling_port *sample_arr;
	 } partition;

typedef struct {
	 char type[32];
	 int sizebytes;
	 char access[32];
	 char physicaladdress[32];
	} memory_requirements;

typedef struct{
	 int partitionidentifier;
	 char partitionname[32];
	 const memory_requirements *memory_arr;
	} partition_memory;

typedef struct {
	 int windowidentifier;
	 float windowstartseconds;
	 float windowdurationseconds;
	 bool partitionperiodstart;
	} window_schedule;

typedef struct{
	 int partitionidentifier;
	 char partitionname[32];
	 float peroidseconds;
	 float perioddurationseconds;
	 const window_schedule *window_arr;
	} partition_schedule;


#endif // HEDDER_H
