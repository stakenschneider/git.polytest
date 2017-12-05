// --------------------------------------------------------------------
// Copyright (c) 2007 by Terasic Technologies Inc. 
// --------------------------------------------------------------------
//
// Permission:
//
//   Terasic grants permission to use and modify this code for use
//   in synthesis for all Terasic Development Boards and Altera Development 
//   Kits made by Terasic.  Other use of this code, including the selling 
//   ,duplication, or modification of any portion is strictly prohibited.
//
// Disclaimer:
//
//   This VHDL/Verilog or C/C++ source code is intended as a design reference
//   which illustrates how these types of functions can be implemented.
//   It is the user's responsibility to verify their design for
//   consistency and functionality through the use of formal
//   verification methods.  Terasic provides no warranty regarding the use 
//   or functionality of this code.
//
// --------------------------------------------------------------------
//           
//                     Terasic Technologies Inc
//                     356 Fu-Shin E. Rd Sec. 1. JhuBei City,
//                     HsinChu County, Taiwan
//                     302
//
//                     web: http://www.terasic.com/
//                     email: support@terasic.com
//
// --------------------------------------------------------------------
#include "terasic_includes.h"
#include "I2C.h"

// Note. Remember to reset device befroe acceess I2C interface
#ifdef DEBUG_I2C
    #define I2C_DEBUG(x)    DEBUG(x)  
#else
    #define I2C_DEBUG(x)
#endif


//#define E2_I2C_SCL_BASE         PIO_ID_EEPROM_SCL_BASE
//#define E2_I2C_DAT_BASE         PIO_ID_EEPROM_DAT_BASE

//void dump_message(char *pMessage);
//#define DEBUG_DUMP dump_message

#define SDA_DIR_IN(data_base)   IOWR_ALTERA_AVALON_PIO_DIRECTION(data_base,0)
#define SDA_DIR_OUT(data_base)  IOWR_ALTERA_AVALON_PIO_DIRECTION(data_base,1) 
#define SDA_HIGH(data_base)     IOWR_ALTERA_AVALON_PIO_DATA(data_base, 1)
#define SDA_LOW(data_base)      IOWR_ALTERA_AVALON_PIO_DATA(data_base, 0)
#define SDA_READ(data_base)     IORD_ALTERA_AVALON_PIO_DATA(data_base)
#define SCL_HIGH(clk_base)     IOWR_ALTERA_AVALON_PIO_DATA(clk_base, 1)
#define SCL_LOW(clk_base)      IOWR_ALTERA_AVALON_PIO_DATA(clk_base, 0)

#define SCL_DELAY    usleep(2)

#define SLEEP_TIME 15  //us

void i2c_start(alt_u32 clk_base, alt_u32 data_base);
void i2c_stop(alt_u32 clk_base, alt_u32 data_base);
bool i2c_write(alt_u32 clk_base, alt_u32 data_base, alt_u8 Data);
void i2c_read(alt_u32 clk_base, alt_u32 data_base, alt_u8 *pData, bool bAck);

bool oc_i2c_init(alt_32 i2c_base)
{
	bool bSuccess=TRUE;
	alt_32 read_data;
    // I2c sysclock =50M hz
	// i2c scl max 400k
	//scl =sysclock/(prescale*5)
   IOWR(i2c_base, 0,0x18);//write high prescale
   IOWR(i2c_base, 1,0x00);//write low prescale
   //enable the I2C core, but disable the IRQ
   IOWR( i2c_base, 2,0x80);

    read_data =IORD(i2c_base, 0);
    if( (read_data & 0x00ff) != 0x18)
    {
    	bSuccess = FALSE;
       	read_data =IORD(i2c_base, 1);
		if((read_data & 0x00ff) != 0x00){
			bSuccess = FALSE;
		}
    }
    else if (bSuccess){
    	read_data =IORD(i2c_base, 2);
        if( (read_data & 0x00ff) != 0x80 ){
            bSuccess = FALSE;
        }
    }

   if (bSuccess)
       printf("\nI2C core is enabled! \r\n");
   else
       printf("\nI2C core is not enabled! \r\n");

   return bSuccess;

}

