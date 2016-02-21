#include "main.h"
#include "shapes_maker.h"
#include "common.h"

#include "settings.c"

void dec2binTime(int hour, int minute){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
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

static void battery_callback(BatteryChargeState state) {
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  battery_level = (int)state.charge_percent;
  layer_mark_dirty(s_mainLayer);
}

static void bluetooth_handler(bool connected){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  bluetooth_status = connected ? 1 : 0;
  if(bluetooth != BT_NEVER && bluetooth_status == 0){
    vibes_enqueue_custom_pattern(bt_vibe);
  }

  layer_mark_dirty(s_mainLayer);
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
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
        if(DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "shape: %d", shape);
        break;
      case COLOR_KEY:
        color = t->value->uint8;
        color = color % COLOR_NUM;
        persist_write_int(COLOR_KEY, color);
        if(DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "color: %d", color);
        break;
      case NUMBER_KEY:
        number = t->value->uint8;
        number = number % 2;
        persist_write_int(NUMBER_KEY, number);
        if(DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "number: %d", number);
        break;
      case BLUETOOTH_KEY:
        bluetooth = t->value->uint8;
        bluetooth = bluetooth % 3;
        persist_write_int(BLUETOOTH_KEY, bluetooth);
        if(DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "bluetooth: %d", bluetooth);
        break;      
      case BATTERY_KEY:
        battery = t->value->uint8;
        battery = battery % 3;
        persist_write_int(BATTERY_KEY, battery);
        if(DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "battery: %d", battery);
        break;
      case BATTERY_MOD_KEY:
        battery_modality = t->value->uint8;
        battery_modality = battery_modality % 2;
        persist_write_int(BATTERY_MOD_KEY, battery_modality);
        if(DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "battery_modality: %d", battery_modality);
        break;
      case DATE_KEY:
        date = t->value->uint8;
        date = date % DATE_OPTIONS;
        persist_write_int(DATE_KEY, date);
        if(DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "date: %d", date);
        break;
      case HELP_NUM_KEY:
        help_num = t->value->uint8;
        help_num = help_num % 2;
        persist_write_int(HELP_NUM_KEY, help_num);
        if(DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "help numbers: %d", help_num);
        break;
      case SNOW_KEY:
        snow = t->value->uint8;
        snow = snow % 2;
        persist_write_int(SNOW_KEY, snow);
        if(DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "snow option: %d", snow);
        break;
    }
    // Get next pair, if any
    t = dict_read_next(iterator);
  }
  layer_mark_dirty(s_mainLayer);
}

static void inbox_dropped_callback(AppMessageResult app_message_error, void *context) {
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_ERROR, "AppMessageResult error: %d (%s)", app_message_error, translate_error(app_message_error));
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_ERROR, "Heap free: %d", (unsigned int)heap_bytes_free());
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_ERROR, "Heap used: %d", (unsigned int)heap_bytes_used());
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void update_time(){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);

  time_t currentTime = time(NULL);
  struct tm* cTime = localtime(&currentTime);

  if(clock_is_24h_style()){
    hour = cTime->tm_hour;
  }else{
    hour = (cTime->tm_hour == 12) ? 12 : (cTime->tm_hour % 12);
  }
  minute = cTime->tm_min;
  
  /////////////////
  // hour = 18;
  // minute = 26;
  /////////////////

  dec2binTime(hour, minute);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);

  update_time();
  layer_mark_dirty(s_mainLayer);
}

