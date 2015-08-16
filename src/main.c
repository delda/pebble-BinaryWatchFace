#include "main.h"

char *logTime(){
  time_t now = time(NULL);
  strftime(bufferTime, 20, "%Y-%m-%d %H:%M:%S.000", localtime(&now));
  return bufferTime;
}

void dec2binTime(int hour, int minute){
  APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  int bufferSize = 6;
  int d;
  int time[] = {hour, minute};
  
  for(int i=0; i<2; i++){
    for(int j=bufferSize-1; j>=0; j--){
      d = time[i] >> j;
      if(d & 1){
        s_bufferTime[i][j] = 1;
      }else{
        s_bufferTime[i][j] = 0;
      }
    }    
  }
}

char *translate_error(AppMessageResult result) {
  switch (result) {
    case APP_MSG_OK: return "APP_MSG_OK";
    case APP_MSG_SEND_TIMEOUT: return "APP_MSG_SEND_TIMEOUT";
    case APP_MSG_SEND_REJECTED: return "APP_MSG_SEND_REJECTED";
    case APP_MSG_NOT_CONNECTED: return "APP_MSG_NOT_CONNECTED";
    case APP_MSG_APP_NOT_RUNNING: return "APP_MSG_APP_NOT_RUNNING";
    case APP_MSG_INVALID_ARGS: return "APP_MSG_INVALID_ARGS";
    case APP_MSG_BUSY: return "APP_MSG_BUSY";
    case APP_MSG_BUFFER_OVERFLOW: return "APP_MSG_BUFFER_OVERFLOW";
    case APP_MSG_ALREADY_RELEASED: return "APP_MSG_ALREADY_RELEASED";
    case APP_MSG_CALLBACK_ALREADY_REGISTERED: return "APP_MSG_CALLBACK_ALREADY_REGISTERED";
    case APP_MSG_CALLBACK_NOT_REGISTERED: return "APP_MSG_CALLBACK_NOT_REGISTERED";
    case APP_MSG_OUT_OF_MEMORY: return "APP_MSG_OUT_OF_MEMORY";
    case APP_MSG_CLOSED: return "APP_MSG_CLOSED";
    case APP_MSG_INTERNAL_ERROR: return "APP_MSG_INTERNAL_ERROR";
    default: return "UNKNOWN ERROR";
  }
}

char *debug_dictionary_result( DictionaryResult result ) {
	switch( result ) {
		case DICT_OK: return "DICT_OK";
		case DICT_NOT_ENOUGH_STORAGE: return "DICT_NOT_ENOUGH_STORAGE";
		case DICT_INVALID_ARGS: return "DICT_INVALID_ARGS";	
		case DICT_INTERNAL_INCONSISTENCY: return "DICT_INTERNAL_INCONSISTENCY";
		case DICT_MALLOC_FAILED: return"DICT_MALLOC_FAILED";
    default: return "UNKNOW ERROR";
	}
}

static void bluetooth_handler(bool connected){
  APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  bluetooth_status = connected;
  if(bluetooth != BT_NEVER){
    if(bluetooth_status == 0){
      vibes_enqueue_custom_pattern(pat);
    }else if(bluetooth == BT_ALWAYS){
      vibes_enqueue_custom_pattern(pat);
    }
  }
}

