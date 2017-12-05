/*****************************************************************************
 *  File: skin1.h
 * 
 *  This file contains the constants which define what the "skin" of the GUI
 *  looks like.  Things such as locations, colors and sizes of titles, buttons,
 *  etc.. are represented here.
 *
 ****************************************************************************/

#ifndef SKIN1_H_
#define SKIN1_H_ 

#include "app_selector.h"

//////////////////////////////////////////////////////////////////////////////
//                  General constants for all screens                       //
//////////////////////////////////////////////////////////////////////////////
#define MAX_MENU_TITLE_LINES                    3
#define MENU_TITLE_FONT                         tahomabold_32
#define MAX_STRING_LENGTH                       64

#define COLOR_MENU_BACKGROUND                   0x00000000

#define COLOR_MENU_TITLE_BAR_32                 0x004A64AC
#define COLOR_MENU_CORNER_32                    0x00061AAE

#define TOP_BORDER_X_START                      50
#define TOP_BORDER_Y_START                      0
#define TOP_BORDER_WIDTH                        (ALT_VIDEO_DISPLAY_COLS - TOP_BORDER_X_START)
#define TOP_BORDER_HEIGHT                       110

#define TL_CORNER_X_START                       0
#define TL_CORNER_Y_START                       0
#define TL_CORNER_WIDTH                         50
#define TL_CORNER_HEIGHT                        110

#define LEFT_BORDER_X_START                     0
#define LEFT_BORDER_Y_START                     110
#define LEFT_BORDER_WIDTH                       50
#define LEFT_BORDER_HEIGHT                      290

#define BL_CORNER_X_START                       0
#define BL_CORNER_Y_START                       400
#define BL_CORNER_WIDTH                         50
#define BL_CORNER_HEIGHT                        (ALT_VIDEO_DISPLAY_ROWS - BL_CORNER_Y_START)

#define BOTTOM_BORDER_X_START                   50
#define BOTTOM_BORDER_Y_START                   400
#define BOTTOM_BORDER_WIDTH                     (ALT_VIDEO_DISPLAY_COLS - BOTTOM_BORDER_X_START)
#define BOTTOM_BORDER_HEIGHT                    (ALT_VIDEO_DISPLAY_ROWS - BOTTOM_BORDER_Y_START)

// Command button constants
#define COMMAND_BUTTON_CORNER_RADIUS            10
#define COMMAND_BUTTON_FONT                     tahomabold_20
#define COMMAND_BUTTON_SPACING                  10
#define COMMAND_BUTTON_HEIGHT                   50

#define COMMAND_BUTTON_COLOR_NOT_SELECTED       0x00C0C0C0
#define COMMAND_BUTTON_BORDER_COLOR_NOT_SELECTED 0x00000000
#define COMMAND_BUTTON_TEXT_COLOR_NOT_SELECTED  0x00000000

#define COMMAND_BUTTON_COLOR_SELECTED           0x00000000
#define COMMAND_BUTTON_BORDER_COLOR_SELECTED    0x00C0C0C0
#define COMMAND_BUTTON_TEXT_COLOR_SELECTED      0x00C0C0C0


//////////////////////////////////////////////////////////////////////////////
//                       Constants for Main Menu                            //
//////////////////////////////////////////////////////////////////////////////
#define MENU_ITEMS_ON_SCREEN_AT_ONCE            5

#define COLOR_MAIN_MENU_TITLE_TEXT_32           0x00FFFFFF

#define COLOR_MENU_ITEM_NOT_SELECTED_ODD_32     0x007B9EED
#define COLOR_MENU_ITEM_NOT_SELECTED_EVEN_32    0x005C76B1
#define COLOR_MENU_ITEM_TEXT_NOT_SELECTED_32    0x00FFFFFF
#define COLOR_MENU_ITEM_BULLET_NOT_SELECTED_32  0x00FFFFFF

#define COLOR_MENU_ITEM_SELECTED_32             0x00061AAE
#define COLOR_MENU_ITEM_TEXT_SELECTED_32        0x00FFFFFF
#define COLOR_MENU_ITEM_BULLET_SELECTED_32      0x00FFFFFF
#define COLOR_MENU_ITEM_BORDER_SELECTED_32      0x00F0F000

