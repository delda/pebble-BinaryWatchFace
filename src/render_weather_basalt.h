#pragma once

#include <pebble.h>
#include "common.h"
#include "weather.h"
#include "bottom_layout.h"

void render_weather_basalt(GContext *gContext, Color palette, WeatherData data,
                           const BottomLayout *bottom_layout);
