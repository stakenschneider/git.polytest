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
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include "alt_video_display.h"
#include "app_selector.h"
#include "my_ui.h"
#include "simple_graphics.h"
#include "fonts.h"
#include "sys/alt_alarm.h"
#include "sys/alt_irq.h"  // interrupt
#include <unistd.h>
#include "gimp_bmp.h"
#include "system.h"

#include "terasic_includes.h"
#include "I2C.h"
#include "multi_touch2.h"
//#include "ADXL345.h"
#include "math.h"
//#include "light_sensor.h"

#define I2C_OPENCORES_0_BASE 0x10000000
#define LCD_TOUCH_INT_BASE 0x10000040
#define LCD_TOUCH_INT_IRQ 4

#define COLOR_R           0xFF0000
#define COLOR_G           0x00FF00
#define COLOR_B           0x0000FF
#define COLOR_WHITE       0xFFFFFF
#define COLOR_BLACK       0x000000
#define COLOR_GRAY        0x7F7F7F
#define COLOR_Temp_Up     0x000000
#define COLOR_Temp_Down   0xFFFFFF
#define COLOR_Temp_Block  0XFF0000
#define COLOR_Rh_Up       0x000000
#define COLOR_Rh_Down     0xFFFFFF
#define COLOR_Rh_Line     0XEE7600

#define RhLinex_zero      563
#define RhLiney_zero      278
#define Rh_x              535
#define Rh_y              356
#define Temp_x            48
#define Temp_y            170
#define Temp_Blockx_star  222
#define Temp_Blockx_end   235
#define Temp_Blockx_zero  280
#define Temp_Blocky_end   415

#define Line_R 60

#define ENABLE_BUTTON 0

buttons_struct command_buttons;

int isCelsius, isFahrenheit, isKelvin;
RECT celsiusButton, fahrenheitButton, kelvinButton;

