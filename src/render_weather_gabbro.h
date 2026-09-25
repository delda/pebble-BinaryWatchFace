#pragma once

#include <pebble.h>
#include "common.h"
#include "shapes_maker.h"
#include "weather.h"

void render_weather_gabbro(GContext *gContext, Color palette, WeatherData data,
                           const BottomLayout *bottom_layout);
