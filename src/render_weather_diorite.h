#pragma once

#include <pebble.h>
#include "common.h"
#include "weather.h"

void render_weather_diorite(GContext *gContext, Color palette, WeatherData data);
