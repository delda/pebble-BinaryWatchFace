#pragma once

#include <pebble.h>
#include "common.h"
#include "weather.h"

void render_weather_emery(GContext *gContext, Color palette, WeatherData data);
