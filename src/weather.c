#include "weather.h"

enum WeatherPersistKey {
  WEATHER_ENABLED_PERSIST_KEY = 0xB,
  WEATHER_TEMPERATURE_PERSIST_KEY = 0xC,
  WEATHER_ICON_PERSIST_KEY = 0xD,
  WEATHER_UPDATED_AT_PERSIST_KEY = 0xE,
};

static bool s_enabled;
static WeatherData s_data;

void weather_init(void) {
  s_enabled = true;
  s_data.temperature = 0;
  s_data.forecast_icon = WEATHER_ICON_UNAVAILABLE;
  s_data.updated_at = 0;
  s_data.has_data = false;

  if (persist_exists(WEATHER_ENABLED_PERSIST_KEY)) {
    s_enabled = persist_read_bool(WEATHER_ENABLED_PERSIST_KEY);
  }
  if (persist_exists(WEATHER_TEMPERATURE_PERSIST_KEY) &&
      persist_exists(WEATHER_ICON_PERSIST_KEY) &&
      persist_exists(WEATHER_UPDATED_AT_PERSIST_KEY)) {
    s_data.temperature = persist_read_int(WEATHER_TEMPERATURE_PERSIST_KEY);
    s_data.forecast_icon = (WeatherIcon)persist_read_int(WEATHER_ICON_PERSIST_KEY);
    s_data.updated_at = persist_read_int(WEATHER_UPDATED_AT_PERSIST_KEY);
    s_data.has_data = s_data.forecast_icon > WEATHER_ICON_UNAVAILABLE &&
                      s_data.forecast_icon <= WEATHER_ICON_FOG;
  }
}

void weather_deinit(void) {
}

void weather_set_enabled(bool enabled) {
  s_enabled = enabled;
  persist_write_bool(WEATHER_ENABLED_PERSIST_KEY, s_enabled);
}

bool weather_is_enabled(void) {
  return s_enabled;
}

void weather_set_temperature(int temperature) {
  if (temperature < -99 || temperature > 99) {
    return;
  }
  s_data.temperature = temperature;
  persist_write_int(WEATHER_TEMPERATURE_PERSIST_KEY, temperature);
}

void weather_set_forecast_icon(uint8_t forecast_icon) {
  if (forecast_icon > WEATHER_ICON_FOG) {
    forecast_icon = WEATHER_ICON_UNAVAILABLE;
  }
  s_data.forecast_icon = (WeatherIcon)forecast_icon;
  persist_write_int(WEATHER_ICON_PERSIST_KEY, forecast_icon);
}

void weather_set_updated_at(int32_t updated_at) {
  if (updated_at <= 0) {
    return;
  }
  s_data.updated_at = updated_at;
  s_data.has_data = s_data.forecast_icon != WEATHER_ICON_UNAVAILABLE;
  persist_write_int(WEATHER_UPDATED_AT_PERSIST_KEY, updated_at);
}

WeatherData weather_get_data(void) {
  return s_data;
}
