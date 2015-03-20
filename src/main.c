#include "main.h"

char *logTime(){
  time_t now = time(NULL);
  strftime(bufferTime, 20, "%Y-%m-%d %H:%M:%S.000", localtime(&now));
  return bufferTime;
}

void long_click_up_handler(ClickRecognizerRef recognizer, void *context){}

static void update_time(){
  APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  time_t currentTime = time(NULL);
  struct tm* cTime = localtime(&currentTime);
  unsigned int hour, minute;
  hour = (cTime->tm_hour == 12) ? 12 : (cTime->tm_hour % 12);
  minute = cTime->tm_min;
  
  printf("[%s] Time: %d:%d", logTime(), hour, minute);
  
  dec2bin(hour, 0);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "[%s] Bin h: %d %d %d %d %d %d", logTime(), s_bufferTime[0][5], s_bufferTime[0][4], s_bufferTime[0][3], s_bufferTime[0][2], s_bufferTime[0][1], s_bufferTime[0][0]);
  
  dec2bin(minute, 1);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "[%s] Bin m: %d %d %d %d %d %d", logTime(), s_bufferTime[1][5], s_bufferTime[1][4], s_bufferTime[1][3], s_bufferTime[1][2], s_bufferTime[1][1], s_bufferTime[1][0]);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed){
  APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  update_time();
  layer_mark_dirty(s_mainLayer);
}

void change_shape(ClickRecognizerRef recognizer, void *context){
  APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  if(s_isSettingModality == 1){
    s_shapeType = (s_shapeType+1) % 2;
  }
  APP_LOG(APP_LOG_LEVEL_ERROR, "s_shapeType: %d", s_shapeType);
}

void stop_settings_modality(ClickRecognizerRef recognizer, void *context){
  APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);

  if(s_isSettingModality == 1){
    s_isSettingModality = 0;
    s_updateTime = MINUTE_UNIT;
    tick_timer_service_subscribe(s_updateTime, tick_handler);    
  }
}

void start_settings_modality(ClickRecognizerRef recognizer, void *context){
  APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);

  s_isSettingModality = 1;
  s_updateTime = SECOND_UNIT;
  tick_timer_service_subscribe(s_updateTime, tick_handler);
}

void click_config_provider(void *context){
  APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  window_single_click_subscribe(BUTTON_ID_UP, change_shape);
//  window_single_repeating_click_subscribe(BUTTON_ID_DOWN, 100, down_single_repeating_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, stop_settings_modality);
  window_long_click_subscribe(BUTTON_ID_SELECT, 1000, start_settings_modality, long_click_up_handler);
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

static void fill_row(Layer *layer, GContext *gContext){
  APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  int currentWidth, currentHeight;
  int widthSingleLayer;
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "[%s] %s (0): %d %d %d %d %d %d", logTime(), __func__, s_bufferTime[0][5], s_bufferTime[0][4], s_bufferTime[0][3], s_bufferTime[0][2], s_bufferTime[0][1], s_bufferTime[0][0]);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "[%s] %s (1): %d %d %d %d %d %d", logTime(), __func__, s_bufferTime[1][5], s_bufferTime[1][4], s_bufferTime[1][3], s_bufferTime[1][2], s_bufferTime[1][1], s_bufferTime[1][0]);
  
  for(int i=0; i<2; i++){
    // trick to simulate the round function
    widthSingleLayer = (int)(s_layerRect[i].size.w/(s_bulletsNumber[i]-1));
    int wRest = s_layerRect[i].size.w%(s_bulletsNumber[i]-1);
    if(wRest > (s_bulletsNumber[i]-1)/2){
      widthSingleLayer++;
    }
  
    for(int j=0; j<s_bulletsNumber[i]; j++){
      if(j == (s_bulletsNumber[i] - 1)){
        currentWidth = s_layerRect[i].origin.x;
      }else if(j == 0){
        currentWidth = s_layerRect[i].origin.x + s_layerRect[i].size.w;
      }else{
        currentWidth = s_layerRect[i].origin.x + widthSingleLayer * (s_bulletsNumber[i] - j - 1);
      }
      currentHeight = s_layerRect[i].origin.y;
      if(s_bufferTime[i][j] == 1){
        if(s_shapeType == 0){
          graphics_fill_circle(gContext, GPoint(currentWidth, currentHeight+24), 8);
        }else{
          graphics_fill_rect(gContext, (GRect){.origin={currentWidth-8, currentHeight+18}, .size={16,16}}, 0, GCornerNone);
        }
      }else{
        if(s_shapeType == 0){
          graphics_draw_circle(gContext, GPoint(currentWidth, currentHeight+24), 8);
        }else{
          graphics_draw_rect(gContext, (GRect){.origin={currentWidth-8, currentHeight+18}, .size={16,16}});
          graphics_draw_rect(gContext, (GRect){.origin={currentWidth-7, currentHeight+19}, .size={14,14}});
        }
      }
      graphics_context_set_fill_color(gContext, s_textColor);
      if(text[i][j] == NULL)
        text[i][j] = text_layer_create((GRect){.origin={currentWidth-8, currentHeight}, .size={16, 18}});
      text_layer_set_background_color(text[i][j], s_backgroundColor);
      text_layer_set_font(text[i][j], fonts_get_system_font(FONT_KEY_GOTHIC_14));
      text_layer_set_text_color(text[i][j], s_textColor);
      text_layer_set_text_alignment(text[i][j], GTextAlignmentCenter);
      text_layer_set_text(text[i][j], s_textBase[j]);
      layer_add_child(layer, text_layer_get_layer(text[i][j]));
    }
  }
}

static void update_time_view(Layer *layer, GContext *gContext){
  APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  s_counter++;
  if(s_isSettingModality && s_counter%2){
    s_backgroundColor = GColorClear;
    s_textColor = GColorWhite;
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Colore test: GColorWhite");
    for(int i=0; i<2; i++){
      for(int j=0; j<6; j++){
        text_layer_set_text_color(text[i][j], s_textColor);
      }
    }
    return;
  }else{
    s_backgroundColor = GColorClear;
    s_textColor = GColorBlack;
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Colore test: GColorBlack");
  }
  
  fill_row(layer, gContext);
}

static void window_load(Window *window){
  APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  Layer *window_layer = window_get_root_layer(window);
  GRect windowBounds = layer_get_bounds(window_layer);

  s_counter = 1;
  s_shapeType = 0;
  
  if(clock_is_24h_style()){
    s_bulletsNumber[0] = 5;
  }else{
    s_bulletsNumber[0] = 4;
  }
  s_bulletsNumber[1] = 6;
  
  s_layerRect[0] = (GRect){.origin={20, 20}, .size={104, 24}};
  s_layerRect[1] = (GRect){.origin={20, 60}, .size={104, 24}};
    
  s_mainLayer = layer_create((GRect){.origin={0, 0}, .size={windowBounds.size.w, windowBounds.size.h}});
  layer_add_child(window_layer, s_mainLayer);
  layer_set_update_proc(s_mainLayer, update_time_view);
}

static void window_unload(){
  APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  layer_destroy(s_mainLayer);
}

static void init(){
  APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  // Set standard values
  s_backgroundColor = GColorClear;
  s_textColor = GColorBlack;  
  s_updateTime = MINUTE_UNIT;
  s_isSettingModality = false;
  
  // Create main window view
  s_window = window_create();
  
  window_set_click_config_provider(s_window, click_config_provider);
  
  // Set window handlers
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload
  });
  
  // Show the window on the watch
  window_stack_push(s_window, true);
  
  tick_timer_service_subscribe(s_updateTime, tick_handler);
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