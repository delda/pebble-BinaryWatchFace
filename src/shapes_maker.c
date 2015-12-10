#include <pebble.h>
#include <math.h>
#include "shapes_maker.h"
#include "common.h"

#include "settings.c"

GPathInfo * draw_regular_shape(int number_of_sides, int w, int h, int radius){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  GPathInfo *shape;
  shape = (GPathInfo *) malloc (sizeof(GPathInfo));
  GPoint *points;
  points = (GPoint *) calloc (number_of_sides, sizeof(GPoint));
  int32_t secondAngle = 0;
  for(int i=0; i<=ceil((number_of_sides+1)/2); i++){
    points[i].x = (int16_t)(sin_lookup(secondAngle) * (int32_t)radius / TRIG_MAX_RATIO) + w;
    points[i].y = (int16_t)(-cos_lookup(secondAngle) * (int32_t)radius / TRIG_MAX_RATIO) + h;
    points[(number_of_sides-i)%number_of_sides].x = (int16_t)(sin_lookup(-secondAngle) * (int32_t)radius / TRIG_MAX_RATIO) + w;
    points[(number_of_sides-i)%number_of_sides].y = (int16_t)(-cos_lookup(-secondAngle) * (int32_t)radius / TRIG_MAX_RATIO) + h;
    secondAngle += (int32_t)(TRIG_MAX_ANGLE / number_of_sides);
  }
  shape->num_points = number_of_sides;
  shape->points = points;
  return shape;  
}

void draw_shape(int shape, int currentWidth, int currentHeight, GContext *gContext, GColor strokeColor, GColor fillColor){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  int numberOfSides = 0;
  int border = 0;
  switch(shape){
    case 3:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
      switch(shape){
        case 3: numberOfSides = 4; border = 3; break;
        case 6: numberOfSides = 3; border = 4; break;
        case 7: numberOfSides = 5; border = 2; break;
        case 8: numberOfSides = 6; border = 2; break;
        case 9: numberOfSides = 7; border = 2; break;
        case 10: numberOfSides = 8; border = 2; break;
      }
      #ifndef PBL_PLATFORM_CHALK
        currentHeight += 2;
      #endif
      graphics_context_set_fill_color(gContext, strokeColor);
      gpath_draw_filled(gContext, gpath_create(draw_regular_shape(numberOfSides, currentWidth, currentHeight, 10)));
      graphics_context_set_fill_color(gContext, fillColor);
      gpath_draw_filled(gContext, gpath_create(draw_regular_shape(numberOfSides, currentWidth, currentHeight, 10-border)));
      break;        
    case 5:     // cross
      graphics_context_set_fill_color(gContext, strokeColor);
      int w = currentWidth;
      int h = currentHeight;
      gpath_draw_filled(gContext, gpath_create(&(GPathInfo){.num_points=13, .points=(GPoint []){
        {w-10, h-2}, 
        {w-5 , h-3}, 
        {w-5 , h-8}, 
        {w+5 , h-8}, 
        {w+5 , h-3}, 
        {w+10, h-3}, 
        {w+10, h+6}, 
        {w+5 , h+6}, 
        {w+5 , h+11}, 
        {w-5 , h+11}, 
        {w-5 , h+7}, 
        {w-10, h+7},
        {w-10, h-2}
      }}));
      graphics_context_set_fill_color(gContext, fillColor);
      gpath_draw_filled(gContext, gpath_create(&(GPathInfo){.num_points=13, .points=(GPoint []){
        {w-8, h}, 
        {w-3, h-1}, 
        {w-3, h-6}, 
        {w+3, h-6}, 
        {w+3, h-1}, 
        {w+8, h-1}, 
        {w+8, h+4}, 
        {w+3, h+4}, 
        {w+3, h+9}, 
        {w-3, h+9}, 
        {w-3, h+5}, 
        {w-8, h+5}, 
        {w-8, h-1}
      }}));
      break;
    case 4:     // triangle
      graphics_context_set_fill_color(gContext, strokeColor);
      gpath_draw_filled(gContext, gpath_create(&(GPathInfo){.num_points=3, .points=(GPoint []){{currentWidth, currentHeight-11}, {currentWidth+9, currentHeight+8}, {currentWidth-8, currentHeight+8}}}));
      graphics_context_set_fill_color(gContext, fillColor);
      gpath_draw_filled(gContext, gpath_create(&(GPathInfo){.num_points=3, .points=(GPoint []){{currentWidth, currentHeight-7}, {currentWidth+6, currentHeight+6}, {currentWidth-5, currentHeight+6}}}));
      break;
    case 2:     // rectangle
      graphics_context_set_fill_color(gContext, fillColor);
      graphics_context_set_stroke_color(gContext, strokeColor);
      graphics_fill_rect(gContext, (GRect){.origin={currentWidth-6, currentHeight-7}, .size={12,19}}, 0, GCornerNone);
      graphics_draw_rect(gContext, (GRect){.origin={currentWidth-6, currentHeight-7}, .size={12,19}});
      graphics_draw_rect(gContext, (GRect){.origin={currentWidth-5, currentHeight-6}, .size={10,17}});          
      break;
    case 1:     // square
      graphics_context_set_fill_color(gContext, strokeColor);
      graphics_fill_rect(gContext, (GRect){.origin={currentWidth-8, currentHeight-6}, .size={16,16}}, 0, GCornerNone);
      graphics_context_set_fill_color(gContext, fillColor);
      graphics_fill_rect(gContext, (GRect){.origin={currentWidth-6, currentHeight-4}, .size={12,12}}, 0, GCornerNone);
      break;
    case 0:
    default:    // disk
      graphics_context_set_fill_color(gContext, strokeColor);
      graphics_fill_circle(gContext, GPoint(currentWidth, currentHeight), 8);
      graphics_context_set_fill_color(gContext, fillColor);
      graphics_fill_circle(gContext, GPoint(currentWidth, currentHeight), 6);
      break;
  }
}

