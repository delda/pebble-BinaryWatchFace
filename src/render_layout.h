#pragma once

#include <pebble.h>
#include "common.h"

void render_layout_draw_health_indicators(GContext *gContext, Color palette,
                                          bool show_heart_rate, bool show_steps,
                                          int heart_rate_bpm, int steps_today);
