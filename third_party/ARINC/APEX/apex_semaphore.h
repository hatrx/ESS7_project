/*----------------------------------------------------------------*/
/*                                                                */
/* SEMAPHORE constant and type definitions and management services*/
/*                                                                */
/*----------------------------------------------------------------*/ 
 
#ifndef APEX_SEMAPHORE
#define APEX_SEMAPHORE 
 
#define  MAX_NUMBER_OF_SEMAPHORES  SYSTEM_LIMIT_NUMBER_OF_SEMAPHORES 
 
#define  MAX_SEMAPHORE_VALUE       32767 
 
typedef  NAME_TYPE       SEMAPHORE_NAME_TYPE; 
 
typedef  APEX_INTEGER    SEMAPHORE_ID_TYPE; 
 
typedef  APEX_INTEGER    SEMAPHORE_VALUE_TYPE; 
 
typedef
    struct {
        SEMAPHORE_VALUE_TYPE  CURRENT_VALUE;
        SEMAPHORE_VALUE_TYPE  MAXIMUM_VALUE;
        WAITING_RANGE_TYPE    WAITING_PROCESSES;
    } SEMAPHORE_STATUS_TYPE; 
 
 
 
extern void CREATE_SEMAPHORE (
    /*in */ SEMAPHORE_NAME_TYPE      SEMAPHORE_NAME,
    /*in */ SEMAPHORE_VALUE_TYPE     CURRENT_VALUE,
    /*in */ SEMAPHORE_VALUE_TYPE     MAXIMUM_VALUE,
    /*in */ QUEUING_DISCIPLINE_TYPE  QUEUING_DISCIPLINE,
    /*out*/ SEMAPHORE_ID_TYPE        *SEMAPHORE_ID,
    /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ); 
 
extern void WAIT_SEMAPHORE (
    /*in */ SEMAPHORE_ID_TYPE        SEMAPHORE_ID,
    /*in */ SYSTEM_TIME_TYPE         TIME_OUT,
    /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ); 
 
extern void SIGNAL_SEMAPHORE (
    /*in */ SEMAPHORE_ID_TYPE        SEMAPHORE_ID,
    /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ); 
 
extern void GET_SEMAPHORE_ID (
    /*in */ SEMAPHORE_NAME_TYPE      SEMAPHORE_NAME,
    /*out*/ SEMAPHORE_ID_TYPE        *SEMAPHORE_ID,
    /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ); 
 
extern void GET_SEMAPHORE_STATUS (
    /*in */ SEMAPHORE_ID_TYPE        SEMAPHORE_ID,
    /*out*/ SEMAPHORE_STATUS_TYPE    *SEMAPHORE_STATUS,
    /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ); 
 
#endif