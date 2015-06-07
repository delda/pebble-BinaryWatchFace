#include <pebble.h>

static Window *s_window;             // main window view
static Layer *s_mainLayer;           // bullets for hours
static char *s_textBase[]={"1","2","4","8","16","32"};
static int s_bufferTime[2][6];
static TextLayer *text[2][6];
static char bufferTime[20];
static int s_bulletsNumber[2];
static GRect s_layerRect[2];