static void fill_number(int number, GPoint position, GContext *gContext){
  APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  switch(number){
    case 0:
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y +  0, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x + 46, position.y +  0, 14, 90), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y + 76, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y +  0, 14, 90), 0, GCornerNone);
      break;
    case 1:
      graphics_fill_rect(gContext, GRect(position.x +  4, position.y +  0, 32, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x + 24, position.y +  0, 14, 90), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  4, position.y + 76, 52, 14), 0, GCornerNone);
      break;
    case 2:
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y +  0, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y + 36, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y + 76, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x + 46, position.y +  0, 14, 50), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y + 36, 14, 50), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y +  0, 14, 24), 0, GCornerNone);
      break;
    case 3:
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y +  0, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y + 76, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  8, position.y + 36, 52, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x + 46, position.y +  0, 14, 90), 0, GCornerNone);
      break;
    case 4:
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y +  0, 14, 52), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x + 46, position.y +  0, 14, 90), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y + 38, 60, 14), 0, GCornerNone);
      break;
    case 5:
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y +  0, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y + 36, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y + 76, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y +  0, 14, 50), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x + 46, position.y + 36, 14, 50), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y + 66, 14, 24), 0, GCornerNone);
      break;
    case 6:
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y +  0, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y + 36, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y + 76, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y +  0, 14, 90), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x + 46, position.y + 36, 14, 50), 0, GCornerNone);
      break;
    case 7:
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y +  0, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x + 46, position.y +  0, 14, 90), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y +  0, 14, 24), 0, GCornerNone);
      break;
    case 8:
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y +  0, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y + 36, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y + 76, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y +  0, 14, 90), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x + 46, position.y +  0, 14, 90), 0, GCornerNone);
      break;
    case 9:
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y +  0, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y + 36, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y + 76, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x + 46, position.y +  0, 14, 90), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y +  0, 14, 50), 0, GCornerNone);
      break;
  }
}