#define MENU_ITEM_X_START                       60  
#define MENU_ITEM_X_WIDTH                       660  
#define MENU_ITEM_X_END                         ( MENU_ITEM_X_START + MENU_ITEM_X_WIDTH - 1 )

#define MENU_ITEM_Y_START                       117
#define MENU_ITEM_Y_HEIGHT                      55

#define MENU_ITEM_CORNER_RADIUS                 10
#define MENU_ITEM_BORDER_WIDTH                  5
#define MENU_ITEM_FONT                          tahomabold_32

#define MENU_AREA_X_START                       MENU_ITEM_X_START
#define MENU_AREA_Y_START                       MENU_ITEM_Y_START
#define MENU_AREA_X_END                         ( MENU_ITEM_X_START + MENU_ITEM_X_WIDTH )
#define MENU_AREA_Y_END                         ( MENU_ITEM_Y_START + ( MENU_ITEM_Y_HEIGHT * MENU_ITEMS_ON_SCREEN_AT_ONCE ))

#define MAIN_MENU_TITLE_Y_START_1               15
#define MAIN_MENU_TITLE_TEXT1                   "Cyclone III Embedded Evaluation Kit"
#define MAIN_MENU_TITLE_Y_START_2               55
#define MAIN_MENU_TITLE_TEXT2                   "Application Selector"

// Main menu command button constants
#define MAIN_MENU_NUM_COMMAND_BUTTONS           2

#define MAIN_MENU_LOAD_BUTTON_INDEX             0
#define MAIN_MENU_LOAD_BUTTON_X_START           80
#define MAIN_MENU_LOAD_BUTTON_Y_START           410
#define MAIN_MENU_LOAD_BUTTON_TEXT              "Load"

#define MAIN_MENU_INFO_BUTTON_INDEX             1
#define MAIN_MENU_INFO_BUTTON_X_START           500
#define MAIN_MENU_INFO_BUTTON_Y_START           410
#define MAIN_MENU_INFO_BUTTON_TEXT               "Show Info"


//////////////////////////////////////////////////////////////////////////////
//                Constants for Application Info Screen                     //
//////////////////////////////////////////////////////////////////////////////
#define INFO_SCREEN_TITLE_Y_START_1             15
#define INFO_SCREEN_TITLE_Y_START_2             55
#define INFO_SCREEN_TITLE_TEXT2                 "Application Info"

#define INFO_SCREEN_NUM_COMMAND_BUTTONS         1

#define INFO_SCREEN_OK_BUTTON_INDEX             0
#define INFO_SCREEN_OK_BUTTON_X_START           80
#define INFO_SCREEN_OK_BUTTON_Y_START           410
#define INFO_SCREEN_OK_BUTTON_TEXT              "OK"


//////////////////////////////////////////////////////////////////////////////
//                      Constants for all pop-ups                           //
//////////////////////////////////////////////////////////////////////////////
#define WINDOW_CORNER_RADIUS                    20
#define WINDOW_BORDER_WIDTH                     5


//////////////////////////////////////////////////////////////////////////////
//                    Constants for Question pop-ups                        //
//////////////////////////////////////////////////////////////////////////////
#define QUESTION_WINDOW_TEXT_FONT               tahomabold_20

#define QUESTION_WINDOW_Y_HEIGHT                180
#define QUESTION_WINDOW_OK_BUTTON_INDEX         0
#define QUESTION_WINDOW_CANCEL_BUTTON_INDEX     1
#define QUESTION_WINDOW_BORDER_WIDTH            WINDOW_BORDER_WIDTH

#define QUESTION_WINDOW_BODY_COLOR              0x00FFFFFF
#define QUESTION_WINDOW_BORDER_COLOR            0x00000080
#define QUESTION_WINDOW_TEXT_COLOR              0x00000000


//////////////////////////////////////////////////////////////////////////////
//                     Constants for progress bars                          //
//////////////////////////////////////////////////////////////////////////////
#define PROGRESS_WINDOW_Y_HEIGHT                180
#define PROGRESS_WINDOW_BORDER_WIDTH            WINDOW_BORDER_WIDTH

