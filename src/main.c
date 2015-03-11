#include "main.h"

static Window *s_window;             // main window view
static Layer *s_mainLayer;           // bullets for hours
char *s_textBase[]={"1","2","4","8","16","32"};
static int s_bufferTime[2][6];
static TextLayer *text[2][6];

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
  unsigned int hour, minute;
  hour = (cTime->tm_hour == 12) ? 12 : (cTime->tm_hour % 12);
  minute = cTime->tm_min;
  
  printf("%d:%d", hour, minute);
  
  dec2bin(hour, 0);
  printf("%d %d %d %d %d %d", s_bufferTime[0][5], s_bufferTime[0][4], s_bufferTime[0][3], s_bufferTime[0][2], s_bufferTime[0][1], s_bufferTime[0][0]);
  
  dec2bin(minute, 1);
  printf("%d %d %d %d %d %d", s_bufferTime[1][5], s_bufferTime[1][4], s_bufferTime[1][3], s_bufferTime[1][2], s_bufferTime[1][1], s_bufferTime[1][0]);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed){
  update_time();
  layer_mark_dirty(s_mainLayer);
}

static void fillRow(Layer *layer, GContext *gContext, GRect layerRect, int bulletsNumber, int *s_bufferTime, int textLayerIndex){
  int currentWidth, currentHeight;
  int widthSingleLayer;
  
  printf("fillRow (%d): %d %d %d %d %d %d", textLayerIndex, s_bufferTime[5], s_bufferTime[4], s_bufferTime[3], s_bufferTime[2], s_bufferTime[1], s_bufferTime[0]);
  
  // trick to simulate the round function
  widthSingleLayer = (int)(layerRect.size.w/(bulletsNumber-1));
  int wRest = layerRect.size.w%(bulletsNumber-1);
  if(wRest > (bulletsNumber-1)/2){
    widthSingleLayer++;
  }

  for(int i=0; i<bulletsNumber; i++){
    if(i == (bulletsNumber - 1)){
      currentWidth = layerRect.origin.x;
    }else if(i == 0){
      currentWidth = layerRect.origin.x + layerRect.size.w;
    }else{
      currentWidth = layerRect.origin.x + widthSingleLayer * (bulletsNumber - i - 1);
    }
    currentHeight = layerRect.origin.y + 24;
    if(s_bufferTime[i] == 1){
      graphics_fill_circle(gContext, GPoint(currentWidth, currentHeight), 8);
    }else{
      graphics_draw_circle(gContext, GPoint(currentWidth, currentHeight), 8);
    }
    graphics_context_set_fill_color(gContext, GColorBlack);
    if(!text[textLayerIndex][i]){
      text[textLayerIndex][i] = text_layer_create((GRect){.origin={currentWidth-8, layerRect.origin.y}, .size={16, 18}});
      text_layer_set_background_color(text[textLayerIndex][i], GColorClear);
      text_layer_set_font(text[textLayerIndex][i], fonts_get_system_font(FONT_KEY_GOTHIC_14));
      text_layer_set_text_color(text[textLayerIndex][i], GColorBlack);
      text_layer_set_text_alignment(text[textLayerIndex][i], GTextAlignmentCenter);
      text_layer_set_text(text[textLayerIndex][i], s_textBase[i]);
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Valore della label: s_textBase[%d]=%s", i, s_textBase[i]);
      layer_add_child(layer, text_layer_get_layer(text[textLayerIndex][i]));
    }
  }
}

static void update_time_view(Layer *layer, GContext *gContext){
  fillRow(layer, gContext, (GRect){.origin={20, 20}, .size={104, 24}}, 5, s_bufferTime[0], 0);
  fillRow(layer, gContext, (GRect){.origin={20, 60}, .size={104, 24}}, 6, s_bufferTime[1], 1);
}

static void window_load(Window *window){
  Layer *window_layer = window_get_root_layer(window);
  GRect windowBounds = layer_get_bounds(window_layer);

  s_mainLayer = layer_create((GRect){.origin={0, 0}, .size={windowBounds.size.w, windowBounds.size.h}});
  layer_add_child(window_layer, s_mainLayer);
  layer_set_update_proc(s_mainLayer, update_time_view);
}

static void window_unload(){
  layer_destroy(s_mainLayer);
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