bool I2C_setchannel(alt_u32 clk_base, alt_u32 data_base, alt_8 DeviceAddr, alt_u8 ControlAddr, alt_u8 ControlData){
    bool bSuccess = TRUE;
    //alt_u8 DeviceAddr;
    
    // PCA9543APW set Channel

    i2c_start(clk_base, data_base);
    if (!i2c_write(clk_base, data_base, DeviceAddr)){  // send ID
        bSuccess = FALSE;
        I2C_DEBUG(("I2C HMB_E2 Fail: Address NACK!\n"));
    }
    if (bSuccess && !i2c_write(clk_base, data_base, ControlAddr)){ // send sub-address
        bSuccess = FALSE;
        I2C_DEBUG(("I2C HMB_E2 Fail: SubAddress NACK!\n"));
    }            
//    if (bSuccess && !i2c_write(clk_base, data_base, ControlData)){  
//        bSuccess = FALSE;
//        I2C_DEBUG(("I2C HMB_E2 Fail: write NACK!\n"));
//    }
    i2c_stop(clk_base, data_base);
    
    usleep(7*1000); // delay to wait EE2 ready (at least 5 ms delay is required)
    
    return bSuccess;

    
}

bool I2C_Write(alt_u32 clk_base, alt_u32 data_base, alt_8 DeviceAddr, alt_u8 ControlAddr, alt_u8 ControlData){
    bool bSuccess = TRUE;
    //alt_u8 DeviceAddr;
    
    // device id
    //DeviceAddr = HMB_E2_I2C_ID;

    i2c_start(clk_base, data_base);
    if (!i2c_write(clk_base, data_base, DeviceAddr)){  // send ID
        bSuccess = FALSE;
        I2C_DEBUG(("I2C HMB_E2 Fail: Address NACK!\n"));
    }
    if (bSuccess && !i2c_write(clk_base, data_base, ControlAddr)){ // send sub-address
        bSuccess = FALSE;
        I2C_DEBUG(("I2C HMB_E2 Fail: SubAddress NACK!\n"));
    }            
    if (bSuccess && !i2c_write(clk_base, data_base, ControlData)){  
        bSuccess = FALSE;
        I2C_DEBUG(("I2C HMB_E2 Fail: write NACK!\n"));
    }
    i2c_stop(clk_base, data_base);
    
    usleep(7*1000); // delay to wait EE2 ready (at least 5 ms delay is required)
    
    return bSuccess;

    
}

bool I2C_Write16(alt_u32 clk_base, alt_u32 data_base, alt_8 DeviceAddr, alt_u8 ControlAddr, alt_u16 ControlData){
    bool bSuccess = TRUE;
    alt_u8 HighByte, LowByte;
    //alt_u8 DeviceAddr;
    HighByte = (ControlData >> 8) & 0xFF;
    LowByte = ControlData & 0xFF; 
    
    // device id
    //DeviceAddr = HMB_E2_I2C_ID;

    i2c_start(clk_base, data_base);
    if (!i2c_write(clk_base, data_base, DeviceAddr)){  // send ID
        bSuccess = FALSE;
        I2C_DEBUG(("I2C HMB_E2 Fail: Address NACK!\n"));
    }
    if (bSuccess && !i2c_write(clk_base, data_base, ControlAddr)){ // send sub-address
        bSuccess = FALSE;
        I2C_DEBUG(("I2C HMB_E2 Fail: SubAddress NACK!\n"));
    }            
    if (bSuccess && !i2c_write(clk_base, data_base, HighByte)){  
        bSuccess = FALSE;
        I2C_DEBUG(("I2C HMB_E2 Fail: write NACK!\n"));
    }
    if (bSuccess && !i2c_write(clk_base, data_base, LowByte)){  
        bSuccess = FALSE;
        I2C_DEBUG(("I2C HMB_E2 Fail: write NACK!\n"));
    }    
    i2c_stop(clk_base, data_base);
    
    usleep(7*1000); // delay to wait EE2 ready (at least 5 ms delay is required)
    
    return bSuccess;

    
}

