#include "hedder.h"

const queuing_port queuep_system_management[4] = {{
	 .portname = "Stat_2Dq",
	 .maxmessagesize = 30,
	 .direction = "DESTINATION",
	 .maxnbmessages = 30,
	},{
	 .portname = "Stat_3Dq",
	 .maxmessagesize = 30,
	 .direction = "DESTINATION",
	 .maxnbmessages = 30,
	},{
	 .portname = "Stat_4Dq",
	 .maxmessagesize = 30,
	 .direction = "DESTINATION",
	 .maxnbmessages = 30,
	},{
	 .portname = "Stat_5Dq",
	 .maxmessagesize = 30,
	 .direction = "DESTINATION",
	 .maxnbmessages = 30,
	},};

const queuing_port queuep_flight_controls[1] = {{
	 .portname = "Stat_2Sq",
	 .maxmessagesize = 30,
	 .direction = "SOURCE",
	 .maxnbmessages = 30,
	},};

const sampling_port samplep_flight_controls[4] = {{
	 .portname = "Act_1Ss",
	 .maxmessagesize = 20,
	 .direction = "SOURCE",
	 .refreshrateseconds = 0.100,
	},{
	 .portname = "Act_2Ss",
	 .maxmessagesize = 20,
	 .direction = "SOURCE",
	 .refreshrateseconds = 0.100,
	},{
	 .portname = "Sens_1Ds",
	 .maxmessagesize = 40,
	 .direction = "DESTINATION",
	 .refreshrateseconds = 0.100,
	},{
	 .portname = "Sens_2Ds",
	 .maxmessagesize = 40,
	 .direction = "DESTINATION",
	 .refreshrateseconds = 0.100,
	},};

const queuing_port queuep_flight_management[1] = {{
	 .portname = "Stat_3Sq",
	 .maxmessagesize = 30,
	 .direction = "SOURCE",
	 .maxnbmessages = 30,
	},};

const sampling_port samplep_flight_management[1] = {{
	 .portname = "Sens_2Ds",
	 .maxmessagesize = 40,
	 .direction = "DESTINATION",
	 .refreshrateseconds = 0.100,
	},};

const queuing_port queuep_IO_Processing[1] = {{
	 .portname = "Stat_4Sq",
	 .maxmessagesize = 30,
	 .direction = "SOURCE",
	 .maxnbmessages = 30,
	},};

const queuing_port queuep_IHVM[1] = {{
	 .portname = "Stat_5Sq",
	 .maxmessagesize = 30,
	 .direction = "SOURCE",
	 .maxnbmessages = 30,
	},};

const sampling_port samplep_IHVM[2] = {{
	 .portname = "Act_1Ds",
	 .maxmessagesize = 20,
	 .direction = "DESTINATION",
	 .refreshrateseconds = 0.100,
	},{
	 .portname = "Act_2Ds",
	 .maxmessagesize = 20,
	 .direction = "DESTINATION",
	 .refreshrateseconds = 0.100,
	},};

const partition partitions[5] = {{ 
	 .partitionidentifier = 1,
	 .partitionname = "system_management",
	 .criticality = "LEVEL_A",
	 .systempartion = true,
	 .entrypoint = "Initial",
	 .queue_arr = queuep_system_management,
	 .sample_arr = 0,
	},{ 
	 .partitionidentifier = 2,
	 .partitionname = "flight_controls",
	 .criticality = "LEVEL_A",
	 .systempartion = false,
	 .entrypoint = "Initial",
	 .queue_arr = queuep_flight_controls,
	 .sample_arr = samplep_flight_controls,
	},{ 
	 .partitionidentifier = 3,
	 .partitionname = "flight_management",
	 .criticality = "LEVEL_A",
	 .systempartion = false,
	 .entrypoint = "Initial",
	 .queue_arr = queuep_flight_management,
	 .sample_arr = samplep_flight_management,
	},{ 
	 .partitionidentifier = 4,
	 .partitionname = "IO_Processing",
	 .criticality = "LEVEL_A",
	 .systempartion = true,
	 .entrypoint = "Initial",
	 .queue_arr = queuep_IO_Processing,
	 .sample_arr = 0,
	},{ 
	 .partitionidentifier = 5,
	 .partitionname = "IHVM",
	 .criticality = "LEVEL_B",
	 .systempartion = false,
	 .entrypoint = "Initial",
	 .queue_arr = queuep_IHVM,
	 .sample_arr = samplep_IHVM,
	},};

const memory_requirements memoryp_system_management[3] = {{
	 .type = "CODE",
	 .sizebytes = 20000,
	 .access = "READ_ONLY",
	 .physicaladdress = "nope",
	},{
	 .type = "DATA",
	 .sizebytes = 20000,
	 .access = "READ_WRITE",
	 .physicaladdress = "nope",
	},{
	 .type = "INPUT_OUTPUT",
	 .sizebytes = 128000,
	 .access = "READ_WRITE",
	 .physicaladdress = "0xFF000000",
	},};

const memory_requirements memoryp_flight_controls[2] = {{
	 .type = "CODE",
	 .sizebytes = 25000,
	 .access = "READ_ONLY",
	 .physicaladdress = "nope",
	},{
	 .type = "DATA",
	 .sizebytes = 25000,
	 .access = "READ_WRITE",
	 .physicaladdress = "nope",
	},};

