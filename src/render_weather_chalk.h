#pragma once

#include "weather.h"
#include "common.h"
#include "bottom_layout.h"

void render_weather_chalk(GContext *gContext, Color palette, WeatherData data,
                          const BottomLayout *bottom_layout);
