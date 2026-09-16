#pragma once

#include <pebble.h>
#include "common.h"
#include "shapes_maker.h"

typedef struct {
  unsigned int hour;
  unsigned int minute;
  int bullets_number[2];
  int buffer_time[2][6];
  int shape;
  int color;
  int number;
  int bluetooth;
  int bluetooth_status;
  int battery;
  int battery_level;
  int battery_modality;
  int date;
  int help_num;
  int snow;
  int show_heart_rate;
  int show_steps;
  int heart_rate_bpm;
  int steps_today;
  bool weather_enabled;
} RenderState;

void render_watchface(GContext *gContext, Color palettes[], const RenderState *state,
                      struct Flake *flakes,
                      Layer *flake_layers[NUM_FLAKES]);
