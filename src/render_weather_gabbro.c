#include "render_weather_gabbro.h"
#include "weather_icons.h"

#if defined(PBL_PLATFORM_GABBRO)
static int gabbro_layout_value(int value) {
  return (value * 13 + 4) / 9;
}

void render_weather_gabbro(GContext *gContext, Color palette, WeatherData data,
                           const BottomLayout *bottom_layout) {
  char temperature[8];
  if (data.has_data) {
    snprintf(temperature, sizeof(temperature), "%d\xC2\xB0", data.temperature);
  } else {
    snprintf(temperature, sizeof(temperature), "--\xC2\xB0");
  }

  GFont font = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  GSize text_size = graphics_text_layout_get_content_size(
      temperature, font, GRect(0, 0, 36, 24),
      GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft);
  int weather_x = gabbro_layout_value(bottom_layout->x[BOTTOM_WEATHER]);
  int weather_y = gabbro_layout_value(bottom_layout->y[BOTTOM_WEATHER]);
  GPoint icon_origin = GPoint(weather_x, weather_y);
  GRect temperature_rect = GRect(weather_x + 22, weather_y - 2,
                                 text_size.w, 24);
  // Preserve the existing four-element arrangement. In all other layouts,
  // weather uses the same bottom-row slots as the other active indicators.
  // The slot's left edge is the group's origin. Its width was calculated from
  // the current temperature string by bottom_layout_create().

  draw_weather_icon(gContext,
                    data.has_data ? data.forecast_icon : WEATHER_ICON_UNAVAILABLE,
                    icon_origin, palette);
  graphics_context_set_text_color(gContext, palette.text);
  graphics_draw_text(gContext, temperature,
                     fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD),
                     temperature_rect,
                     GTextOverflowModeTrailingEllipsis,
                     GTextAlignmentLeft, NULL);
}
#endif