static void update_view(Layer *layer, GContext *gContext){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  int esternEgg = isEasterEggDay();
  if(esternEgg != 0){
    #ifdef PBL_PLATFORM_APLITE
      color = 0;
    #else
      shape = (esternEgg == 2) ? 11 : shape;
      color = (esternEgg == 1) ? 15 : 16;
    #endif
  }
    
  // Background
  draw_background(gContext, 0, GCornerNone, palette[color]);
  
  // Time in background
  if(number > 0)
    draw_time_background(gContext, palette[color]);

  // Dots and help numbers
  draw_clock(gContext, palette[color], (bool)help_num);
  
  // Bluetooth
  if(bluetooth > 0)
    draw_bluetooth(gContext);
  
  // Battery
  if(battery > 0)
    draw_battery(gContext, battery, palette[color]); 

  // Print date
  if(date > 0)
    draw_date(gContext, palette[color]);

  // is easter egg
  if(isEasterEggDay() != 0 || snow){
    for(int i=0; i<NUM_FLAKES; i++){
      draw_flake(gContext, s_flakeLayer[i], flakes[i]);
    }    
  }
}

static void window_load(Window *window){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  Layer *window_layer = window_get_root_layer(window);
  GRect windowBounds = layer_get_bounds(window_layer);
  
  s_mainLayer = layer_create((GRect){.origin={0, 0}, .size={windowBounds.size.w, windowBounds.size.h}});
  layer_add_child(window_layer, s_mainLayer);
  
  s_bulletsNumber[0] = clock_is_24h_style() ? 5 : 4;
  s_bulletsNumber[1] = 6;
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "bullets: %d - %d", s_bulletsNumber[0], s_bulletsNumber[1]);

  for(int i=0; i<NUM_FLAKES; i++){
    s_flakeLayer[i] = layer_create(GRect(0, 0, 16, 16));
    layer_add_child(window_layer, s_flakeLayer[i]);
  }

  layer_set_update_proc(s_mainLayer, update_view);
  
  bluetooth_handler(bluetooth_connection_service_peek());

  battery_callback(battery_state_service_peek());
}

static void window_unload(){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  layer_destroy(s_mainLayer);
}