static void draw_number(int number, GPoint position, GContext *gContext){
  APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  switch(number){
    case 0:
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y +  0), GPoint(position.x + 60, position.y +  0));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y +  0), GPoint(position.x + 60, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y + 90), GPoint(position.x +  0, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y + 90), GPoint(position.x +  0, position.y +  0));
      graphics_draw_line(gContext, GPoint(position.x + 15, position.y + 15), GPoint(position.x + 45, position.y + 15));
      graphics_draw_line(gContext, GPoint(position.x + 45, position.y + 15), GPoint(position.x + 45, position.y + 75));
      graphics_draw_line(gContext, GPoint(position.x + 45, position.y + 75), GPoint(position.x + 15, position.y + 75));
      graphics_draw_line(gContext, GPoint(position.x + 15, position.y + 75), GPoint(position.x + 15, position.y + 15));
      break;
    case 1:
      graphics_draw_line(gContext, GPoint(position.x + 10, position.y +  0), GPoint(position.x + 34, position.y +  0));
      graphics_draw_line(gContext, GPoint(position.x + 34, position.y +  0), GPoint(position.x + 34, position.y + 75));
      graphics_draw_line(gContext, GPoint(position.x + 34, position.y + 75), GPoint(position.x + 52, position.y + 75));
      graphics_draw_line(gContext, GPoint(position.x + 52, position.y + 75), GPoint(position.x + 52, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x + 52, position.y + 90), GPoint(position.x +  4, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x +  4, position.y + 90), GPoint(position.x +  4, position.y + 75));
      graphics_draw_line(gContext, GPoint(position.x +  4, position.y + 75), GPoint(position.x + 22, position.y + 75));
      graphics_draw_line(gContext, GPoint(position.x + 22, position.y + 75), GPoint(position.x + 22, position.y + 15));
      graphics_draw_line(gContext, GPoint(position.x + 22, position.y + 15), GPoint(position.x + 10, position.y + 15));
      graphics_draw_line(gContext, GPoint(position.x + 10, position.y + 15), GPoint(position.x + 10, position.y +  0));
      break;
    case 2:
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y +  0), GPoint(position.x + 60, position.y +  0));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y +  0), GPoint(position.x + 60, position.y + 52));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y + 52), GPoint(position.x + 14, position.y + 52));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 52), GPoint(position.x + 14, position.y + 72));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 72), GPoint(position.x + 60, position.y + 72));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y + 72), GPoint(position.x + 60, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y + 90), GPoint(position.x +  0, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y + 90), GPoint(position.x +  0, position.y + 36));
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y + 36), GPoint(position.x + 46, position.y + 36));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 36), GPoint(position.x + 46, position.y + 14));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 14), GPoint(position.x +  0, position.y + 14));
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y + 14), GPoint(position.x +  0, position.y +  0)); 
      break;
    case 3:
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y +  0), GPoint(position.x + 60, position.y +  0));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y +  0), GPoint(position.x + 60, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y + 90), GPoint(position.x +  0, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y + 90), GPoint(position.x +  0, position.y + 75));
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y + 75), GPoint(position.x + 46, position.y + 75));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 75), GPoint(position.x + 46, position.y + 54));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 54), GPoint(position.x +  8, position.y + 54));
      graphics_draw_line(gContext, GPoint(position.x +  8, position.y + 54), GPoint(position.x +  8, position.y + 36));
      graphics_draw_line(gContext, GPoint(position.x +  8, position.y + 36), GPoint(position.x + 46, position.y + 36));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 36), GPoint(position.x + 46, position.y + 14));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 14), GPoint(position.x +  0, position.y + 14));
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y + 14), GPoint(position.x +  0, position.y +  0));
      break;
    case 4:
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y +  0), GPoint(position.x + 14, position.y +  0));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y +  0), GPoint(position.x + 14, position.y + 36));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 36), GPoint(position.x + 46, position.y + 36));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 36), GPoint(position.x + 46, position.y +  0));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y +  0), GPoint(position.x + 60, position.y +  0));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y +  0), GPoint(position.x + 60, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y + 90), GPoint(position.x + 46, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 90), GPoint(position.x + 46, position.y + 54));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 54), GPoint(position.x +  0, position.y + 54));
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y + 54), GPoint(position.x +  0, position.y +  0));
      break;
    case 5:
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y +  0), GPoint(position.x + 60, position.y +  0));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y +  0), GPoint(position.x + 60, position.y + 14));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y + 14), GPoint(position.x + 14, position.y + 14));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 14), GPoint(position.x + 14, position.y + 36));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 36), GPoint(position.x + 60, position.y + 36));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y + 36), GPoint(position.x + 60, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y + 90), GPoint(position.x +  0, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y + 90), GPoint(position.x +  0, position.y + 66));
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y + 66), GPoint(position.x + 14, position.y + 66));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 66), GPoint(position.x + 14, position.y + 76));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 76), GPoint(position.x + 46, position.y + 76));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 76), GPoint(position.x + 46, position.y + 52));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 52), GPoint(position.x +  0, position.y + 52));
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y + 52), GPoint(position.x +  0, position.y +  0));
      break;
    case 6:
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y +  0), GPoint(position.x + 60, position.y +  0));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y +  0), GPoint(position.x + 60, position.y + 14));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y + 14), GPoint(position.x + 14, position.y + 14));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 14), GPoint(position.x + 14, position.y + 36));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 36), GPoint(position.x + 60, position.y + 36));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y + 36), GPoint(position.x + 60, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y + 90), GPoint(position.x +  0, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y + 90), GPoint(position.x +  0, position.y +  0));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 52), GPoint(position.x + 46, position.y + 52));  
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 52), GPoint(position.x + 46, position.y + 74));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 74), GPoint(position.x + 14, position.y + 74));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 74), GPoint(position.x + 14, position.y + 52));
      break;
    case 7:
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y +  0), GPoint(position.x + 60, position.y +  0));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y +  0), GPoint(position.x + 60, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y + 90), GPoint(position.x + 46, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 90), GPoint(position.x + 46, position.y + 14));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 14), GPoint(position.x + 14, position.y + 14));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 14), GPoint(position.x + 14, position.y + 24));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 24), GPoint(position.x +  0, position.y + 24));
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y + 24), GPoint(position.x +  0, position.y +  0));
      break;
    case 8:
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y +  0), GPoint(position.x + 60, position.y +  0));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y +  0), GPoint(position.x + 60, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y + 90), GPoint(position.x +  0, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y + 90), GPoint(position.x +  0, position.y +  0));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 14), GPoint(position.x + 46, position.y + 14));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 14), GPoint(position.x + 46, position.y + 36));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 36), GPoint(position.x + 14, position.y + 36));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 36), GPoint(position.x + 14, position.y + 14));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 54), GPoint(position.x + 46, position.y + 54));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 54), GPoint(position.x + 46, position.y + 74));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 74), GPoint(position.x + 14, position.y + 74));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 74), GPoint(position.x + 14, position.y + 54));
      break;
    case 9:
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y +  0), GPoint(position.x + 60, position.y +  0));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y +  0), GPoint(position.x + 60, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y + 90), GPoint(position.x +  0, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y + 90), GPoint(position.x +  0, position.y + 76));
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y + 76), GPoint(position.x + 46, position.y + 76));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 76), GPoint(position.x + 46, position.y + 54));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 54), GPoint(position.x +  0, position.y + 54));
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y + 54), GPoint(position.x +  0, position.y +  0));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 14), GPoint(position.x + 46, position.y + 14));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 14), GPoint(position.x + 46, position.y + 36));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 36), GPoint(position.x + 14, position.y + 36));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 36), GPoint(position.x + 14, position.y + 14));
      break;
  }
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  // Get the first pair
  Tuple *t = dict_read_first(iterator);

  // Process all pairs present
  while (t != NULL) {
    // Process this pair's key
    switch (t->key) {
      case SHAPE_KEY:
        shape = t->value->uint8;
        shape = shape % SHAPE_NUM;
        persist_write_int(SHAPE_KEY, shape);
        APP_LOG(APP_LOG_LEVEL_DEBUG, "shape: %d", shape);
        break;
      case COLOR_KEY:
        color = t->value->uint8;
        color = color % COLOR_NUM;
        persist_write_int(COLOR_KEY, color);
        APP_LOG(APP_LOG_LEVEL_DEBUG, "color: %d", color);
        break;
      case NUMBER_KEY:
        number = t->value->uint8;
        number = number % 2;
        persist_write_int(NUMBER_KEY, number);
        APP_LOG(APP_LOG_LEVEL_DEBUG, "number: %d", number);
        break;
      case BLUETOOTH_KEY:
        bluetooth = t->value->uint8;
        bluetooth = bluetooth % 3;
        persist_write_int(BLUETOOTH_KEY, bluetooth);
        APP_LOG(APP_LOG_LEVEL_DEBUG, "bluetooth: %d", bluetooth);
        break;      
    }
    // Get next pair, if any
    t = dict_read_next(iterator);
  }
  layer_mark_dirty(s_mainLayer);
}

