/*----------------------------------------------------------------*/
/*                                                                */
/* PARTITION constant and type definitions and management services*/
/*                                                                */
/*----------------------------------------------------------------*/

#ifndef APEX_PARTITION
#define APEX_PARTITION 

#define  MAX_NUMBER_OF_PARTITIONS  SYSTEM_LIMIT_NUMBER_OF_PARTITIONS

typedef
    enum {
        IDLE       = 0,
        COLD_START = 1,
        WARM_START = 2,
        NORMAL     = 3
    } OPERATING_MODE_TYPE;

typedef  APEX_INTEGER    PARTITION_ID_TYPE;

typedef
    enum {
        NORMAL_START         = 0,
        PARTITION_RESTART    = 1,
        HM_MODULE_RESTART    = 2,
        HM_PARTITION_RESTART = 3
    } START_CONDITION_TYPE;

typedef
    struct {
        SYSTEM_TIME_TYPE     PERIOD;
        SYSTEM_TIME_TYPE     DURATION;
        PARTITION_ID_TYPE    IDENTIFIER;
        LOCK_LEVEL_TYPE      LOCK_LEVEL;
        OPERATING_MODE_TYPE  OPERATING_MODE;
        START_CONDITION_TYPE START_CONDITION;
    } PARTITION_STATUS_TYPE;



extern void GET_PARTITION_STATUS (
    /*out*/ PARTITION_STATUS_TYPE    *PARTITION_STATUS,
    /*out*/ RETURN_CODE_TYPE         *RETURN_CODE );

extern void SET_PARTITION_MODE (
    /*in */ OPERATING_MODE_TYPE      OPERATING_MODE,
    /*out*/ RETURN_CODE_TYPE         *RETURN_CODE );

#endif