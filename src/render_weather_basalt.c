#include "render_weather_basalt.h"
#include "weather_icons.h"

#if defined(PBL_PLATFORM_BASALT) || defined(PBL_PLATFORM_FLINT)
void render_weather_basalt(GContext *gContext, Color palette, WeatherData data,
                           const BottomLayout *bottom_layout) {
  char temperature[8];
  if (data.has_data) {
    snprintf(temperature, sizeof(temperature), "%d\xC2\xB0", data.temperature);
  } else {
    snprintf(temperature, sizeof(temperature), "--\xC2\xB0");
  }

  int weather_x = bottom_layout->x[BOTTOM_WEATHER];
  int weather_y = bottom_layout->y[BOTTOM_WEATHER];
  draw_weather_icon(gContext,
                    data.has_data ? data.forecast_icon : WEATHER_ICON_UNAVAILABLE,
                    GPoint(weather_x, weather_y), palette);
  graphics_context_set_text_color(gContext, palette.text);
  graphics_draw_text(gContext, temperature,
                     fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD),
                     GRect(weather_x + 22, weather_y, 38, 20),
                     GTextOverflowModeTrailingEllipsis,
                     GTextAlignmentLeft, NULL);
}
#endif
