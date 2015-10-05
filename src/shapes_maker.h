#pragma once

#include <pebble.h>
#include "common.h"
  
GPathInfo * draw_regular_shape(int number_of_sides, int w, int h, int radius);
void draw_shape(int shape, int currentWidth, int currentHeight, GContext *gContext, GColor strokeColor, GColor fillColor);
void fill_number(int number, GPoint position, GContext *gContext);
void draw_number(int number, GPoint position, GContext *gContext);
void draw_background(GContext *gContext, uint16_t corner_radius, GCornerMask corner_mask, Color palette);
void draw_time_background(GContext *gContext, Color palette);
void draw_clock(GContext *gContext, Color palette);
void draw_bluetooth(GContext *gContext);
void draw_battery(GContext *gContext, int battery, Color palette);
void draw_date(GContext *gContext, Color palette);
void get_date_format(int dateKey);