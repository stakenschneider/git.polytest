
#ifndef Temp_RH_H_
#define Temp_RH_H_

#include "terasic_includes.h"
#include "I2C.h"

#define R_TEMPERATURE      0x00
#define R_HUMIDITY         0x01
#define R_CONFIGURATION    0x02
#define R_MANUFACTURER_ID  0xFE
#define R_DEVICE_ID        0xFF

#define HDC1000_I2C_ADRESS 0x80 // 1000_000 + Write bit 0

bool RH_Temp_Sensor_init(void);
bool sensor_getdata();
bool Read_Configuration(void);
bool Write_Configuration(alt_u8 acquisition_mode , alt_u8 temp_resolution,alt_u8 humidity_resolution);


#endif /*Temp_RH_H_*/
