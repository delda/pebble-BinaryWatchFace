#include "main.h"

char *logTime(){
  time_t now = time(NULL);
  strftime(bufferTime, 20, "%Y-%m-%d %H:%M:%S.000", localtime(&now));
  return bufferTime;
}

void dec2bin(int number, int hORm){
  APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
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

static void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  APP_LOG(APP_LOG_LEVEL_ERROR, "App Message Sync Error: %d", app_message_error);
}

static void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  switch (key) {
    case SHAPE_KEY:
      shape = new_tuple->value->uint8;
      persist_write_int(SHAPE_KEY, shape);
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Shape: %u", shape);
      break;

    case COLOR_KEY:
      break;

    case BATTERY_KEY:
      break;

    case BLUETOOTH_KEY:
      break;

    case FLIP_PHONE_KEY:
      break;
  }
}

static void update_time(){
  APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  time_t currentTime = time(NULL);
  struct tm* cTime = localtime(&currentTime);
  unsigned int hour, minute;
  if(clock_is_24h_style()){
    hour = cTime->tm_hour;
  }else{
    hour = (cTime->tm_hour == 12) ? 12 : (cTime->tm_hour % 12);
  }
  minute = cTime->tm_min;
  
  printf("[%s] Time: %d:%d", logTime(), hour, minute);
  
  dec2bin(hour, 0);
  dec2bin(minute, 1);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed){
  APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  update_time();
  layer_mark_dirty(s_mainLayer);
}

static void update_view(Layer *layer, GContext *gContext){
  APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  int currentWidth, currentHeight;
  int widthSingleLayer;
  
  // Foreach row
  for(int j=0; j<2; j++){
    // trick to simulate the round function
    widthSingleLayer = (int)(s_layerRect[j].size.w/(s_bulletsNumber[j]-1));
    int wRest = s_layerRect[j].size.w%(s_bulletsNumber[j]-1);
    if(wRest > (s_bulletsNumber[j]-1)/2){
      widthSingleLayer++;
    }

    // Foreach binary value
    for(int i=0; i<s_bulletsNumber[j]; i++){
      if(i == (s_bulletsNumber[j] - 1)){
        currentWidth = s_layerRect[j].origin.x;
      }else if(i == 0){
        currentWidth = s_layerRect[j].origin.x + s_layerRect[j].size.w;
      }else{
        currentWidth = s_layerRect[j].origin.x + widthSingleLayer * (s_bulletsNumber[j] - i - 1);
      }
      currentHeight = s_layerRect[j].origin.y + 24;
      if(s_bufferTime[j][i] == 1){
        graphics_fill_circle(gContext, GPoint(currentWidth, currentHeight), 8);
      }else{
        graphics_draw_circle(gContext, GPoint(currentWidth, currentHeight), 8);
      }
      graphics_context_set_fill_color(gContext, GColorBlack);
      // set text
      graphics_context_set_text_color(gContext, GColorBlack);
      graphics_draw_text(gContext, 
                         s_textBase[i], 
                         fonts_get_system_font(FONT_KEY_GOTHIC_14),
                         (GRect){.origin={currentWidth-8, s_layerRect[j].origin.y}, .size={16, 18}}, 
                         GTextOverflowModeWordWrap,
                         GTextAlignmentCenter,
                         NULL
                        );

    }
  }
}

static void window_load(Window *window){
  APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  Layer *window_layer = window_get_root_layer(window);
  GRect windowBounds = layer_get_bounds(window_layer);

  s_mainLayer = layer_create((GRect){.origin={0, 0}, .size={windowBounds.size.w, windowBounds.size.h}});
  layer_add_child(window_layer, s_mainLayer);
  
  s_bulletsNumber[0] = clock_is_24h_style() ? 5 : 4;
  s_bulletsNumber[1] = 6;

  s_layerRect[0] = (GRect){.origin={20, 20}, .size={104, 24}};
  s_layerRect[1] = (GRect){.origin={20, 60}, .size={104, 24}};
  
  layer_set_update_proc(s_mainLayer, update_view);
  
  Tuplet initial_values[] = {
    TupletInteger(SHAPE_KEY,      (uint8_t) 0),
    TupletInteger(COLOR_KEY,      (uint8_t) 0),
    TupletInteger(BATTERY_KEY,    (uint8_t) 1),
    TupletInteger(BLUETOOTH_KEY,  (uint8_t) 1),
    TupletInteger(FLIP_PHONE_KEY, (uint8_t) 1),
  };
  
  app_sync_init(&sync, sync_buffer, sizeof(sync_buffer), initial_values, ARRAY_LENGTH(initial_values),
                sync_tuple_changed_callback, sync_error_callback, NULL);
}

static void window_unload(){
  APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  layer_destroy(s_mainLayer);
}

static void init(){
  APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  if(persist_exists(SHAPE_KEY)){
		shape = persist_read_int(SHAPE_KEY);
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Read text alignment from store: %u", shape);
	}
  
  // Create main window view
  s_window = window_create();
  #ifdef PBL_COLOR
    window_set_background_color(s_window, GColorWhite);
  #else
    window_set_background_color(s_window, GColorWhite);
  #endif
  
  // Set window handlers
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload
  });
  
  // Show the window on the watch
  window_stack_push(s_window, true);
  
  // Open AppMessage to transfers
  app_message_open(64, 64);
  
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit(){
  APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  // Destroy Window
  window_destroy(s_window);
}

int main(void){
  APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  init();
  app_event_loop();
  deinit();
}