#define PROGRESS_WINDOW_BODY_COLOR              0x00FFFFFF
#define PROGRESS_WINDOW_BORDER_COLOR            0x00000080
#define PROGRESS_WINDOW_TEXT_COLOR              0x00000000
#define PROGRESS_WINDOW_PROG_BAR_FRAME_COLOR    0x00000080
#define PROGRESS_WINDOW_PROG_BAR_COLOR          0x007B9EED
#define PROGRESS_WINDOW_PROG_BAR_HEIGHT         30
#define PROGRESS_WINDOW_TEXT_FONT               tahomabold_20


//////////////////////////////////////////////////////////////////////////////
//                     Constants for scroll buttons                         //
//////////////////////////////////////////////////////////////////////////////

#define SCROLL_BUTTON_CORNER_RADIUS             10
#define SCROLL_BUTTON_BORDER_WIDTH              3

#define SCROLL_BUTTON_COLOR_NOT_SELECTED        0x00C0C0C0
#define SCROLL_BUTTON_BORDER_COLOR_NOT_SELECTED 0x00000000
#define SCROLL_BUTTON_ARROW_COLOR_NOT_SELECTED  0x00000000

#define SCROLL_BUTTON_COLOR_SELECTED            0x00000000
#define SCROLL_BUTTON_BORDER_COLOR_SELECTED     0x00C0C0C0
#define SCROLL_BUTTON_ARROW_COLOR_SELECTED      0x00C0C0C0
#define SCROLL_BUTTON_X_START                   (MENU_ITEM_X_START + MENU_ITEM_X_WIDTH + 10 + MENU_ITEM_BORDER_WIDTH + SCROLL_BUTTON_BORDER_WIDTH)
#define SCROLL_BUTTON_WIDTH                     ((ALT_VIDEO_DISPLAY_COLS - SCROLL_UP_BUTTON_X_START) - SCROLL_BUTTON_BORDER_WIDTH)
#define SCROLL_BUTTON_HEIGHT                    SCROLL_BUTTON_WIDTH

#define SCROLL_DOWN_INDEX 0
#define SCROLL_DOWN_BUTTON_X_START              SCROLL_BUTTON_X_START
#define SCROLL_DOWN_BUTTON_Y_START              (((MENU_ITEM_Y_START + (MENU_ITEM_Y_HEIGHT * (MENU_ITEMS_ON_SCREEN_AT_ONCE))) - SCROLL_BUTTON_HEIGHT) - 10)
#define SCROLL_DOWN_BUTTON_WIDTH                SCROLL_BUTTON_WIDTH
#define SCROLL_DOWN_HEIGHT                      SCROLL_BUTTON_HEIGHT
    
#define SCROLL_UP_INDEX 1
#define SCROLL_UP_BUTTON_X_START                SCROLL_BUTTON_X_START
#define SCROLL_UP_BUTTON_Y_START                MENU_ITEM_Y_START
#define SCROLL_UP_BUTTON_WIDTH                  SCROLL_BUTTON_WIDTH
#define SCROLL_UP_BUTTON_HEIGHT                 SCROLL_BUTTON_HEIGHT

#define SCROLL_INDICATOR_COLOR                  0x00C0C0C0
#define SCROLL_INDICATOR_X_START                SCROLL_BUTTON_X_START
#define SCROLL_INDICATOR_WIDTH                  SCROLL_BUTTON_WIDTH
#define SCROLL_INDICATOR_HEIGHT                 20
#define SCROLL_INDICATOR_AREA_X_START           SCROLL_BUTTON_X_START
#define SCROLL_INDICATOR_AREA_X_END             (SCROLL_BUTTON_X_START + SCROLL_BUTTON_WIDTH)
#define SCROLL_INDICATOR_AREA_Y_START           (SCROLL_UP_BUTTON_Y_START + SCROLL_BUTTON_HEIGHT)
#define SCROLL_INDICATOR_AREA_Y_END             SCROLL_DOWN_BUTTON_Y_START

#define SCROLL_UP                               0
#define SCROLL_DOWN                             1

#define COLOR_R 0xFF0000
#define COLOR_G 0x00FF00
#define COLOR_B 0x0000FF
#define COLOR_WHITE 0xFFFFFF
#define COLOR_BLACK 0x000000
#define COLOR_GRAY  0x7F7F7F
#endif /*SKIN1_H_*/