int MainMenu( alt_video_display* display){
    char   szText[128];
    int    RhLinex_end,RhLiney_end;
    float  angle;

    //temp display
    void *temp_Buffer;
    temp_Buffer =(void*)malloc(display->bytes_per_frame);
    printf("===== welcome to Temp_RH demo program =====\n");
    usleep(200*1000);
    InitBackGround(display, temp_Buffer);
    extern struct gimp_image_struct dot;

    const bool bVPG = ((IORD(KEY_BASE, 0x00) & 0x01) == 0x00)?TRUE:FALSE;
    //IOWR(LED_BASE, 0x00, 0xFF);

    oc_i2c_init(I2C_OPENCORES_0_BASE);

    usleep(1000);

	// Инициализация касаний
    MTC2_INFO *pTouch = MTC2_Init(I2C_OPENCORES_0_BASE,LCD_TOUCH_INT_BASE,LCD_TOUCH_INT_IRQ);
	if (!pTouch){
		printf("Failed to init multi-touch\r\n");
	}else{
		printf("Init touch successfully\r\n");
	}

	// Переменные для определения единицы измерения
	isCelsius = TRUE;
	isFahrenheit = FALSE;
	isKelvin = FALSE;

	// Прямоугольники, для определения попадания
	RectSet(&celsiusButton, 400, 700, 100, 160);
	RectSet(&fahrenheitButton, 400, 700, 200, 260);
	RectSet(&kelvinButton, 400, 700, 300, 360);

	alt_u8 Event, TouchNum;
	int X1, Y1, X2, Y2, X3, Y3, X4, Y4, X5, Y5;
	POINT Pt1;

	double resultTemperature;
	char postfix[3];

	// Количество итераций
	int ind = 0;
	while(1)
	{     
		// Получение информации с сенсора
		if(!sensor_getdata()){
			printf("sensor_getdata failed!\n");
			return 0;
		}
		
		// Корректировка результата
		if(rh==0){
				RhLinex_end=RhLinex_zero-Line_R;
				RhLiney_end=RhLiney_zero;
			}
		else if((rh>0)&&(rh<50)){
				angle=rh/100.0*M_PI;
				RhLinex_end=RhLinex_zero-Line_R*cos(angle);
				RhLiney_end=RhLiney_zero-Line_R*sin(angle);
			}
		else if(rh==50){
				RhLinex_end=RhLinex_zero;
				RhLiney_end=RhLiney_zero-Line_R;
			}
		else if((rh>50)&&(rh<100)){
				angle=(1-rh/100.0)*M_PI;
				RhLinex_end=RhLinex_zero+Line_R*cos(angle);
				RhLiney_end=RhLiney_zero-Line_R*sin(angle);
			}
		else if(rh==100){
				RhLinex_end=RhLinex_zero+Line_R;
				RhLiney_end=RhLiney_zero;
			}

		// Получаем информацию о количестве касаний
		if(MTC2_GetStatus(pTouch, &Event, &TouchNum, &X1, &Y1, &X2, &Y2, &X3, &Y3, &X4, &Y4, &X5, &Y5)) {
			PtSet(&Pt1, X1, Y1);

			printf("TOUCH\n");

			if(IsPtInRect(&Pt1, &celsiusButton)) {
				// Если было касание первой кнопки, выводим в цельсиях
				isCelsius = TRUE;
				isFahrenheit = FALSE;
				isKelvin = FALSE;
			}
			else if(IsPtInRect(&Pt1, &fahrenheitButton)) {
				// Если было касание второй кнопки, выводим в фаренгейтах
				isCelsius = FALSE;
				isFahrenheit = TRUE;
				isKelvin = FALSE;
			}
			else if(IsPtInRect(&Pt1, &kelvinButton)) {
				// Если было касание третей кнопки, выводим в кельвинах
				isCelsius = FALSE;
				isFahrenheit = FALSE;
				isKelvin = TRUE;
			}
		}

		++ind;

		// Определение единицы измерения
		isCelsius = !(ind % 3);
		isFahrenheit = !((ind + 1) % 3);
		isKelvin = !((ind + 2) % 3);

		memcpy((void*)(display->buffer_ptrs[display->buffer_being_written]->buffer),temp_Buffer ,display->bytes_per_frame);

		// Рисование контуров кнопок
		vid_draw_box(400, 100, 700, 160, BLACK_24, DO_NOT_FILL, display);
		vid_draw_box(400, 200, 700, 260, BLACK_24, DO_NOT_FILL, display);
		vid_draw_box(400, 300, 700, 360, BLACK_24, DO_NOT_FILL, display);

		// Рисование надписей на кнопках
		strcpy(szText, "Celsius");
		vid_print_string_alpha(410, 110, BLACK_24, WHITE_24, tahomabold_32, display, szText);
		strcpy(szText, "Fahrenheit");
		vid_print_string_alpha(410, 210, BLACK_24, WHITE_24, tahomabold_32, display, szText);
		strcpy(szText, "Kelvin");
		vid_print_string_alpha(410, 310, BLACK_24, WHITE_24, tahomabold_32, display, szText);

		// В зависимости отединицы измерения, определяем выводимую температуру
		if(isCelsius) {
			resultTemperature = temp;
			strcpy(postfix, " C");
		}
		else if(isFahrenheit) {
			resultTemperature = temp * 1.8 + 32;
			strcpy(postfix, " F");
		}
		else if(isKelvin) {
			resultTemperature = temp + 273.15;
			strcpy(postfix, " K");
		}

		// Выводим температуру
		snprintf(szText, 128, "%4.3f", resultTemperature);
		vid_print_string_alpha(30, 30, COLOR_Temp_Up, COLOR_Temp_Down, tahomabold_32, display, szText);
		int length = vid_string_pixel_length_alpha(tahomabold_32, szText);
		vid_print_string_alpha(30 + length, 30, COLOR_Temp_Up,COLOR_Temp_Down, tahomabold_32, display, postfix);

		// Определение заполненности шкалы температуры
		int coordinate;
		if(temp < -30)
			coordinate = 32;
		else if(temp > 50)
			coordinate = 152;
		else
			coordinate = (int) ((temp + 30.) / 80. * 120. + 32.);

		// Определение цвета шкалы температуры
		int color;
		if(temp < -10)
			color = BLUE_24;
		else if (temp < 10)
			color = AQUA_24;
		else if (temp < 27)
			color = YELLOW_24;
		else if (temp < 37)
			color = ORANGE_24;
		else if (temp >= 37)
			color = RED_24;

		// Вывод шкалы температуры
		vid_draw_box(30, 89, 153, 122, BLACK_24, DO_NOT_FILL, display);
		vid_draw_box(31, 90, coordinate, 121, color, DO_FILL, display);

		// Вывод значения влажности воздуха
		snprintf(szText, 128, "%4.3f", rh);
		vid_print_string_alpha(30, 130, COLOR_Rh_Up, COLOR_Rh_Down, tahomabold_32, display, szText);
		length = vid_string_pixel_length_alpha(tahomabold_32, szText);
		vid_print_string_alpha(30 + length, 130, COLOR_Rh_Up, COLOR_Rh_Down, tahomabold_32, display, " %RH");

		// Определение заполненности шкалы влажности воздуха
		if(rh < 0)
			coordinate = 32;
		else if(rh > 100)
			coordinate = 152;
		else
			coordinate = (int) (rh / 100. * 120. + 32.);

		// Определение цвета шкалы влажности воздуха
		if(rh < 30)
			color = YELLOW_24;
		else if (rh < 70)
			color = AQUA_24;
		else if (rh >= 70)
			color = BLUE_24;

		// Вывод шкалы влажности воздуха
		vid_draw_box(30, 189, 153, 222, BLACK_24, DO_NOT_FILL, display);
		vid_draw_box(31, 190, coordinate, 221, color, DO_FILL, display);

		alt_video_display_register_written_buffer( display );

		// Задержка для плавного перехода
		usleep(1000000);
     }
}