void fill_number(int number, GPoint position, GContext *gContext){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);

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

void draw_number(int number, GPoint position, GContext *gContext){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
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

void draw_background(GContext *gContext, uint16_t corner_radius, GCornerMask corner_mask, Color palette){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  #ifdef PBL_ROUND
    GRect rect = GRect(0, 0, 180, 180);
  #else
    GRect rect = GRect(0, 0, 144, 168);
  #endif
  graphics_context_set_fill_color(gContext, palette.background);
  graphics_fill_rect(gContext, rect, corner_radius, corner_mask);
  #ifdef PBL_ROUND
    GRect base = GRect(0, 165, 180, 20);
    graphics_context_set_fill_color(gContext, palette.fillDot);
    graphics_fill_rect(gContext, base, 0, GCornerNone);
  #endif

}

void draw_time_background(GContext *gContext, Color palette){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);

  #ifdef PBL_PLATFORM_APLITE
    graphics_context_set_stroke_color(gContext, palette.time);
    draw_number((hour-(hour%10))/10, (GPoint){10, -6}, gContext);
    draw_number(hour%10, (GPoint){72, -6}, gContext);
    draw_number((minute-(minute%10))/10, (GPoint){10, 84}, gContext);
    draw_number(minute%10, (GPoint){72, 84}, gContext);      
  #elif PBL_PLATFORM_BASALT
    graphics_context_set_fill_color(gContext, palette.time);
    fill_number((hour-(hour%10))/10, (GPoint){10, -6}, gContext);
    fill_number(hour%10, (GPoint){72, -6}, gContext);
    fill_number((minute-(minute%10))/10, (GPoint){10, 84}, gContext);
    fill_number(minute%10, (GPoint){72, 84}, gContext);
  #elif PBL_PLATFORM_CHALK
    graphics_context_set_fill_color(gContext, palette.time);
    fill_number((hour-(hour%10))/10, (GPoint){30, -6}, gContext);
    fill_number(hour%10, (GPoint){92, -6}, gContext);
    fill_number((minute-(minute%10))/10, (GPoint){30, 84}, gContext);
    fill_number(minute%10, (GPoint){92, 84}, gContext);      
  #endif
  #ifdef PBL_ROUND
    GRect base = GRect(0, 165, 180, 20);
    graphics_context_set_fill_color(gContext, palette.fillDot);
    graphics_fill_rect(gContext, base, 0, GCornerNone);
  #endif
}

