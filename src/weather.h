#pragma once

#include <pebble.h>

typedef enum {
  WEATHER_ICON_UNAVAILABLE = 0,
  WEATHER_ICON_CLEAR,
  WEATHER_ICON_CLOUDY,
  WEATHER_ICON_RAIN,
  WEATHER_ICON_SNOW,
  WEATHER_ICON_THUNDER,
  WEATHER_ICON_FOG,
} WeatherIcon;

typedef struct {
  int temperature;
  WeatherIcon forecast_icon;
  int32_t updated_at;
  bool has_data;
} WeatherData;

void weather_init(void);
void weather_deinit(void);
void weather_set_enabled(bool enabled);
bool weather_is_enabled(void);
void weather_set_temperature(int temperature);
void weather_set_forecast_icon(uint8_t forecast_icon);
void weather_set_updated_at(int32_t updated_at);
WeatherData weather_get_data(void);
