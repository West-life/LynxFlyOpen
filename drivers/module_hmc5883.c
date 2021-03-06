/*********************************************************************************
*                                山猫飞控（Lynx）
*                                 for LynxFly
*
* Version   	: V1.0
* By        	: Lynx@sia 84693469@qq.com
*
* For       	: Stm32f405RGT6
* Mode      	: Thumb2
* Description   : hmc5883驱动 抄袭来的
*
*				
* Date          : 2013.XX.XX
*******************************************************************************/
/*==============================================================================================*/
/*==============================================================================================*/
#include "stm32f4_i2c_dma.h"
#include "module_hmc5883.h"
#include <rtthread.h>
/*==============================================================================================*/
/*==============================================================================================*
**函數 : HMC5883_Init
**功能 : 初始化HMC5883
**輸入 : None
**輸出 : None
**使用 : HMC5883_Init()
**==============================================================================================*/
/*==============================================================================================*/
void HMC5883_Init( void )
{
	u8 HMC5883_Init_Data[3] = {
			0x70, /* HMC5883_REG_CONF_A */
			0xA0, /* HMC5883_REG_CONF_B */
			0x00  /* HMC5883_REG_MODE */
		};
	I2C_DMA_WriteReg(HMC5883_I2C_ADDR, HMC5883_REG_CONF_A, HMC5883_Init_Data,   1); rt_thread_delay(rt_tick_from_millisecond(10));
	I2C_DMA_WriteReg(HMC5883_I2C_ADDR, HMC5883_REG_CONF_B, HMC5883_Init_Data+1, 1); rt_thread_delay(rt_tick_from_millisecond(10));
	I2C_DMA_WriteReg(HMC5883_I2C_ADDR, HMC5883_REG_MODE,   HMC5883_Init_Data+2, 1); rt_thread_delay(rt_tick_from_millisecond(10));
}
/*==============================================================================================*/
/*==============================================================================================*/
