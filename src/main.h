#pragma once

#include <pebble.h>
#include "common.h"

static Window *s_window;             // main window view
static Layer *s_mainLayer;           // bullets for hours
static TextLayer *text[2][6];
static TextLayer *s_backgrounLayer;
static TextLayer *s_batteryLayer;  

static AppSync sync;
static uint8_t sync_buffer[32];

static GColor backgroundColor;
static GColor textColor;

// bluetooth
static BitmapLayer *bt_layer;
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
};

static int SHAPE_NUM = 11;
#ifdef PBL_COLOR
  static int COLOR_NUM = 15;
  static Color palette[15];
#else
  static int COLOR_NUM = 2;
  static Color palette[2];
#endif
static int BLUETOOTH_OPTIONS = 3;
static int BATTERY_OPTIONS = 3;
static int DATE_OPTIONS = 37;