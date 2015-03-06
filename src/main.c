
#include "main.h"

static Window *s_window;             // main window view
static TextLayer *s_layerHours;      // text layer for hours
static TextLayer *s_layerMinutes;    // test layer for minutes
static Layer *s_dotsHours;           // bullets for hours
static Layer *s_dotsMinutes;         // bullets for minutes
static char *s_textBase[]={"1","2","4","8","16","32"};
static TextLayer *temp[6];
static int s_bufferTime[2][6];

void dec2bin(int number, int hORm){
  int bufferSize = 6;
  int d;

  for(int i=bufferSize; i>=0; i--){
    d = number >> i;
    if(d & 1){
      s_bufferTime[hORm][i] = 1;
    }else{
      s_bufferTime[hORm][i] = 0;
    }
  }
}

static void update_time(){
  time_t currentTime = time(NULL);
  struct tm* cTime = localtime(&currentTime);
  unsigned int hour = cTime->tm_hour % 12;
  unsigned int minute = cTime->tm_min;
  
  printf("%d:%d", hour, minute);
  
  dec2bin(hour, 0);
  printf("%d %d %d %d %d %d", s_bufferTime[0][5], s_bufferTime[0][4], s_bufferTime[0][3], s_bufferTime[0][2], s_bufferTime[0][1], s_bufferTime[0][0]);
  
  dec2bin(minute, 1);
  printf("%d %d %d %d %d %d", s_bufferTime[1][5], s_bufferTime[1][4], s_bufferTime[1][3], s_bufferTime[1][2], s_bufferTime[1][1], s_bufferTime[1][0]);

}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed){
  layer_mark_dirty(s_dotsHours);
  layer_mark_dirty(s_dotsMinutes);
}

static void fillRow(Layer *layer, GContext *gContext, GRect layerDim, int bulletsNumber, int *s_bufferTime){
  int currentWidth;
  int widthSingleLayer;
  
  printf("fillRow: %d %d %d %d %d %d", s_bufferTime[5], s_bufferTime[4], s_bufferTime[3], s_bufferTime[2], s_bufferTime[1], s_bufferTime[0]);
  
  // trick to simulate the round function
  widthSingleLayer = (int)(layerDim.size.w/(bulletsNumber-1));
  int wRest = layerDim.size.w%(bulletsNumber-1);
  if(wRest > (bulletsNumber-1)/2){
    widthSingleLayer++;
  }

  for(int i=0; i<bulletsNumber; i++){
    if(i==0){
      currentWidth = layerDim.origin.x;
    }else if(i==(bulletsNumber-1)){
      currentWidth = layerDim.origin.x + layerDim.size.w;
    }else{
      currentWidth = layerDim.origin.x + widthSingleLayer * i;
    }
    if(s_bufferTime[i] == 1){
      graphics_fill_circle(gContext, GPoint(currentWidth,24), 8);
    }else{
      graphics_draw_circle(gContext, GPoint(currentWidth,24), 8);
    }
    graphics_context_set_fill_color(gContext,GColorBlack);
    temp[i] = text_layer_create(GRect((currentWidth-8), 0, 16, 18));
    text_layer_set_background_color(temp[i], GColorClear);
    text_layer_set_font(temp[i], fonts_get_system_font(FONT_KEY_GOTHIC_14));
    text_layer_set_text_color(temp[i], GColorBlack);
    text_layer_set_text_alignment(temp[i], GTextAlignmentCenter);
    text_layer_set_text(temp[i], s_textBase[i]);
    layer_add_child(layer, text_layer_get_layer(temp[i]));
  }
}

static void update_hours(Layer *layer, GContext *gContext){
  printf("Update hours");
  fillRow(layer, gContext, GRect(20, 20, 104, 24), 5, s_bufferTime[0]);
}

static void update_minutes(Layer *layer, GContext *gContext){
  printf("Update minutes");
  fillRow(layer, gContext, GRect(20, 20, 104, 24), 6, s_bufferTime[1]);
}

static void window_load(Window *window){
  Layer *window_layer = window_get_root_layer(window);
  GRect windowBounds = layer_get_bounds(window_layer);
  
  update_time();
  s_dotsHours = layer_create(GRect(0, 21, windowBounds.size.w, 40));
  layer_add_child(window_get_root_layer(window), s_dotsHours);
  layer_set_update_proc(s_dotsHours, &update_hours);

  s_dotsMinutes = layer_create(GRect(0, 61, windowBounds.size.w, 40));
  layer_add_child(window_get_root_layer(window), s_dotsMinutes);
  layer_set_update_proc(s_dotsMinutes, &update_minutes);
}

static void window_unload(){
  //Destroy TextLayer
  text_layer_destroy(s_layerHours);
  text_layer_destroy(s_layerMinutes);
}

static void init(){
  // Create main window view
  s_window = window_create();
  
  // Set window handlers
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload
  });
  
  // Show the window on the watch
  window_stack_push(s_window, true);
  
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit(){ 
  // Destroy Window
  window_destroy(s_window);
}

int main(void){
  init();
  app_event_loop();
  deinit();
}