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
static int bluetooth = 0;
static int bluetooth_status = 0;
static int dotIndex = 0;

static GColor backgroundColor;
static GColor textColor;

// bluetooth
static GBitmap *bt_bitmap;
static BitmapLayer *bt_layer;
static int BT_NEVER         = 0;
static int BT_ON_DISCONNECT = 1;
static int BT_ALWAYS        = 2;
// Vibe pattern: ON for 200ms, OFF for 100ms, ON for 400ms:
static const uint32_t const segments[] = { 500, 500, 500 };
VibePattern bt_vibe = {
  .durations = segments,
  .num_segments = ARRAY_LENGTH(segments),
};

enum appParameters {
  SHAPE_KEY      = 0x0,
  COLOR_KEY      = 0x1,
  NUMBER_KEY     = 0x2,
  BLUETOOTH_KEY  = 0X3,
/*  BATTERY_KEY    = 0x2,
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
static int BLUETOOTH_OPTIONS = 3;