#pragma once

#include <pebble.h>
#include "common.h"
#include "weather.h"

void draw_weather_icon(GContext *gContext, WeatherIcon icon, GPoint origin,
                       Color palette);
