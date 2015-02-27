
#include "main.h"

static Window *s_window;             // main window view
static TextLayer *s_layerHours;      // text layer for hours
static TextLayer *s_layerMinutes;    // test layer for minutes
static Layer *s_dotsHours;           // bullets for hours
static Layer *s_dotsMinutes;         // bullets for minutes
char *s_textBase[]={"1","2","4","8","16","32"};
static TextLayer *temp[6];

static void fillRow(Layer *layer, GContext *gContext, GPoint startPosition, int width, int height, int bulletsNumber){
  int currentWidth;
  int widthSingleLayer = (int) width/(bulletsNumber-1);
  printf("%d / %d = %d", width , bulletsNumber, widthSingleLayer);
  for(int i=0; i<bulletsNumber; i++){
    if(i==0){
      currentWidth = startPosition.x;
    }else if(i==(bulletsNumber-1)){
      currentWidth = startPosition.x + width;
    }else{
      currentWidth = startPosition.x + widthSingleLayer * i;
    }
    graphics_context_set_fill_color(gContext,GColorBlack);
    printf("width: %d", currentWidth);
    graphics_fill_circle(gContext, GPoint(currentWidth,24), 8);
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
  fillRow(layer, gContext, GPoint(20, 20), 110, 24, 5);
}

static void update_minutes(Layer *layer, GContext *gContext){
  fillRow(layer, gContext, GPoint(20, 20), 110, 24, 6);
}

static void window_load(Window *window){
  Layer *window_layer = window_get_root_layer(window);
  GRect windowBounds = layer_get_bounds(window_layer);
  
  s_dotsHours = layer_create(GRect(0, 21, windowBounds.size.w, 40));
  layer_add_child(window_get_root_layer(window), s_dotsHours);
  layer_set_update_proc(s_dotsHours, update_hours);

  s_dotsMinutes = layer_create(GRect(0, 61, windowBounds.size.w, 40));
  layer_add_child(window_get_root_layer(window), s_dotsMinutes);
  layer_set_update_proc(s_dotsMinutes, update_minutes);
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