void draw_clock(GContext *gContext, Color palette, bool drawNumbers){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  int currentWidth, currentHeight;
  int widthSingleLayer;

  #ifdef PBL_PLATFORM_CHALK
    s_layerRect[0] = (GRect){.origin={20, 45}, .size={104, 24}};
    s_layerRect[1] = (GRect){.origin={20, 70}, .size={104, 24}};
  #else
    s_layerRect[0] = (GRect){.origin={20, 30}, .size={104, 24}};
    if(drawNumbers == true){
      s_layerRect[1] = (GRect){.origin={20, 70}, .size={104, 24}};
    }else{
      s_layerRect[1] = (GRect){.origin={20, 60}, .size={104, 24}};
    }
  #endif

  graphics_context_set_stroke_color(gContext, palette.strokeDot);
  for(int j=0; j<2; j++){
    // trick to simulate the round function
    widthSingleLayer = (int)(s_layerRect[j].size.w/(s_bulletsNumber[j]-1));
    int wRest = s_layerRect[j].size.w%(s_bulletsNumber[j]-1);
    if(wRest > (s_bulletsNumber[j]-1)/2){
      widthSingleLayer++;
    }

    for(int i=0; i<s_bulletsNumber[j]; i++){
      // Defines the bullets position width
      if(i == (s_bulletsNumber[j] - 1)){
        currentWidth = s_layerRect[j].origin.x;
      }else if(i == 0){
        currentWidth = s_layerRect[j].origin.x + s_layerRect[j].size.w;
      }else{
        currentWidth = s_layerRect[j].origin.x + widthSingleLayer * (s_bulletsNumber[j] - i - 1);
      }
      currentWidth -= 1;
      // Very simple define of height
      currentHeight = s_layerRect[j].origin.y + 24;
      // Chalk platform correction
      #ifdef PBL_PLATFORM_CHALK
        currentWidth += 20;
        currentHeight += 10;
      #endif
    
      // Draws the dots
      GColor strokeColor, fillColor;
      strokeColor = palette.strokeDot;
      if(s_bufferTime[j][i] == 1){
        fillColor = palette.fillDot;
      }else{
        fillColor = palette.background;
      }
      draw_shape(shape, currentWidth, currentHeight, gContext, strokeColor, fillColor);
  
      // Prints texts
      currentHeight = s_layerRect[j].origin.y;
      #ifdef PBL_PLATFORM_CHALK
        currentHeight += 7;
        if(j == 1){
          currentHeight += 36;
        }
      #endif
      
      graphics_context_set_text_color(gContext, palette.text);
      if(drawNumbers == true){
        graphics_draw_text(gContext, 
                           s_textBase[i], 
                           fonts_get_system_font(FONT_KEY_GOTHIC_14),
                           (GRect){.origin={currentWidth-8, currentHeight}, .size={16, 18}}, 
                           GTextOverflowModeWordWrap,
                           GTextAlignmentCenter,
                           NULL
                          );        
      }
    }
  }
}

void draw_bluetooth(GContext *gContext){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  #if PBL_PLATFORM_APLITE
    if(bt_bitmap_off == NULL)
      bt_bitmap_off = gbitmap_create_with_resource(RESOURCE_ID_BLUETOOTH_BW_OFF_IMG);
    if(bt_bitmap_on == NULL)
      bt_bitmap_on = gbitmap_create_with_resource(RESOURCE_ID_BLUETOOTH_BW_ON_IMG);
  #else
    if(bt_bitmap_off == NULL)
      bt_bitmap_off = gbitmap_create_with_resource(RESOURCE_ID_BLUETOOTH_OFF_IMG);
    if(bt_bitmap_on == NULL)
      bt_bitmap_on = gbitmap_create_with_resource(RESOURCE_ID_BLUETOOTH_ON_IMG);
  #endif

  if(bluetooth == BT_ALWAYS || (bluetooth == BT_ON_DISCONNECT && bluetooth_status == 0)){
    int x = 7;
    int y = 7;
    int w = 10;
    int h = 16;
    #ifdef PBL_PLATFORM_APLITE
      if(bluetooth_status == 1){
        w = 6;
        h = 14;
      }else{
        w = 12;
        h = 13;
      }
    #elif PBL_PLATFORM_BASALT
      y = 6;
    #elif PBL_PLATFORM_CHALK
      x = 85;
      y = 140;
      if((battery == BA_UNDER_20_PERC && battery_level < BA_PERCENT_WARNING) || battery == BA_ALWAYS){
        x -= 20;
      }
    #endif
    GRect rect = GRect(x, y, w, h);
    #ifdef PBL_PLATFORM_APLITE
      graphics_context_set_compositing_mode(gContext, GCompOpOr);
    #else
      graphics_context_set_compositing_mode(gContext, GCompOpSet);
    #endif
    if(bluetooth_status == 0){
      graphics_draw_bitmap_in_rect(gContext, bt_bitmap_off, rect);
    }else{
      graphics_draw_bitmap_in_rect(gContext, bt_bitmap_on, rect);
    }
    #ifdef PBL_PLATFORM_APLITE
      if(color == 0){
        graphics_context_set_compositing_mode(gContext, GCompOpClear);
        if(bluetooth_status == 0){
          graphics_draw_bitmap_in_rect(gContext, bt_bitmap_off, rect);
        }else{
          graphics_draw_bitmap_in_rect(gContext, bt_bitmap_on, rect);
        }        
      }
    #endif
  }
}

