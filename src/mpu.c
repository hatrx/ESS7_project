#include <stm32f4xx_hal.h>

#include "mpu.h"


void init_mpu(long addr)
{
	HAL_MPU_Disable();

	MPU_Region_InitTypeDef MPU_InitStruct = {
		.Enable = MPU_REGION_ENABLE,
		.Number = MPU_REGION_NUMBER5,
		.BaseAddress = addr,
		.Size = MPU_REGION_SIZE_1KB,
		.SubRegionDisable = 0x00,
		.TypeExtField = MPU_TEX_LEVEL0,
		.AccessPermission = MPU_REGION_NO_ACCESS,
		.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE,
		.IsShareable = MPU_ACCESS_NOT_SHAREABLE,
		.IsCacheable = MPU_ACCESS_NOT_CACHEABLE,
		.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE,
	};

	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}
