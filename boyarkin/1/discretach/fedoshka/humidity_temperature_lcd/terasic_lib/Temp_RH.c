#include <unistd.h>
#include <string.h>
#include <io.h>
#include <stdio.h>
#include "system.h"
#include "Temp_RH.h"

#include "terasic_includes.h"
#include "I2C.h"


bool RH_Temp_Sensor_init(void)
{
	bool bPass;
    alt_u16 data;
	bPass = I2C_Read16(RH_TEMP_I2C_SCL_BASE,RH_TEMP_I2C_SDA_BASE,HDC1000_I2C_ADRESS,R_MANUFACTURER_ID,&data);
    if(bPass){
		printf("Manufacturer ID is %x\r\n",data);// 0x5449 expected
    }else{
	    printf("I2C MultipleRead Failed!\r\n");
    }

    bPass = I2C_Read16(RH_TEMP_I2C_SCL_BASE,RH_TEMP_I2C_SDA_BASE,HDC1000_I2C_ADRESS,R_DEVICE_ID,&data);
    if(bPass){
	    printf("Device ID is %x\r\n",data); // 0x1000 expected
    }else{
	    printf("I2C Read16 Failed!\r\n");
    }

    bPass =  Read_Configuration();
    if(!bPass){
		printf("I2C Read_Configuration Failed!\r\n");
	}
	
	bPass =  Write_Configuration(1,0,0);
    if(!bPass){
		printf("[Note] Set Configuration Failed!\r\n");
	}	
	
	return bPass;
	
}



bool sensor_getdata()
{
	bool    bPass;
	alt_u8  rh_temp[4];
	alt_u16 data;
	bPass =  I2C_MultipleRead(RH_TEMP_I2C_SCL_BASE,RH_TEMP_I2C_SDA_BASE,HDC1000_I2C_ADRESS,R_TEMPERATURE,rh_temp,4);
    if(bPass){
		data = (rh_temp[0] << 8) | rh_temp[1] ;
		data = (rh_temp[0] << 8) | rh_temp[1] ;
		temp = (double)data/397.188 -40.0;
		//  data/2^16 * 165 - 40
    	printf("[Together] Temperature: %.2f*C \n",temp);

		data = (rh_temp[2] << 8) | rh_temp[3] ;
    	rh = (double)((double)data/65536)*100;
    	//  data/2^16 * 165 - 40
    	printf("[Together] Relative Humidity: %.2f%% \n",rh);

    }else{
		printf("[Together] Read Temperature and Relative Humidity failed! \n");
    	   }
	return bPass;	   
}

bool Read_Configuration(void)
{
	bool bPass;
	alt_u16 data;

	bPass = I2C_Read16(RH_TEMP_I2C_SCL_BASE,RH_TEMP_I2C_SDA_BASE,HDC1000_I2C_ADRESS,R_CONFIGURATION,&data);

	if(bPass){

		printf("Read Configuration Data = 0x%x \r\n",data);

		if(((data>>12)&0x0001) == 0) printf("Acquisition mode:Temperature or Humidity is acquired.\r\n ");
		else printf("Acquisition mode:Temperature and Humidity are acquired in sequence,Temperature first.\r\n ");

		if(((data>>10)&0x0001) == 0) printf("Temperature Measurement Resolution: 14 bit.\r\n ");
		else  printf("Temperature Measurement Resolution: 11 bit.\r\n ");

		if(((data>> 8)&0x0003) == 0) printf("Humidity Measurement Resolution: 14 bit.\r\n ");
		else if(((data>> 8)&0x0003) == 1) printf("Humidity Measurement Resolution: 11 bit.\r\n ");
		else  printf("Humidity Measurement Resolution: 8 bit.\r\n ");

	}

	return bPass;
}



bool Write_Configuration(alt_u8 acquisition_mode , alt_u8 temp_resolution,alt_u8 humidity_resolution)
{
	bool bPass;
	alt_u16 data;

	data = ((acquisition_mode&0x0001) <<12) + ((temp_resolution&0x0001)<<10) + ((humidity_resolution&0x0003)<<8);

//	printf("Write Configuration Data = 0x%x \r\n",data);

	bPass = I2C_Write16(RH_TEMP_I2C_SCL_BASE,RH_TEMP_I2C_SDA_BASE,HDC1000_I2C_ADRESS,R_CONFIGURATION,data);

	return bPass;
}