bool I2C_Read(alt_u32 clk_base, alt_u32 data_base, alt_8 DeviceAddr, alt_u8 ControlAddr, alt_u8 *pControlData){
    bool bSuccess = TRUE;
    //alt_u8 DeviceAddr;
   
    // device id
    //DeviceAddr = HMB_E2_I2C_ID;

    i2c_start(clk_base, data_base);
    if (!i2c_write(clk_base, data_base, DeviceAddr)){  // send ID
        bSuccess = FALSE;
        I2C_DEBUG(("I2C HMB_E2 Fail: Address NACK!\n"));
    }
    if (bSuccess && !i2c_write(clk_base, data_base, ControlAddr)){ // send sub-address
        bSuccess = FALSE;
        I2C_DEBUG(("I2C HMB_E2 Fail: SubAddress NACK!\n"));
    }            
    i2c_start(clk_base, data_base);  // restart
    DeviceAddr |= 1; // Read
    if (bSuccess && !i2c_write(clk_base, data_base, DeviceAddr)){  // send id
        bSuccess = FALSE;
        I2C_DEBUG(("I2C HMB_E2 Fail: Address+1 NACK!\n"));
    }
    
    if (bSuccess){
        i2c_read(clk_base, data_base, pControlData, FALSE);  // read
    }        
    i2c_stop(clk_base, data_base);
    
    return bSuccess;
}

bool I2C_Read16(alt_u32 clk_base, alt_u32 data_base, alt_8 DeviceAddr, alt_u8 ControlAddr, alt_u16 *pControlData){
    bool bSuccess = TRUE;
    alt_u8 LowByte, HighByte;
    //alt_u8 DeviceAddr;
   
    // device id
    //DeviceAddr = HMB_E2_I2C_ID;

    i2c_start(clk_base, data_base);
    if (!i2c_write(clk_base, data_base, DeviceAddr)){  // send ID
        bSuccess = FALSE;
        I2C_DEBUG(("I2C HMB_E2 Fail: Address NACK!\n"));
    }
    if (bSuccess && !i2c_write(clk_base, data_base, ControlAddr)){ // send sub-address
        bSuccess = FALSE;
        I2C_DEBUG(("I2C HMB_E2 Fail: SubAddress NACK!\n"));
    }            

    usleep(15*1000); // Conversation time delay for Humidity/Temperature Sensor

    i2c_start(clk_base, data_base);  // restart
    DeviceAddr |= 1; // Read
    if (bSuccess && !i2c_write(clk_base, data_base, DeviceAddr)){  // send id
        bSuccess = FALSE;
        I2C_DEBUG(("I2C HMB_E2 Fail: Address+1 NACK!\n"));
    }
    
    if (bSuccess){
        i2c_read(clk_base, data_base, &HighByte, TRUE);  // read
    }        
    if (bSuccess){
        i2c_read(clk_base, data_base, &LowByte, FALSE);  // read
    }        
    if (bSuccess)
        *pControlData = (HighByte << 8) | LowByte;
    i2c_stop(clk_base, data_base);
    
    return bSuccess;
}

bool I2C_MultipleRead(alt_u32 clk_base, alt_u32 data_base, alt_8 DeviceAddr, alt_u8 ControlAddr, alt_u8 szData[], alt_u16 len){
    int i;
    bool bSuccess = TRUE;
    //alt_u8 DeviceAddr, 
//    alt_u8 ControlAddr = 0;
    
   
    // device id
    //DeviceAddr = HMB_E2_I2C_ID;

    i2c_start(clk_base, data_base);
    if (!i2c_write(clk_base, data_base, DeviceAddr)){  // send ID
        bSuccess = FALSE;
        I2C_DEBUG(("I2C HMB_E2 Fail: Address NACK!\n"));
    }
    if (bSuccess && !i2c_write(clk_base, data_base, ControlAddr)){ // send sub-address
        bSuccess = FALSE;
        I2C_DEBUG(("I2C HMB_E2 Fail: SubAddress NACK!\n"));
    }

    usleep(15*1000); // Conversation time delay for Humidity/Temperature Sensor

    if (bSuccess)        
    i2c_start(clk_base, data_base);  // restart
    DeviceAddr |= 1; // Read
    if (bSuccess && !i2c_write(clk_base, data_base, DeviceAddr)){  // send id
        bSuccess = FALSE;
        I2C_DEBUG(("I2C HMB_E2 Fail: Address+1 NACK!\n"));
    }
    
    if (bSuccess){
        for(i=0;i<len && bSuccess;i++){
            i2c_read(clk_base, data_base, &szData[i], (i==(len-1))?FALSE:TRUE);  // read
        }            
    }        
    i2c_stop(clk_base, data_base);
    
    return bSuccess;    
    
}




