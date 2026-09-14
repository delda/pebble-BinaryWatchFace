#pragma once

#include <pebble.h>
#include "common.h"

#define NUM_FLAKES 100

struct Flake{
  GPoint pos;
  int size;
  int angle;
};

GPathInfo * draw_regular_shape(int number_of_sides, int w, int h, int radius);
void draw_shape(int shape, bool is_active, int currentWidth, int currentHeight, GContext *gContext, GColor strokeColor, GColor fillColor);
void fill_number(int number, GPoint position, GContext *gContext);
void draw_number(int number, GPoint position, GContext *gContext);
void draw_background(GContext *gContext, uint16_t corner_radius, GCornerMask corner_mask, Color palette);
void draw_time_background(GContext *gContext, Color palette, unsigned int hour, unsigned int minute);
void draw_clock(GContext *gContext, Color palette, bool draw_numbers, int shape,
                const int bullets_number[2], const int buffer_time[2][6]);
void draw_bluetooth(GContext *gContext, int bluetooth_option, int bluetooth_status,
                    int battery_option, int battery_level, int color);
void draw_battery(GContext *gContext, int battery_option, int bluetooth_option,
                  int bluetooth_status, int battery_level, int battery_modality,
                  int easter_egg, Color palette);
void draw_heart_rate(GContext *gContext, Color palette, uint8_t heart_rate_bpm,
                     bool show_steps);
void draw_steps(GContext *gContext, Color palette, int steps, bool show_heart_rate);
void draw_date(GContext *gContext, Color palette, int date_option, int easter_egg);
void draw_snow(GContext *gContext, struct Flake *flakes, Layer *flake_layers[NUM_FLAKES]);
void draw_flake(GContext *gContext, Layer *flake_layer, struct Flake flake);
void shake_flakes(struct Flake *flakes);
void anim_stopped_handler(Animation *animation, bool finished, void *context);
int isEasterEggDay();
