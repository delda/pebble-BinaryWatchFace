#include "render_weather_emery.h"
#include "weather_icons.h"

#if defined(PBL_PLATFORM_EMERY)
void render_weather_emery(GContext *gContext, Color palette, WeatherData data) {
  char temperature[8];
  if (data.has_data) {
    snprintf(temperature, sizeof(temperature), "%d\xC2\xB0", data.temperature);
  } else {
    snprintf(temperature, sizeof(temperature), "--\xC2\xB0");
  }

  draw_weather_icon(gContext,
                    data.has_data ? data.forecast_icon : WEATHER_ICON_UNAVAILABLE,
                    GPoint(61, 160), palette);
  graphics_context_set_text_color(gContext, palette.text);
  graphics_draw_text(gContext, temperature,
                     fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),
                     GRect(83, 153, 31, 30), GTextOverflowModeTrailingEllipsis,
                     GTextAlignmentLeft, NULL);
}
#endif
