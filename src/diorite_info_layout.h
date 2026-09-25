#pragma once

#include <pebble.h>

typedef struct {
  int visible_count;
  int heart_rate_center;
  int weather_center;
  int steps_center;
  bool compact;
  bool weather_top;
} DioriteInfoLayout;

DioriteInfoLayout diorite_info_layout_get(void);