void draw_battery(GContext *gContext, int battery, Color palette){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  if(battery != BA_NEVER){
    #ifdef PBL_PLATFORM_APLITE
      graphics_context_set_fill_color(gContext, palette.text);
      graphics_context_set_stroke_color(gContext, palette.text);
      graphics_context_set_text_color(gContext, palette.text);        
    #else
      if(battery_level < BA_PERCENT_WARNING){
        graphics_context_set_fill_color(gContext, GColorRed);
        graphics_context_set_stroke_color(gContext, GColorRed);
        graphics_context_set_text_color(gContext, GColorRed);
      }else{
        graphics_context_set_fill_color(gContext, palette.text);
        graphics_context_set_stroke_color(gContext, palette.text);
        graphics_context_set_text_color(gContext, palette.text);
      }
    #endif
  }

  if((battery == BA_UNDER_20_PERC && battery_level < BA_PERCENT_WARNING) || battery == BA_ALWAYS){
    int x, y;
    #ifdef PBL_PLATFORM_CHALK
      x = 80;
      y = 142;      
    #else
      x = 115;
      y = 7;
    #endif
    int z;
    // if i display bluetooth image too, battery sign must shift right
    #ifdef PBL_PLATFORM_CHALK
      if(bluetooth == BT_ALWAYS || (bluetooth == BT_ON_DISCONNECT && bluetooth_status == 0)){
        x += 20;
      }
    #endif
    if(battery_modality == 0){
      graphics_draw_rect(gContext, (GRect){.origin={x, y}, .size={23,13}});
      graphics_draw_line(gContext, (GPoint){.x=x+2*11+1, .y=y+4}, (GPoint){.x=x+2*11+1, .y=y+9});

      for(z=1; z<=battery_level/10; z++){
        graphics_draw_line(gContext, (GPoint){.x=x+2*z, .y=y+2}, (GPoint){.x=x+2*z, .y=y+10});
      }
      if(battery_level < BA_PERCENT_WARNING){
        graphics_fill_rect(gContext, GRect(x+10, y+2, 4, 4), 0, GCornerNone);
        graphics_fill_rect(gContext, GRect(x+11, y+6, 2, 2), 0, GCornerNone);
        graphics_fill_rect(gContext, GRect(x+11, y+9, 2, 2), 0, GCornerNone);
      }
    }else{
      #ifndef PBL_PLATFORM_CHALK
        GFont font = fonts_get_system_font(FONT_KEY_GOTHIC_14);
        graphics_draw_rect(gContext, (GRect){.origin={x, y}, .size={24,13}});
        graphics_draw_line(gContext, (GPoint){.x=x+24, .y=y+4}, (GPoint){.x=x+24, .y=y+9});
        x -= 2;
        y -= 3;
      #else
        GFont font = fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD);    
      #endif
      snprintf(battery_buffer, sizeof(battery_buffer), "%d%%", battery_level);
      graphics_draw_text(gContext, 
                         battery_buffer, 
                         font,
                         (GRect){.origin={x, y}, .size={30, 10}}, 
                         GTextOverflowModeFill,
                         GTextAlignmentCenter,
                         NULL
                        ); 
    }
  }
}