/////////////////////////////////////////////////////////////////////////////////////////////
///////////// Interncal function (i2cXXX) body //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


//SDA 1->0 while SCL=1
void i2c_start(alt_u32 clk_base, alt_u32 data_base){
    
    // make sure it is in normal state
    SDA_DIR_OUT(data_base);  // data output enabled
    
    
    
    // start condition
    SDA_HIGH(data_base); // data high
    SCL_HIGH(clk_base);
    SCL_DELAY;
     
    SDA_LOW(data_base); // data low
    SCL_DELAY; 
    SCL_LOW(clk_base); // clock low
    SCL_DELAY;
}

// SDA 0->1 while SCL=1
void i2c_stop(alt_u32 clk_base, alt_u32 data_base){
    // assume SCL = 0
    
    SDA_DIR_OUT(data_base);  // data output enabled
    SDA_LOW(data_base); // Data Low
    //SCL_DELAY; 
    SCL_HIGH(clk_base);  // clock high
    SCL_DELAY; // clock high long delay
    SDA_HIGH(data_base); // data high
    SCL_DELAY; // data high delay
    

    
}

bool i2c_write(alt_u32 clk_base, alt_u32 data_base, alt_u8 Data){ // return true if device response ack
    alt_u8 Mask = 0x80;
    bool bAck;
    int i;
    
    // assume, SCL = 0
    
    SDA_DIR_OUT(data_base);  // data write mode
    
    for(i=0;i<8;i++){
        SCL_LOW(clk_base);  // new, make sure data change at clk low
        // output data on bus
        if (Data & Mask){ // there is a delay in this command
            SDA_HIGH(data_base);
        }else{    
            SDA_LOW(data_base);
        }
        Mask >>= 1; // there is a delay in this command
        // clock high
        SCL_HIGH(clk_base);
        SCL_DELAY;
        SCL_LOW(clk_base);
        SCL_DELAY;
    }
    
    //===== get ack
    SDA_DIR_IN(data_base);  // data read mode
    //SCL_DELAY;
    // clock high
    SCL_HIGH(clk_base);  // clock high
    SCL_DELAY;  // clock high delay
    bAck = SDA_READ(data_base)?FALSE:TRUE;  // get ack
    //SCL_DELAY;
    //SDA_DIR_OUT;
    SCL_LOW(clk_base); // clock low         
    SCL_DELAY; // clock low delay
    return bAck;
}    

