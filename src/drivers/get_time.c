#include <stm32f4xx_hal.h>
#include "apex_time.h"
#include "apex_types.h"
#include "time_get.h"

void GET_TIME(SYSTEM_TIME_TYPE *SYSTEM_TIME,  RETURN_CODE_TYPE *RETURN_CODE)
{
	*SYSTEM_TIME = TIME_Get_Total();
	*RETURN_CODE = NO_ERROR;
}