void draw_date(GContext *gContext, Color palette){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  graphics_context_set_text_color(gContext, palette.text);
  get_date_format(date);
  int x;
  int y;
  int w;
  int h = (date > 28) ? 120 : 136;
  #ifdef PBL_PLATFORM_CHALK
    x = 24;
    y = (date > 28) ? 5 : 20;
    w = 132;
  #else
    x = 0;
    y = (date > 28) ? 120 : 136;
    w = 144;
  #endif
  GRect rect = GRect(x, y, w, h);
  //////////////////////////////////////////////////////////////////
  //snprintf(date_buffer, sizeof(date_buffer), "Fri, 01 Feb 2016");
  //////////////////////////////////////////////////////////////////
  graphics_draw_text(gContext, 
                     date_buffer,
                     fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD),
                     rect, 
                     GTextOverflowModeWordWrap,
                     GTextAlignmentCenter,
                     NULL
                    );
}

void get_date_format(int dateKey){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  time_t rawtime;
  struct tm *info;
  
  time(&rawtime);
  info = localtime(&rawtime);
  switch(dateKey){
    case 0:
      strftime(date_buffer, sizeof(date_buffer), " ", info);
      break;
    case 1:
      strftime(date_buffer, sizeof(date_buffer), "%d-%m-%y", info);
      break;
    case 2:
      strftime(date_buffer, sizeof(date_buffer), "%d.%m.%y", info);
      break;
    case 3:
      strftime(date_buffer, sizeof(date_buffer), "%d/%m/%y", info);
      break;
    case 4:
      strftime(date_buffer, sizeof(date_buffer), "%d %m %y", info);
      break;
    case 5:
      strftime(date_buffer, sizeof(date_buffer), "%d-%m-%Y", info);
      break;
    case 6:
      strftime(date_buffer, sizeof(date_buffer), "%d.%m.%Y", info);
      break;
    case 7:
      strftime(date_buffer, sizeof(date_buffer), "%d/%m/%Y", info);
      break;
    case 8:
      strftime(date_buffer, sizeof(date_buffer), "%d %m %Y", info);
      break;
    case 9:
      strftime(date_buffer, sizeof(date_buffer), "%m-%d-%Y", info);
      break;
    case 10:
      strftime(date_buffer, sizeof(date_buffer), "%m.%d.%Y", info);
      break;
    case 11:
      strftime(date_buffer, sizeof(date_buffer), "%m/%d/%Y", info);
      break;
    case 12:
      strftime(date_buffer, sizeof(date_buffer), "%m %d %Y", info);
      break;
    case 13:
      strftime(date_buffer, sizeof(date_buffer), "%Y-%m-%d", info);
      break;
    case 14:
      strftime(date_buffer, sizeof(date_buffer), "%Y.%m.%d", info);
      break;
    case 15:
      strftime(date_buffer, sizeof(date_buffer), "%Y/%m/%d", info);
      break;
    case 16:
      strftime(date_buffer, sizeof(date_buffer), "%Y %m %d", info);
      break;
    case 17:
      strftime(date_buffer, sizeof(date_buffer), "%b/%d/%Y", info);
      break;
    case 18:
      strftime(date_buffer, sizeof(date_buffer), "%b %e, %Y", info);
      break;
    case 19:
      strftime(date_buffer, sizeof(date_buffer), "%d %b %Y", info);
      break;
    case 20:
      strftime(date_buffer, sizeof(date_buffer), "%d %b, %Y", info);
      break;
    case 21:
      strftime(date_buffer, sizeof(date_buffer), "%a, %b/%d/%Y", info);
      break;
    case 22:
      strftime(date_buffer, sizeof(date_buffer), "%a, %b %e, %Y", info);
      break;
    case 23:
      strftime(date_buffer, sizeof(date_buffer), "%a, %d %b %Y", info);
      break;
    case 24:
      strftime(date_buffer, sizeof(date_buffer), "%a, %d %b, %Y", info);
      break;
    case 25:
      strftime(date_buffer, sizeof(date_buffer), "%e %B %Y", info);
      break;
    case 26:
      strftime(date_buffer, sizeof(date_buffer), "%d %B %Y", info);
      break;
    case 27:
      strftime(date_buffer, sizeof(date_buffer), "%B %d, %Y", info);
      break;
    case 28:
      strftime(date_buffer, sizeof(date_buffer), "%B-%d-%Y", info);
      break;
    case 29:
      strftime(date_buffer, sizeof(date_buffer), "%a,\n%e %B %Y", info);
      break;
    case 30:
      strftime(date_buffer, sizeof(date_buffer), "%a,\n%d %B %Y", info);
      break;
    case 31:
      strftime(date_buffer, sizeof(date_buffer), "%a,\n%B %d, %Y", info);
      break;
    case 32:
      strftime(date_buffer, sizeof(date_buffer), "%a,\n%B-%d-%Y", info);
      break;
    case 33:
      strftime(date_buffer, sizeof(date_buffer), "%A,\n%e %B %Y", info);
      break;
    case 34:
      strftime(date_buffer, sizeof(date_buffer), "%A,\n%d %B %Y", info);
      break;
    case 35:
      strftime(date_buffer, sizeof(date_buffer), "%A,\n%B %d, %Y", info);
      break;
    case 36:
      strftime(date_buffer, sizeof(date_buffer), "%A,\n%B-%d-%Y", info);
      break;
  }
}