static void init(){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);

  snprintf(bufferLocale, sizeof(bufferLocale), "%s", i18n_get_system_locale());
  
  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  // Open AppMessage to transfers
  app_message_open(80, 80);
  
  shape = 0;
  color = 0;
  number = 1;
  bluetooth = 2;
  battery = 2;
  battery_modality = 0;
  date = 23;
  help_num = 1;
  snow = 0;
  if(persist_exists(SHAPE_KEY)){
		shape = persist_read_int(SHAPE_KEY);
    shape = shape % SHAPE_NUM;
  }
  if(persist_exists(COLOR_KEY)){
		color = persist_read_int(COLOR_KEY);
    color = color % COLOR_NUM;
  }
  if(persist_exists(NUMBER_KEY)){
		number = persist_read_int(NUMBER_KEY);
    number = number % 2;
  }
  if(persist_exists(BLUETOOTH_KEY)){
    bluetooth = persist_read_int(BLUETOOTH_KEY);
    bluetooth = bluetooth % BLUETOOTH_OPTIONS;
  }
  if(persist_exists(BATTERY_KEY)){
    battery = persist_read_int(BATTERY_KEY);
    battery = battery % BATTERY_OPTIONS;
  }
  if(persist_exists(BATTERY_MOD_KEY)){
    battery_modality = persist_read_int(BATTERY_MOD_KEY);
    battery_modality = battery_modality % 2;
  }
  if(persist_exists(DATE_KEY)){
    date = persist_read_int(DATE_KEY);
    date = date % DATE_OPTIONS;
  }
  if(persist_exists(HELP_NUM_KEY)){
    help_num = persist_read_int(HELP_NUM_KEY);
    help_num = help_num % 2;
  }
  if(persist_exists(SNOW_KEY)){
    snow = persist_read_int(SNOW_KEY);
    snow = snow % 2;
  }
  
  // Create the colors palette
  #ifdef PBL_COLOR
  //                     background             text                         strokeDot                    fillDot                  time background
    palette[0]  = (Color){GColorWhite,          GColorBlack,                 GColorBlack,                 GColorDarkGray,          GColorLightGray};         // Light
    palette[1]  = (Color){GColorBlack,          GColorWhite,                 GColorLightGray,             GColorWhite,             GColorDarkGray};          // Dark
    palette[2]  = (Color){GColorCeleste,        GColorDukeBlue,              GColorDukeBlue,              GColorBlueMoon,          GColorCyan};              // Sky
    palette[3]  = (Color){GColorOxfordBlue,     GColorCeleste,               GColorTiffanyBlue,           GColorCeleste,           GColorBlue};              // Deap Blue
    palette[4]  = (Color){GColorMelon,          GColorBulgarianRose,         GColorBulgarianRose,         GColorDarkCandyAppleRed, GColorSunsetOrange};      // Love
    palette[5]  = (Color){GColorBulgarianRose,  GColorMelon,                 GColorSunsetOrange,          GColorMelon,             GColorDarkCandyAppleRed}; // Passion
    palette[10] = (Color){GColorWindsorTan,     GColorIcterine,              GColorIcterine,              GColorRajah,             GColorOrange};            // Clean Heart
    palette[11] = (Color){GColorIcterine,       GColorWindsorTan,            GColorWindsorTan,            GColorOrange,            GColorRajah};             // Orange Gloom
    palette[6]  = (Color){GColorInchworm,       GColorDarkGreen,             GColorDarkGreen,             GColorMayGreen,          GColorBrightGreen};       // Hope
    palette[7]  = (Color){GColorDarkGreen,      GColorScreaminGreen,         GColorGreen,                 GColorScreaminGreen,     GColorKellyGreen};        // Freedom
    palette[8]  = (Color){GColorBlack,          GColorChromeYellow,          GColorChromeYellow,          GColorYellow,            GColorDarkGray};          // Wisdom
    palette[9]  = (Color){GColorWhite,          GColorBulgarianRose,         GColorBulgarianRose,         GColorRed,               GColorLightGray};         // Trick time
    palette[12] = (Color){GColorBlack,          GColorJaegerGreen,           GColorJaegerGreen,           GColorGreen,             GColorDarkGray};          // Bash
    palette[13] = (Color){GColorImperialPurple, GColorRichBrilliantLavender, GColorRichBrilliantLavender, GColorMagenta,           GColorPurple};            // Plum
    palette[14] = (Color){GColorDarkGreen,      GColorLimerick,              GColorLimerick,              GColorYellow,            GColorArmyGreen};         // Summer Grass
    palette[15] = (Color){GColorRed,            GColorDarkGreen,             GColorDarkGreen,             GColorDarkGreen,         GColorRed};               // Christmas
    palette[16] = (Color){GColorBlack,          GColorPastelYellow,          GColorPastelYellow,          GColorChromeYellow,      GColorBlack};             // Christmas
  #else
    palette[0]  = (Color){GColorWhite,         GColorBlack,         GColorBlack,         GColorBlack,             GColorBlack};
    palette[1]  = (Color){GColorBlack,         GColorWhite,         GColorWhite,         GColorWhite,             GColorWhite};
    palette[2]  = (Color){GColorBlack,         GColorWhite,         GColorWhite,         GColorWhite,             GColorWhite};
  #endif
  
  int flakesSize = sizeof(flakes) / sizeof(flakes[0]);
  int x, y, size;
  #ifdef PBL_IF_RECT_ELSE
    int windowWidth = 144;
    int windowHeight = 168;
  #else
    int windowWidth = 180;
    int windowHeight = 180;
  #endif
    
  // Intializes random number generator
  srand(time(NULL));
  for(int i=0; i<flakesSize; i++){
    x = rand() % windowWidth;
    y = rand() % windowHeight;
    size = rand() % 6;
    flakes[i].pos = (GPoint){x, y};
    flakes[i].size = size;
  }
  
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
  
  // Register for battery level updates
  battery_state_service_subscribe(battery_callback);
}

static void deinit(){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  // Destroy Window
  window_destroy(s_window);
}

int main(void){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  init();
  app_event_loop();
  deinit();
}