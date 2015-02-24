
#include "main.h"

static Window *s_window;             // main window view
static TextLayer *s_layerHours;      // text layer for hours
static TextLayer *s_layerMinutes;    // test layer for minutes
static Layer *s_dotsHours;           // bullets for hours
//static Layer *s_dotsMinutes;         // bullets for minutes
char *s_textBase[]={"1","2","4","8","16","32"};
static TextLayer *temp[6];

static void fillRow(Layer *layer, GContext *gContext, GPoint startPosition, int width, int height, int bulletsNumber){
  for(int i=0; i<bulletsNumber; i++){
    graphics_context_set_fill_color(gContext,GColorBlack);
    graphics_fill_circle(gContext, GPoint((20*(i+1)),24), 8);
    temp[i] = text_layer_create(GRect((20*(i+1)-8), 0, 16, 18));
    text_layer_set_background_color(temp[i], GColorClear);
    text_layer_set_font(temp[i], fonts_get_system_font(FONT_KEY_GOTHIC_14));
    text_layer_set_text_color(temp[i], GColorBlack);
    text_layer_set_text_alignment(temp[i], GTextAlignmentCenter);
    text_layer_set_text(temp[i], s_textBase[i]);
    layer_add_child(layer, text_layer_get_layer(temp[i]));
  }
}

/*static void update_proc_dotsMinutes(Layer *layer, GContext *gContext){
  graphics_context_set_fill_color(gContext, GColorBlack);
  graphics_fill_circle(gContext, GPoint(20,8), 8);
  graphics_context_set_fill_color(gContext, GColorBlack);
  graphics_fill_circle(gContext, GPoint(40,8), 8);
  graphics_context_set_fill_color(gContext, GColorBlack);
  graphics_fill_circle(gContext, GPoint(60,8), 8);
  graphics_context_set_fill_color(gContext, GColorBlack);
  graphics_fill_circle(gContext, GPoint(80,8), 8);
  graphics_context_set_fill_color(gContext, GColorBlack);
  graphics_fill_circle(gContext, GPoint(100,8), 8);
  graphics_context_set_fill_color(gContext, GColorBlack);
  graphics_fill_circle(gContext, GPoint(120,8), 8);
}*/

static void update_hours(Layer *layer, GContext *gContext){
  fillRow(layer, gContext, GPoint(20, 20), 24, 120, 6);
}

static void window_load(Window *window){
//  s_layerMinutes = text_layer_create(GRect(0, 61, 144, 40));
//  text_layer_set_background_color(s_layerMinutes, GColorClear);
//  text_layer_set_text_color(s_layerMinutes, GColorBlack);
//  text_layer_set_text(s_layerMinutes, "32   16   8   4   2   1");
  
  s_dotsHours = layer_create(GRect(0, 21, 144, 40));
  layer_add_child(window_get_root_layer(window), s_dotsHours);
  layer_set_update_proc(s_dotsHours, update_hours);

//  s_dotsMinutes = layer_create(GRect(0, 81, 144, 40));
//  layer_add_child(window_get_root_layer(window), s_dotsMinutes);
//  layer_set_update_proc(s_dotsMinutes, update_proc_dotsMinutes);

  // Improve the style
//  text_layer_set_font(s_layerHours, fonts_get_system_font(FONT_KEY_GOTHIC_18));
//  text_layer_set_text_alignment(s_layerHours, GTextAlignmentCenter);
//  text_layer_set_font(s_layerMinutes, fonts_get_system_font(FONT_KEY_GOTHIC_18));
//  text_layer_set_text_alignment(s_layerMinutes, GTextAlignmentCenter);
  
  // Add it as a child layer to the Window's root layer
//  layer_add_child(window_get_root_layer(s_window), text_layer_get_layer(s_layerHours));
//  layer_add_child(window_get_root_layer(s_window), text_layer_get_layer(s_layerMinutes));
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