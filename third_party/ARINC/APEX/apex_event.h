/*----------------------------------------------------------------*/
/*                                                                */
/* EVENT constant and type definitions and management services    */
/*                                                                */
/*----------------------------------------------------------------*/ 
 
#ifndef APEX_EVENT
#define APEX_EVENT 
 
#define  MAX_NUMBER_OF_EVENTS     SYSTEM_LIMIT_NUMBER_OF_EVENTS 
 
typedef  NAME_TYPE       EVENT_NAME_TYPE; 
 
typedef  APEX_INTEGER    EVENT_ID_TYPE; 
 
typedef  enum { DOWN = 0, UP = 1 } EVENT_STATE_TYPE; 
 
typedef
    struct {
        EVENT_STATE_TYPE    EVENT_STATE;
        WAITING_RANGE_TYPE  WAITING_PROCESSES;
    } EVENT_STATUS_TYPE; 
 
extern void CREATE_EVENT (
    /*in */ EVENT_NAME_TYPE          EVENT_NAME,
    /*out*/ EVENT_ID_TYPE            *EVENT_ID,
    /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ); 
 
extern void SET_EVENT (
    /*in */ EVENT_ID_TYPE            EVENT_ID,
    /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ); 
 
extern void RESET_EVENT (
    /*in */ EVENT_ID_TYPE            EVENT_ID,
    /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ); 
 
extern void WAIT_EVENT (
    /*in */ EVENT_ID_TYPE            EVENT_ID,
    /*in */ SYSTEM_TIME_TYPE         TIME_OUT,
    /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ); 
 
extern void GET_EVENT_ID (
    /*in */ EVENT_NAME_TYPE          EVENT_NAME,
    /*out*/ EVENT_ID_TYPE            *EVENT_ID,
    /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ); 
 
extern void GET_EVENT_STATUS (
    /*in */ EVENT_ID_TYPE            EVENT_ID,
    /*out*/ EVENT_STATUS_TYPE        *EVENT_STATUS,
    /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ); 
 
#endif