void i2c_read(alt_u32 clk_base, alt_u32 data_base, alt_u8 *pData, bool bAck){ // return true if device response ack
    alt_u8 Data=0;
    int i;
    
    // assume SCL = low
    
    SDA_DIR_IN(data_base);  // set data read mode
    SCL_LOW(clk_base); // clock low
    SCL_DELAY; // clock low delay

    for(i=0;i<8;i++){
        Data <<= 1;
        SCL_HIGH(clk_base);  // clock high
        SCL_DELAY;
        if (SDA_READ(data_base))  // read data   
            Data |= 0x01;
        SCL_LOW(clk_base);  // clock log  
        SCL_DELAY;
    }
    
    // send ACK
    SCL_LOW(clk_base);  // new, make sure data change at clk low
    SDA_DIR_OUT(data_base);  // set data write mode
    if (bAck)
        SDA_LOW(data_base);
    else
        SDA_HIGH(data_base);
    SCL_HIGH(clk_base); // clock high
    SCL_DELAY; // clock high  delay
    SCL_LOW(clk_base); // clock low
    SCL_DELAY; // clock low delay
    SDA_LOW(data_base);  // data low
    SCL_DELAY; // data low delay
//    SDA_DIR_IN;  // set data read mode
    
    *pData = Data;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
// size > 2Kbits
bool I2CL_Write(alt_u32 clk_base, alt_u32 data_base, alt_8 DeviceAddr, alt_u16 ControlAddr, alt_u8 ControlData){
    bool bSuccess = TRUE;
    alt_u8 AddrHigh, AddrLow;
    
    AddrHigh = (ControlAddr >> 8) & 0xFF; 
    AddrLow  = ControlAddr & 0xFF; 
    
    // device id
    //DeviceAddr = HMB_E2_I2C_ID;

    i2c_start(clk_base, data_base);
    if (!i2c_write(clk_base, data_base, DeviceAddr)){  // send ID
        bSuccess = FALSE;
        I2C_DEBUG(("I2C HMB_E2 Fail: Address NACK!\n"));
    }
    if (bSuccess && !i2c_write(clk_base, data_base, AddrHigh)){ // send sub-address
        bSuccess = FALSE;
        I2C_DEBUG(("I2C HMB_E2 Fail: SubAddress-high NACK!\n"));
    }            
    if (bSuccess && !i2c_write(clk_base, data_base, AddrLow)){ // send sub-address
        bSuccess = FALSE;
        I2C_DEBUG(("I2C HMB_E2 Fail: SubAddress-low NACK!\n"));
    }            
    if (bSuccess && !i2c_write(clk_base, data_base, ControlData)){  
        bSuccess = FALSE;
        I2C_DEBUG(("I2C HMB_E2 Fail: write NACK!\n"));
    }
    i2c_stop(clk_base, data_base);
    
    usleep(7*1000); // delay to wait EE2 ready (at least 5 ms delay is required)
    
    return bSuccess;
    
}

bool I2CL_Read(alt_u32 clk_base, alt_u32 data_base, alt_8 DeviceAddr, alt_u16 ControlAddr, alt_u8 *pControlData){
    bool bSuccess = TRUE;
    alt_u8 AddrHigh, AddrLow;
    
    AddrHigh = (ControlAddr >> 8) & 0xFF; 
    AddrLow  = ControlAddr & 0xFF; 

    i2c_start(clk_base, data_base);
    if (!i2c_write(clk_base, data_base, DeviceAddr)){  // send ID
        bSuccess = FALSE;
        I2C_DEBUG(("I2C HMB_E2 Fail: Address NACK!\n"));
    }
    if (bSuccess && !i2c_write(clk_base, data_base, AddrHigh)){ // send sub-address
        bSuccess = FALSE;
        I2C_DEBUG(("I2C HMB_E2 Fail: SubAddress-high NACK!\n"));
    }            
    if (bSuccess && !i2c_write(clk_base, data_base, AddrLow)){ // send sub-address
        bSuccess = FALSE;
        I2C_DEBUG(("I2C HMB_E2 Fail: SubAddress-low NACK!\n"));
    }            
    i2c_start(clk_base, data_base);  // restart
    DeviceAddr |= 1; // Read
    if (bSuccess && !i2c_write(clk_base, data_base, DeviceAddr)){  // send id
        bSuccess = FALSE;
        I2C_DEBUG(("I2C HMB_E2 Fail: Address+1 NACK!\n"));
    }
    
    if (bSuccess){
        i2c_read(clk_base, data_base, pControlData, FALSE);  // read
    }        
    i2c_stop(clk_base, data_base);
    
    return bSuccess;    
}

// read from address 0
bool I2CL_MultipleRead(alt_u32 clk_base, alt_u32 data_base, alt_8 DeviceAddr, alt_u16 ControlAddr, alt_u8 szData[], alt_u16 len){
    int i;
    bool bSuccess = TRUE;
  // alt_u8 ControlAddr = 0;
    alt_u8 AddrHigh, AddrLow;
    
    AddrHigh = (ControlAddr >> 8) & 0xFF; 
    AddrLow  = ControlAddr & 0xFF;     
    
   
    // device id
    //DeviceAddr = HMB_E2_I2C_ID;

    i2c_start(clk_base, data_base);
    if (!i2c_write(clk_base, data_base, DeviceAddr)){  // send ID
        bSuccess = FALSE;
        I2C_DEBUG(("I2C HMB_E2 Fail: Address NACK!\n"));
    }
    if (bSuccess && !i2c_write(clk_base, data_base, AddrHigh)){ // send sub-address
        bSuccess = FALSE;
        I2C_DEBUG(("I2C HMB_E2 Fail: SubAddress-high NACK!\n"));
    }    
    if (bSuccess && !i2c_write(clk_base, data_base, AddrLow)){ // send sub-address
        bSuccess = FALSE;
        I2C_DEBUG(("I2C HMB_E2 Fail: SubAddress-low NACK!\n"));
    }    
    if (bSuccess)        
        i2c_start(clk_base, data_base);  // restart
    DeviceAddr |= 1; // Read
    if (bSuccess && !i2c_write(clk_base, data_base, DeviceAddr)){  // send id
        bSuccess = FALSE;
        I2C_DEBUG(("I2C HMB_E2 Fail: Address+1 NACK!\n"));
    }
    
    if (bSuccess){
        for(i=0;i<len && bSuccess;i++){
            i2c_read(clk_base, data_base, &szData[i], (i==(len-1))?FALSE:TRUE);  // read
        }            
    }        
    i2c_stop(clk_base, data_base);
    
    return bSuccess;    
    
} 



bool I2CL_MultipleWrite(alt_u32 clk_base, alt_u32 data_base, alt_8 DeviceAddr, alt_u16 ControlAddr, alt_u8 szData[], alt_u16 len){
    bool bSuccess = TRUE;
    alt_u8 AddrHigh, AddrLow;
    int i;
    
    AddrHigh = (ControlAddr >> 8) & 0xFF; 
    AddrLow  = ControlAddr & 0xFF; 
    

    i2c_start(clk_base, data_base);
    if (!i2c_write(clk_base, data_base, DeviceAddr)){  // send ID
        bSuccess = FALSE;
        I2C_DEBUG(("I2C HMB_E2 Fail: Address NACK!\n"));
    }
    if (bSuccess && !i2c_write(clk_base, data_base, AddrHigh)){ // send sub-address
        bSuccess = FALSE;
        I2C_DEBUG(("I2C HMB_E2 Fail: SubAddress-high NACK!\n"));
    }            
    if (bSuccess && !i2c_write(clk_base, data_base, AddrLow)){ // send sub-address
        bSuccess = FALSE;
        I2C_DEBUG(("I2C HMB_E2 Fail: SubAddress-low NACK!\n"));
    }            
    if (bSuccess){// && !i2c_write(clk_base, data_base, ControlData)){
        for(i=0;i<len && bSuccess;i++){
            bSuccess = i2c_write(clk_base, data_base, szData[i]);
        }  
        //bSuccess = FALSE;
        if (!bSuccess)
            I2C_DEBUG(("I2C HMB_E2 Fail: write NACK!\n"));
    }
    i2c_stop(clk_base, data_base);
    
    usleep(7*1000); // delay to wait EE2 ready (at least 5 ms delay is required)
    
    return bSuccess;
    
}



bool I2CL_Read16(alt_u32 clk_base, alt_u32 data_base, alt_8 DeviceAddr, alt_u16 ControlAddr, alt_u16 *pControlData){
    bool bSuccess;
    alt_u8 szData[2];
    bSuccess = I2CL_MultipleRead(clk_base, data_base, DeviceAddr, ControlAddr, szData, sizeof(szData));
    if (bSuccess){
        *pControlData = (szData[0] << 8 ) | szData[1];
    }
    return bSuccess;    
}

bool I2CL_Write16(alt_u32 clk_base, alt_u32 data_base, alt_8 DeviceAddr, alt_u16 ControlAddr, alt_u16 ControlData){
    bool bSuccess;
    alt_u8 szData[2];
    szData[0] = (ControlData >> 8) & 0xFF;
    szData[1] = ControlData & 0xFF;   
    bSuccess = I2CL_MultipleWrite(clk_base, data_base, DeviceAddr, ControlAddr, szData, sizeof(szData));
    return bSuccess;    
}

bool  OC_I2C_Read(alt_32 i2c_base,alt_u8 device_address,alt_u8 sub_address, alt_u8 *pData8, int nReadLength){

    //DWORD reg_data = 0x0;
    //BYTE data = 0x0;
    alt_32 Data32;
    int i;

   //set the tx reg audio chip dev address with write bit
   IOWR( i2c_base, 3,device_address);
   //set STA and WR bits(bit7 and bit4)
   IOWR( i2c_base, 4,0x90);
   //wait TIP bit go to 0 to end Tx
   if (!ACK_check( i2c_base)){
       printf("OC_I2C_Read error[2]\r\n");
       return FALSE;
   }
   IOWR(i2c_base, 3,sub_address);

   //set WR bits(bit4)
   IOWR( i2c_base, 4,0x10);
   //wait TIP bit go to 0 to end Tx
    if (!ACK_check( i2c_base)){
        printf("OC_I2C_Read error[5]\r\n");
        return FALSE;
    }
  // printf("\n read receive ACK-reg address! \n");

   //read
    //set the tx reg audio chip dev address with read bit 1
    IOWR(i2c_base, 3,device_address|0x01);

   //set STA and WR bits(bit7 and bit4)
    IOWR( i2c_base, 4,0x90);

   //wait TIP bit go to 0 to end Tx
    if (!ACK_check( i2c_base)){
        printf("OC_I2C_Read error[8]\r\n");
        return FALSE;
    }

    for(i=0;i<nReadLength;i++){
  // printf("\n read receive ACK-device address(read)! \n");
   //set the RD and ACK bit(bit5 and bit3)
    	IOWR( i2c_base, 4,((i+1) == nReadLength)?0x28:0x20);

        if (!ACK_judge_for_read( i2c_base)){
            printf("OC_I2C_Read error[10]\r\n");
            return FALSE;
        }
   // printf("\n read receive ACK-device address(read)! \n");

        Data32=IORD( i2c_base, 3);
        *(pData8+i) =  Data32 & 0xff;
    }

    IOWR( i2c_base, 4,0x40);

  // Sleep(10);
   //OS_msleep(1);
    usleep(SLEEP_TIME);
 //  printf(" Read  [%02X] = %02Xh\r\n", sub_address, data);


   return TRUE;

}

bool  Read32_Data(alt_32 base_address, alt_32 offset_address, alt_32 *pData32){
	bool bPass;
	*pData32 =IORD(base_address ,offset_address  );
	bPass=TRUE;
    return bPass;
}

bool ACK_judge_for_read(alt_32 i2c_base)
{
	bool bSuccess;
    alt_32 this_data;

   // OS_msleep( SLEEP_TIME );
     usleep(SLEEP_TIME);
    //   while(this_data & 0x02)
//   {
//      this_data =  Read32_Data ( hPCIe, i2c_base, 4);
//   }
    bSuccess =  Read32_Data ( i2c_base, 4, &this_data);
    if (bSuccess){
        if(this_data & 0x02)
            bSuccess = FALSE;
    }
   //wait the rx ACK signal 0-valid

    return bSuccess;

}

bool  ACK_single_check(alt_32 i2c_base)
{
	bool bSuccess=TRUE;
	alt_32 read_data;

    usleep(SLEEP_TIME);
    read_data =IORD(i2c_base, 4);
    if(read_data & 0x02)
            bSuccess = FALSE;
     if (bSuccess){
            usleep(SLEEP_TIME);
            read_data =IORD(i2c_base, 4);
                if(read_data & 0x80)
                    bSuccess = FALSE;
     }
    return bSuccess;
}


bool  ACK_check(alt_32 i2c_base){
    bool bSuccess = FALSE;
    int i=0;

    while(!bSuccess && i++ < 10){
        bSuccess = ACK_single_check(i2c_base);
    }

    return bSuccess;
}