static void inbox_dropped_callback(AppMessageResult app_message_error, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
  APP_LOG(APP_LOG_LEVEL_ERROR, "AppMessageResult error: %d (%s)", app_message_error, translate_error(app_message_error));
  APP_LOG(APP_LOG_LEVEL_ERROR, "Heap free: %d", (unsigned int)heap_bytes_free());
  APP_LOG(APP_LOG_LEVEL_ERROR, "Heap used: %d", (unsigned int)heap_bytes_used());
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void update_time(){
  APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);

  time_t currentTime = time(NULL);
  struct tm* cTime = localtime(&currentTime);

  if(clock_is_24h_style()){
    hour = cTime->tm_hour;
  }else{
    hour = (cTime->tm_hour == 12) ? 12 : (cTime->tm_hour % 12);
  }
  minute = cTime->tm_min;
  
  dec2binTime(hour, minute);
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
    
  //color = (color+1) % COLOR_NUM;
  APP_LOG(APP_LOG_LEVEL_ERROR, "color: %d", color);
  
  // Background
  graphics_context_set_fill_color(gContext, palette[color].background);
  graphics_fill_rect(gContext, GRect(0, 0, 144, 168), 0, GCornerNone);
  
  // Time in background
  if(number > 0){
    #ifdef PBL_COLOR
      graphics_context_set_fill_color(gContext, palette[color].time);
      fill_number((hour-(hour%10))/10, (GPoint){10, -6}, gContext);
      fill_number(hour%10, (GPoint){72, -6}, gContext);
      fill_number((minute-(minute%10))/10, (GPoint){10, 84}, gContext);
      fill_number(minute%10, (GPoint){72, 84}, gContext);
    #else
      draw_number((hour-(hour%10))/10, (GPoint){10, -6}, gContext);
      draw_number(hour%10, (GPoint){72, -6}, gContext);
      draw_number((minute-(minute%10))/10, (GPoint){10, 84}, gContext);
      draw_number(minute%10, (GPoint){72, 84}, gContext);
    #endif
  }
  
  // Dots
  graphics_context_set_stroke_color(gContext, palette[color].strokeDot);
  for(int j=0; j<2; j++){
    // trick to simulate the round function
    widthSingleLayer = (int)(s_layerRect[j].size.w/(s_bulletsNumber[j]-1));
    int wRest = s_layerRect[j].size.w%(s_bulletsNumber[j]-1);
    if(wRest > (s_bulletsNumber[j]-1)/2){
      widthSingleLayer++;
    }

    for(int i=0; i<s_bulletsNumber[j]; i++){      
      // Defines the bullets position
      if(i == (s_bulletsNumber[j] - 1)){
        currentWidth = s_layerRect[j].origin.x;
      }else if(i == 0){
        currentWidth = s_layerRect[j].origin.x + s_layerRect[j].size.w;
      }else{
        currentWidth = s_layerRect[j].origin.x + widthSingleLayer * (s_bulletsNumber[j] - i - 1);
      }
      currentHeight = s_layerRect[j].origin.y + 24;
      
      // Draws the dots
      if(s_bufferTime[j][i] == 1){
        switch(shape){
          case 1:     // square
            graphics_context_set_fill_color(gContext, palette[color].fillDot);
            graphics_fill_rect(gContext, (GRect){.origin={currentWidth-8, currentHeight-6}, .size={16,16}}, 0, GCornerNone);
            graphics_draw_rect(gContext, (GRect){.origin={currentWidth-8, currentHeight-6}, .size={16,16}});
            graphics_draw_rect(gContext, (GRect){.origin={currentWidth-7, currentHeight-5}, .size={14,14}});
            break;
          case 0:
          default:    // disk
            graphics_context_set_fill_color(gContext, palette[color].strokeDot);
            graphics_fill_circle(gContext, GPoint(currentWidth, currentHeight), 8);
            graphics_context_set_fill_color(gContext, palette[color].fillDot);
            graphics_fill_circle(gContext, GPoint(currentWidth, currentHeight), 6);
            break;
        }
      }else{
        switch(shape){
          case 1:    // square
            graphics_context_set_fill_color(gContext, palette[color].strokeDot);
            graphics_fill_rect(gContext, (GRect){.origin={currentWidth-8, currentHeight-6}, .size={16,16}}, 0, GCornerNone);
            graphics_context_set_fill_color(gContext, palette[color].background);
            graphics_fill_rect(gContext, (GRect){.origin={currentWidth-6, currentHeight-4}, .size={12,12}}, 0, GCornerNone);
            break;
          case 0:
          default:    // disc
            graphics_context_set_fill_color(gContext, palette[color].strokeDot);
            graphics_fill_circle(gContext, GPoint(currentWidth, currentHeight), 8);
            graphics_context_set_fill_color(gContext, palette[color].background);
            graphics_fill_circle(gContext, GPoint(currentWidth, currentHeight), 6);
            break;
        }
      }
      
      // Prints texts
      graphics_context_set_text_color(gContext, palette[color].text);
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
  
  // Bluetooth
  bt_bitmap = 0;
  if(bluetooth != BT_NEVER){
    if(bluetooth_status == 0){
      bt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BLUETOOTH_OFF_IMG);
    }else if(bluetooth == BT_ALWAYS){
      bt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BLUETOOTH_ON_IMG);
    }
  }
  if(bt_bitmap){
    bt_layer = bitmap_layer_create(GRect(5, 5, 10, 16));
    #ifdef PBL_PLATFORM_APLITE
      bitmap_layer_set_compositing_mode(bt_layer, GCompOpAssign);
    #elif PBL_PLATFORM_BASALT
      bitmap_layer_set_compositing_mode(bt_layer, GCompOpSet);
    #endif
    bitmap_layer_set_bitmap(bt_layer, bt_bitmap);
    layer_add_child(window_get_root_layer(s_window), bitmap_layer_get_layer(bt_layer));
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

  s_layerRect[0] = (GRect){.origin={20, 30}, .size={104, 24}};
  s_layerRect[1] = (GRect){.origin={20, 70}, .size={104, 24}};
  
  layer_set_update_proc(s_mainLayer, update_view);
  
  bluetooth_handler(bluetooth_connection_service_peek());
  
  Tuplet initial_values[] = {
    TupletInteger(SHAPE_KEY,      (uint8_t) 0),
    TupletInteger(COLOR_KEY,      (uint8_t) 0),
    TupletInteger(NUMBER_KEY,     (uint8_t) 1),
    TupletInteger(BLUETOOTH_KEY,  (uint8_t) 1),
  };
  APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "length values: %lu", (long unsigned int)ARRAY_LENGTH(initial_values));
}