void draw_border(int color, alt_video_display* display){
    int x,y;
    alt_u8 *pDes = display->buffer_ptrs[display->buffer_being_written]->buffer;
    for(y=0;y<display->height;y++){
        for(x=0;x<display->width;x++){
            if (x<=1 || x >= display->width-2 || y <= 1 || y >= display->height-2){
                *pDes++ = color & 0xFF; // blue
                *pDes++ = (color >> 8) & 0xFF; // green
                *pDes++ = (color >> 16) & 0xFF; // red
                pDes++;                 // reserved
            }else{
                pDes += 4;
            }
        }
    }

}

void DrawImage( alt_video_display* display, struct gimp_image_struct* Terasic_image, int horiz, int vert )
{

  bitmap_struct* image;
  char* image_dest;

  image = malloc(sizeof(bitmap_struct));
  
  if( image != NULL )
  {
    // Load the CIII logo
    load_gimp_bmp( Terasic_image, image, 32);

      image_dest = (char*)(( display->buffer_ptrs[display->buffer_being_written]->buffer ) + (vert * (display->width * 4)) + horiz * 4); 
                              
      // Copy the image to the current display buffer
      AsVidCopyImageToBuffer( image_dest,
                              image->data, 
                              display->width, 
                              image->biWidth, 
                              image->biHeight );     

    free_gimp_bmp_data( image );
    free( image );
  }
}

int AsVidCopyImageToBuffer( char* dest, char* src, 
                            int dest_width, 
                            int src_width, int src_height )
{
  int y;
  
  //Copy one line at a time from top to bottom
  for ( y = 0; y < src_height; y++ )
  {
    memcpy( dest, src, ( src_width * 4 ));
    src += ( src_width * 4 );
    dest += ( dest_width * 4 );
  }
  
  return( 0 );
} 

////draw array 
void draw_array(unsigned int *buffer, int x_size, int y_size,
  alt_video_display* display, int x_offset, int y_offset,
  unsigned int transparent_color)
{
  unsigned int *current_buffer;
  int addr = ( (int)(display->buffer_ptrs[display->buffer_being_written]->buffer) );
  current_buffer = (unsigned int *)addr;
  int x_b,y_b;
  unsigned int color;

  transparent_color = 0xFF000000 | transparent_color;

  for (x_b = 0; x_b<x_size; x_b++) {
    for (y_b = 0; y_b<y_size; y_b++) {
      color = buffer[y_b*x_size+x_b];
      if (color != transparent_color) {
        current_buffer[(y_b+y_offset)*display->width+(x_b+x_offset)] = color;
      }
    }
  }
}

void InitBackGround(alt_video_display* display, void *temp_Buffer)
{
    extern struct gimp_image_struct background;
    
    alt_video_display_clear_screen( display, COLOR_WHITE );
        
    //draw background
    //DrawImage(display, &background, 0,0);
              
    memcpy(temp_Buffer, (void*)(display->buffer_ptrs[display->buffer_being_written]->buffer),display->bytes_per_frame);
}

void DrawTransparentImage( alt_video_display* display, struct gimp_image_struct* Terasic_image, int horiz, int vert,unsigned int transparent_color )
{

  bitmap_struct* image;
  char* image_dest;

  image = malloc(sizeof(bitmap_struct));
  
  if( image != NULL )
  {
    // Load the CIII logo
    load_gimp_bmp( Terasic_image, image, 32);

      image_dest = (char*)(( display->buffer_ptrs[display->buffer_being_written]->buffer ) + (vert * (display->width * 4)) + horiz * 4); 
                              
      // Copy the image to the current display buffer
//      AsVidCopyImageToBuffer( image_dest,
//                              image->data, 
//                              display->width, 
//                              image->biWidth, 
//                              image->biHeight );   
  int y;
  //Copy one line at a time from top to bottom
//  for ( y = 0; y < image->biHeight; y++ )
//  {
//    memcpy( dest, src, ( image->biWidth * 4 ));
//    src += ( image->biWidth * 4 );
//    dest += ( display->width * 4 );
//  }  

    draw_array((unsigned int*)image->data,image->biWidth,image->biHeight,display,horiz,vert,transparent_color);

    free_gimp_bmp_data( image );
    free( image );
  }
}

int DRAW_CopyRGBImageToBuffer( char* dest, char* src, 
                            int dest_width, 
                            int src_width, int src_height )
{
  int x, y;
  
  
  //Copy one line at a time from top to bottom
  char *pSrcBase, *pDesBase;
  pSrcBase = src;
  pDesBase = dest;
  for ( y = 0; y < src_height; y++ )
  {
        char *pDes = pDesBase;
        char *pSrc = pSrcBase;
        for(x = 0; x< src_width; x++){
            *pDes++ = *pSrc++;
            *pDes++ = *pSrc++;
            *pDes++ = *pSrc++;
            *pDes++ = 0;  // alpha
        }
        
        pDesBase += dest_width * 4;
        pSrcBase += src_width * 3;
  }
  
  return( 0 );
}