void draw_flake(GContext *gContext, struct Flake flake){
  switch(flake.size){
    case 0:
      graphics_draw_pixel(gContext, flake.pos);
      break;
    case 1:
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x,   flake.pos.y  });
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x+1, flake.pos.y  });
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x+1, flake.pos.y+1});
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x,   flake.pos.y+1});
      break;
    case 2:
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x,   flake.pos.y  });
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x,   flake.pos.y+1});
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x,   flake.pos.y+2});
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x+1, flake.pos.y+1});
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x-1, flake.pos.y+1});
      break;
    case 3:
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x-1, flake.pos.y-1});
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x+1, flake.pos.y-1});
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x,   flake.pos.y  });
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x-1, flake.pos.y+1});
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x+1, flake.pos.y+1});
      break;
    case 4:
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x,   flake.pos.y  });
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x,   flake.pos.y-1});
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x,   flake.pos.y+1});
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x+1, flake.pos.y  });
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x-1, flake.pos.y  });
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x-2, flake.pos.y-1});
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x-2, flake.pos.y+1});
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x-1, flake.pos.y-2});
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x-1, flake.pos.y+2});
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x+1, flake.pos.y-2});
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x+1, flake.pos.y+2});
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x+2, flake.pos.y-1});
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x+2, flake.pos.y+1});
      break;
    case 5:
      tmp.pos.x = flake.pos.x;
      tmp.pos.y = flake.pos.y;
      tmp.size = 3;
      tmp.pos.y = flake.pos.y-2;
      draw_flake(gContext, tmp);
      tmp.pos.y = flake.pos.y+2;
      draw_flake(gContext, tmp);
      tmp.pos.y = flake.pos.y;
      tmp.pos.x = flake.pos.x-2;
      draw_flake(gContext, tmp);
      tmp.pos.x = flake.pos.x+2;
      draw_flake(gContext, tmp);
      break;
  }
}

void shake_flakes(struct Flake *flakes){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);
  
  int x, y, size, windowWidth, windowHeight;
  windowWidth = windowHeight = 0;
  #ifdef PBL_IF_RECT_ELSE
    windowWidth = 144;
    windowHeight = 168;
  #else
    windowWidth = 180;
    windowHeight = 180;
  #endif
  // Intializes random number generator
  srand(time(NULL));
  for(int i=0; i<NUM_FLAKES; i++){
    x = rand() % windowWidth;
    y = rand() % windowHeight;
    size = rand() % 6;
    flakes[i].pos = (GPoint){x, y};
    flakes[i].size = size;
  }
}

void draw_snow(GContext *gContext, struct Flake *flakes){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);

  graphics_context_set_stroke_color(gContext, GColorWhite);
  shake_flakes(flakes);
  for(int i=0; i<NUM_FLAKES; i++){
    draw_flake(gContext, flakes[i]);
  }
}