#pragma once

#include <pebble.h>

enum BottomElement {
  BOTTOM_BLUETOOTH,
  BOTTOM_HEART_RATE,
  BOTTOM_STEPS,
  BOTTOM_BATTERY,
  BOTTOM_WEATHER,
  BOTTOM_ELEMENT_COUNT,
};

typedef struct {
  int visible_count;
  int x[BOTTOM_ELEMENT_COUNT];
  int y[BOTTOM_ELEMENT_COUNT];
  int center[BOTTOM_ELEMENT_COUNT];
  int slot_width[BOTTOM_ELEMENT_COUNT];
  int row[BOTTOM_ELEMENT_COUNT];
  int steps_text_x_offset;
  int steps_text_y_offset;
} BottomLayout;

void bottom_layout_calculate(const bool visible[BOTTOM_ELEMENT_COUNT],
                             BottomLayout *layout);
