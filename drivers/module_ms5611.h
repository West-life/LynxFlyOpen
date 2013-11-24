/* #include "module_ms5611.h" */

#ifndef __MODULE_MS5611_H
#define __MODULE_MS5611_H

#include <board.h>
/*=====================================================================================================*/
/*=====================================================================================================*/
#define MS5611_I2C_ADDR           ((u8)0xEE)

#define	MS5611_ADC                ((u8)0x00)  // ADC Read
#define MS5611_RESET              ((u8)0x1E)  // RESET

#define	MS5611_D1_OSR_256         ((u8)0x40)  // 3 bytes
#define	MS5611_D1_OSR_512         ((u8)0x42)  // 3 bytes
#define	MS5611_D1_OSR_1024        ((u8)0x44)  // 3 bytes
#define	MS5611_D1_OSR_2048        ((u8)0x46)  // 3 bytes
#define	MS5611_D1_OSR_4096        ((u8)0x48)  // 3 bytes
#define	MS5611_D2_OSR_256         ((u8)0x50)  // 3 bytes
#define	MS5611_D2_OSR_512         ((u8)0x52)  // 3 bytes
#define	MS5611_D2_OSR_1024        ((u8)0x54)  // 3 bytes
#define	MS5611_D2_OSR_2048        ((u8)0x56)  // 3 bytes
#define	MS5611_D2_OSR_4096        ((u8)0x58)  // 3 bytes

#define MS5611_PROM_COEFF_1       ((u8)0xA2)  // 2 bytes
#define MS5611_PROM_COEFF_2       ((u8)0xA4)  // 2 bytes
#define MS5611_PROM_COEFF_3       ((u8)0xA6)  // 2 bytes
#define MS5611_PROM_COEFF_4       ((u8)0xA8)  // 2 bytes
#define MS5611_PROM_COEFF_5       ((u8)0xAA)  // 2 bytes
#define MS5611_PROM_COEFF_6       ((u8)0xAC)  // 2 bytes

#define POW_2_7     (128)
#define POW_2_8     (256)
#define POW_2_15    (32768)
#define POW_2_16    (65536)
#define POW_2_21    (2097152)
#define POW_2_23    (8388608)

// 1010~1000  �W��100m �U��10.5mbar
// 1000~900   �W��100m �U��11.2mbar
// 900~800    �W��100m �U��12.2mbar
// 800~700    �W��100m �U��13.3mbar
// 700~600    �W��100m �U��15mbar
/*=====================================================================================================*/
/*=====================================================================================================*/
typedef struct {
	u16 C[7];
  u32 D[3];

  s32 dT;
  s32 rTemp;

  uint64_t OFF;
  uint64_t SENS;
  s32 rPress;

  float Temp;   // -4000~8500, 0.01'C/LSB
  float Press;  // 1000~120000, 0.01mbar/LSB
  float High;   // 0.01mbar ~ 9.5238cm(10.5mbar/100m) or 8.92857(11.2mbar/100m)
} MS5611_ST;
/*=====================================================================================================*/
/*=====================================================================================================*/
extern MS5611_ST Baro;
void MS5611_Init( MS5611_ST* );
void MS5611_ReadADC( MS5611_ST* );
void MS5611_Calculate( MS5611_ST* );
/*=====================================================================================================*/
/*=====================================================================================================*/
#endif
