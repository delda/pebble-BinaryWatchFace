#include <pebble.h>

static Window *s_window;             // main window view
static Layer *s_mainLayer;           // bullets for hours
static char *s_textBase[]={"1","2","4","8","16","32"};
static int s_bufferTime[2][6];
static TextLayer *text[2][6];
static char bufferTime[20];
static int s_bulletsNumber[2];
static GRect s_layerRect[2];

static AppSync sync;
static uint8_t sync_buffer[32];

static int shape = 0;
static int color = 0;

static GColor backgroundColor;
static GColor textColor;

enum appParameters {
  SHAPE_KEY      = 0x0,
  COLOR_KEY      = 0x1,
  BATTERY_KEY    = 0x2,
  BLUETOOTH_KEY  = 0x3,
  FLIP_PHONE_KEY = 0x4,
};