static void window_unload(){
  APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  layer_destroy(s_mainLayer);
}

static void init(){
  APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);

  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  // Open AppMessage to transfers
  app_message_open(64, 64);
  
  if(persist_exists(SHAPE_KEY)){
		shape = persist_read_int(SHAPE_KEY);
    shape = shape % SHAPE_NUM;
  }else{
    shape = 0;
  }
  if(persist_exists(COLOR_KEY)){
		color = persist_read_int(COLOR_KEY);
    color = color % COLOR_NUM;
  }else{
    color = 0;
  }
  if(persist_exists(NUMBER_KEY)){
		number = persist_read_int(NUMBER_KEY);
    number = number % 2;
  }else{
    number = 1;
  }
  if(persist_exists(BLUETOOTH_KEY)){
    bluetooth = persist_read_int(BLUETOOTH_KEY);
    bluetooth = bluetooth % 2;
  }else{
    bluetooth = 1;
  }
  
  // Create the colors palette
  #ifdef PBL_COLOR
  //                     background           text                 strokeDot            fillDot                  time background
    palette[0] = (Color){GColorWhite,         GColorBlack,         GColorBlack,         GColorDarkGray,          GColorLightGray};
    palette[1] = (Color){GColorBlack,         GColorWhite,         GColorLightGray,     GColorWhite,             GColorDarkGray};
    palette[2] = (Color){GColorCeleste,       GColorDukeBlue,      GColorDukeBlue,      GColorBlueMoon,          GColorCyan};
    palette[3] = (Color){GColorOxfordBlue,    GColorCeleste,       GColorTiffanyBlue,   GColorCeleste,           GColorBlue};
    palette[4] = (Color){GColorMelon,         GColorBulgarianRose, GColorBulgarianRose, GColorDarkCandyAppleRed, GColorSunsetOrange};
    palette[5] = (Color){GColorBulgarianRose, GColorMelon,         GColorSunsetOrange,  GColorMelon,             GColorDarkCandyAppleRed};
    palette[6] = (Color){GColorInchworm,      GColorDarkGreen,     GColorDarkGreen,     GColorMayGreen,          GColorBrightGreen};
    palette[7] = (Color){GColorDarkGreen,     GColorScreaminGreen, GColorGreen,         GColorScreaminGreen,     GColorKellyGreen};
  #else
    palette[0] = (Color){GColorWhite, GColorBlack, GColorBlack, GColorBlack, GColorBlack};
    palette[1] = (Color){GColorBlack, GColorWhite, GColorWhite, GColorWhite, GColorWhite};
  #endif
    
  // Create main window view
  s_window = window_create();
  window_set_background_color(s_window, GColorWhite);
  
  // Set window handlers
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload
  });
  
  // Show the window on the watch
  window_stack_push(s_window, true);
  
  // Update on new time
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  // Subscribe to Bluetooth updates
  bluetooth_connection_service_subscribe(bluetooth_handler);
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