const memory_requirements memoryp_flight_management[2] = {{
	 .type = "CODE",
	 .sizebytes = 35000,
	 .access = "READ_ONLY",
	 .physicaladdress = "nope",
	},{
	 .type = "DATA",
	 .sizebytes = 25000,
	 .access = "READ_WRITE",
	 .physicaladdress = "nope",
	},};

const memory_requirements memoryp_IO_Processing[4] = {{
	 .type = "CODE",
	 .sizebytes = 50000,
	 .access = "READ_ONLY",
	 .physicaladdress = "nope",
	},{
	 .type = "DATA",
	 .sizebytes = 75000,
	 .access = "READ_WRITE",
	 .physicaladdress = "nope",
	},{
	 .type = "INPUT_OUTPUT",
	 .sizebytes = 256000,
	 .access = "READ_WRITE",
	 .physicaladdress = "0x50000000",
	},{
	 .type = "INPUT_OUTPUT",
	 .sizebytes = 512000,
	 .access = "READ_WRITE",
	 .physicaladdress = "0x80000000",
	},};

const memory_requirements memoryp_IHVM[2] = {{
	 .type = "CODE",
	 .sizebytes = 50000,
	 .access = "READ_ONLY",
	 .physicaladdress = "nope",
	},{
	 .type = "DATA",
	 .sizebytes = 100000,
	 .access = "READ_WRITE",
	 .physicaladdress = "nope",
	},};

const partition_memory partition_memorys[5] = {{ 
	 .partitionidentifier = 1,
	 .partitionname = "system_management",
	 .memory_arr = memoryp_system_management,
	},{ 
	 .partitionidentifier = 2,
	 .partitionname = "flight_controls",
	 .memory_arr = memoryp_flight_controls,
	},{ 
	 .partitionidentifier = 3,
	 .partitionname = "flight_management",
	 .memory_arr = memoryp_flight_management,
	},{ 
	 .partitionidentifier = 4,
	 .partitionname = "IO_Processing",
	 .memory_arr = memoryp_IO_Processing,
	},{ 
	 .partitionidentifier = 5,
	 .partitionname = "IHVM",
	 .memory_arr = memoryp_IHVM,
	},};

const window_schedule windowp_system_management[2] = {{
	 .windowidentifier = 101,
	 .windowstartseconds = 0.0,
	 .windowdurationseconds = 0.020,
	 .partitionperiodstart = true,
	},{
	 .windowidentifier = 102,
	 .windowstartseconds = 0.1,
	 .windowdurationseconds = 0.020,
	 .partitionperiodstart = true,
	},};

const window_schedule windowp_flight_controls[2] = {{
	 .windowidentifier = 201,
	 .windowstartseconds = 0.030,
	 .windowdurationseconds = 0.010,
	 .partitionperiodstart = true,
	},{
	 .windowidentifier = 202,
	 .windowstartseconds = 0.130,
	 .windowdurationseconds = 0.010,
	 .partitionperiodstart = true,
	},};

const window_schedule windowp_flight_management[2] = {{
	 .windowidentifier = 301,
	 .windowstartseconds = 0.040,
	 .windowdurationseconds = 0.030,
	 .partitionperiodstart = true,
	},{
	 .windowidentifier = 302,
	 .windowstartseconds = 0.140,
	 .windowdurationseconds = 0.030,
	 .partitionperiodstart = true,
	},};

const window_schedule windowp_IO_Processing[4] = {{
	 .windowidentifier = 401,
	 .windowstartseconds = 0.020,
	 .windowdurationseconds = 0.010,
	 .partitionperiodstart = true,
	},{
	 .windowidentifier = 402,
	 .windowstartseconds = 0.070,
	 .windowdurationseconds = 0.010,
	 .partitionperiodstart = false,
	},{
	 .windowidentifier = 403,
	 .windowstartseconds = 0.120,
	 .windowdurationseconds = 0.010,
	 .partitionperiodstart = true,
	},{
	 .windowidentifier = 404,
	 .windowstartseconds = 0.170,
	 .windowdurationseconds = 0.010,
	 .partitionperiodstart = false,
	},};

const window_schedule windowp_IHVM[1] = {{
	 .windowidentifier = 501,
	 .windowstartseconds = 0.180,
	 .windowdurationseconds = 0.020,
	 .partitionperiodstart = true,
	},};

const partition_schedule partition_schedules[5] = {{
	 .partitionidentifier = 1,
	 .partitionname = "system_management",
	 .peroidseconds = 0.100,
	 .perioddurationseconds = 0.020,
	 .window_arr = windowp_system_management,
	},{
	 .partitionidentifier = 2,
	 .partitionname = "flight_controls",
	 .peroidseconds = 0.100,
	 .perioddurationseconds = 0.010,
	 .window_arr = windowp_flight_controls,
	},{
	 .partitionidentifier = 3,
	 .partitionname = "flight_management",
	 .peroidseconds = 0.100,
	 .perioddurationseconds = 0.030,
	 .window_arr = windowp_flight_management,
	},{
	 .partitionidentifier = 4,
	 .partitionname = "IO_Processing",
	 .peroidseconds = 0.100,
	 .perioddurationseconds = 0.020,
	 .window_arr = windowp_IO_Processing,
	},{
	 .partitionidentifier = 5,
	 .partitionname = "IHVM",
	 .peroidseconds = 0.200,
	 .perioddurationseconds = 0.020,
	 .window_arr = windowp_IHVM,
	},};