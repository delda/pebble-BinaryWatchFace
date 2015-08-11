#include <pebble.h>

static Window *s_window;             // main window view
static Layer *s_mainLayer;           // bullets for hours
static char *s_textBase[]={"1","2","4","8","16","32"};
static int s_bufferTime[2][6];
static TextLayer *text[2][6];
static char bufferTime[20];
static int s_bulletsNumber[2];
static TextLayer *s_backgrounLayer;
static GRect s_layerRect[2];
unsigned int hour, minute;

static AppSync sync;
static uint8_t sync_buffer[32];

static int shape = 0;
static int color = 0;
static int number = 0;
static int dotIndex = 0;

static GColor backgroundColor;
static GColor textColor;

enum appParameters {
  SHAPE_KEY      = 0x0,
  COLOR_KEY      = 0x1,
  NUMBER_KEY     = 0x2,
/*  BATTERY_KEY    = 0x2,
  BLUETOOTH_KEY  = 0x3,
  FLIP_PHONE_KEY = 0x4,*/
};
  
typedef struct{
  GColor background;
  GColor text;
  GColor strokeDot;
  GColor fillDot;
  GColor time;
} Color;

#ifdef PBL_COLOR
  static int SHAPE_NUM = 2;
  static int COLOR_NUM = 8;
  static Color palette[8];
#else
  static int SHAPE_NUM = 2;
  static int COLOR_NUM = 2;
  static Color palette[2];
#endif

/*
Themes:
01 LITE    white
02 DARK    dark
03         celeste
04         celeste inverted
05 PASSION red
06 LOVE    red inverted
07         green
08         green inverted
09 ALL     all the themes, one at minute
10 RANDOM  random colors
*/