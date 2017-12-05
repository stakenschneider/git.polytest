#ifndef MY_UI_H_
#define MY_UI_H_

int my_ui( alt_video_display* display);

int AsVidCopyImageToBuffer( char* dest, char* src, 
                            int dest_width, 
                            int src_width, int src_height );
                            
//void DrawImage( alt_video_display* display,
 //                           struct gimp_image_struct* Terasic_image, int horiz, int vert);

void draw_array(unsigned int *buffer, int x_size, int y_size,
  alt_video_display* display, int x_offset, int y_offset,
  unsigned int transparent_color);

void InitBackGround(alt_video_display* display, void *temp_Buffer);

//void DrawTransparentImage( alt_video_display* display, struct gimp_image_struct* Terasic_image, int horiz, int vert,
  //    unsigned int transparent_color );

int DRAW_CopyRGBImageToBuffer( char* dest, char* src, 
                            int dest_width, 
                            int src_width, int src_height );
                            
void Calibrate(alt_video_display* display, void *temp_Buffer);
#define MAX_BUTTON_NUM 10                          
typedef struct {
    int horiz; 
    int vert;
    int Bar_Width;
    int origin_x;
    int origin_y;
} Bar;

typedef struct {
    int horiz; 
    int vert;
    int Ball_Width;
    int origin_x;
    int origin_y;
} Ball;

typedef struct {
  unsigned int loc_x;
  unsigned int loc_y;
  unsigned int size_x;
  unsigned int size_y;
  unsigned int corner_radius;
  char title[64];
  char* button_graphic_not_pressed;
  char* button_graphic_pressed;
  unsigned int is_selected;
  unsigned int pen_down_event_occured;
  unsigned int pen_up_event_occured;
} window_button;

typedef struct {
  window_button button[MAX_BUTTON_NUM];
  int num_buttons;
  int scroll_position;
} buttons_struct;


#define MAIN_MENU_NUM_COMMAND_BUTTONS 2

//Calibrate button
#define MAIN_MENU_CALIBRATE_BUTTON_INDEX 0
#define MAIN_MENU_CALIBRATE_BUTTON_TEXT  "Calibrate"
#define MAIN_MENU_CALIBRATE_BUTTON_X_START 600
#define MAIN_MENU_CALIBRATE_BUTTON_Y_START 300

//Hold/Release button
#define MAIN_MENU_HOLD_BUTTON_INDEX 1
#define MAIN_MENU_HOLD_BUTTON_TEXT  " Hold "
#define MAIN_MENU_HOLD_BUTTON_TEXT1  " Release "
#define MAIN_MENU_HOLD_BUTTON_X_START 600
#define MAIN_MENU_HOLD_BUTTON_Y_START 400

// Command button constants
#define COMMAND_BUTTON_CORNER_RADIUS            10
#define COMMAND_BUTTON_FONT                     tahomabold_20
#define COMMAND_BUTTON_SPACING                  10
#define COMMAND_BUTTON_HEIGHT                   50


#define COMMAND_BUTTON_COLOR_NOT_SELECTED       0x00C0C0C0
#define COMMAND_BUTTON_BORDER_COLOR_NOT_SELECTED 0x00000000
#define COMMAND_BUTTON_TEXT_COLOR_NOT_SELECTED  0x00000000

//#define COMMAND_BUTTON_COLOR_SELECTED           0x00000000
#define COMMAND_BUTTON_COLOR_SELECTED           0x0073c301
#define COMMAND_BUTTON_BORDER_COLOR_SELECTED    0x00C0C0C0
#define COMMAND_BUTTON_TEXT_COLOR_SELECTED      0x00C0C0C0

#define MENU_ITEM_CORNER_RADIUS                 10

#define BUTTON_NOT_SELECTED 0
#define BUTTON_PEN_UP 1
#define BUTTON_SELECTED 2


int get_X(int horiz, int origin_x);
int get_Y(int vert, int origin_y);

int AsBuildMainMenuCommandButtons( buttons_struct* command_buttons );
int AsPrintCommandButtons( alt_video_display* display, buttons_struct* command_buttons );
void AsPrintCommandButton( alt_video_display* display, window_button* button );

int AsUpdateCommandButtons( alt_video_display* display, 
                            int touchscreen_x,
                            int touchscreen_y,
                            int pen_is_down,
                            buttons_struct* command_buttons );

int AsUpdateCommandButton( alt_video_display* display,
                           int touchscreen_x,
                           int touchscreen_y,
                           int pen_is_down,
                           window_button* button );
                           
int AsCoordsAreOnButton( int touchscreen_x, int touchscreen_y, int pen_is_down, window_button* button );
int AsCoordsAreInArea( int coord_x, int coord_y, int coords_are_active,
                       int start_x, int start_y, int end_x, int end_y);

#endif /*MY_UI_H_*/
