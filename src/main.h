#pragma once

#include <pebble.h>
#include "common.h"

static Window *s_window;             // main window view
static Layer *s_mainLayer;           // bullets for hours
// Vibe pattern: ON for 200ms, OFF for 100ms, ON for 400ms:
static const uint32_t segments[] = { 500, 500, 500 };
VibePattern bt_vibe = {
  .durations = segments,
  .num_segments = ARRAY_LENGTH(segments),
};

enum appParameters {
  SHAPE_KEY       = 0x0,
  COLOR_KEY       = 0x1,
  NUMBER_KEY      = 0x2,
  BLUETOOTH_KEY   = 0X3,
  BATTERY_KEY     = 0x4,
  BATTERY_MOD_KEY = 0x5,
  DATE_KEY        = 0x6,
  HELP_NUM_KEY    = 0x7,
  SNOW_KEY        = 0x8,
  HEART_RATE_KEY  = 0x9,
  STEPS_KEY       = 0xA,
  WEATHER_ENABLED_KEY = 0xB,
  WEATHER_TEMPERATURE_KEY = 0xC,
  WEATHER_FORECAST_ICON_KEY = 0xD,
  WEATHER_UPDATED_AT_KEY = 0xE,
  WEATHER_REQUEST_KEY = 0xF,
};

static int SHAPE_NUM = 13;
#ifdef PBL_COLOR
  static int COLOR_NUM = 19;
  static Color palette[19];
#else
  static int COLOR_NUM = 3;
  static Color palette[3];
#endif
static int BLUETOOTH_OPTIONS = 3;
static int BATTERY_OPTIONS = 3;
static int DATE_OPTIONS = 37;
