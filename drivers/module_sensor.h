#ifndef __MODULE_SENSOR_H__
#define __MODULE_SENSOR_H__

#include <board.h>
#include "module_mpu6050.h"
#include "module_ak8975.h"
#include "module_ms5611.h"


typedef struct {
	s16 X;
	s16 Y;
	s16 Z;
  s16 OffsetX;
  s16 OffsetY;
  s16 OffsetZ;
 	float TrueX;
	float TrueY;
	float TrueZ;
	float RawToTrue;  //�����ÿ����Ĳ������ȣ����ڹ淶������׼��λ��ע���������Ҫ�ó���
} Sensor;

typedef  struct{
	int (* initialize)(void);
	int (* read_imu_fast)(void);
	int (* read_imu_slow)(void);
	Sensor Acc;
	Sensor Gyr;
	Sensor Mag;
	MS5611_ST Bar;
}SYS_IMU_SENSOR_T;


extern SYS_IMU_SENSOR_T imu_sensor;



#endif //__BATTERY_H__
