// --------------------------------------------------------------------
// Copyright (c) 2014 by Terasic Technologies Inc.
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

#include <stdio.h>
#include <stdlib.h> 
#include <sys/alt_alarm.h>
#include "app_selector.h"
#include "alt_video_display.h"
#include "my_ui.h"
#include "system.h"
#include "io.h"

#include "terasic_includes.h"
#include "I2C.h"
#include "Temp_RH.h"
#include "math.h"


int main()
{

  volatile int ret_code;
  
  // This is our LCD display
  alt_video_display* display;

  // Initialize and start the LCD display
  display = alt_video_display_init( ALT_VIDEO_DISPLAY_SGDMA_NAME,         // char* sgdma_name
                                    ALT_VIDEO_DISPLAY_COLS,               // int width
                                    ALT_VIDEO_DISPLAY_ROWS,               // int height
                                    ALT_VIDEO_DISPLAY_COLOR_DEPTH,        // int color_depth
                                    ALT_VIDEO_DISPLAY_USE_HEAP,           // int buffer_location
                                    ALT_VIDEO_DISPLAY_USE_HEAP,           // int descriptor_location
                                    2 );                                  // int num_buffers                                  // int num_buffers                                   
  // Set the Gamma Curve and Resolution of the LCD
  
  RH_Temp_Sensor_init();
  MainMenu(display);  

  return ( ret_code );
}

