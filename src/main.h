
#include <pebble.h>

static Window *s_window;             // main window view
static Layer *s_mainLayer;           // bullets for hours
static char *s_textBase[]={"1","2","4","8","16","32"};
static int s_bufferTime[2][6];
static TextLayer *text[2][6];
static char bufferTime[20];
static int s_counter;
static int s_bulletsNumber[2];
static GColor s_backgroundColor;
static GColor s_textColor;
static GRect s_layerRect[2];
static TimeUnits s_updateTime;
static int s_isSettingModality;
static int s_shapeType;

char *logTime();
void long_click_up_handler(ClickRecognizerRef recognizer, void *context);
static void update_time();
static void tick_handler(struct tm *tick_time, TimeUnits units_changed);
void select_long_click_handler(ClickRecognizerRef recognizer, void *context);
void click_config_provider(void *context);
void dec2bin(int number, int hORm);
static void fill_row(Layer *layer, GContext *gContext);
static void update_time_view(Layer *layer, GContext *gContext);
static void window_load(Window *window);
static void window_unload();
static void init();
static void deinit();
int main(void);