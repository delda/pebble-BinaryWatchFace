#pragma once

#include <pebble.h>
#include "common.h"
#include "shapes_maker.h"

void render_watchface(GContext *gContext, Color palettes[], int heart_rate_bpm,
                      int steps_today, struct Flake *flakes,
                      Layer *flake_layers[NUM_FLAKES]);
