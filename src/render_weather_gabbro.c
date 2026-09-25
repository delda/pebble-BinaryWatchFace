#include "render_weather_gabbro.h"
#include "weather_icons.h"

#if defined(PBL_PLATFORM_GABBRO)
void render_weather_gabbro(GContext *gContext, Color palette, WeatherData data,
                           const BottomLayout *bottom_layout) {
  char temperature[8];
  if (data.has_data) {
    snprintf(temperature, sizeof(temperature), "%d\xC2\xB0", data.temperature);
  } else {
    snprintf(temperature, sizeof(temperature), "--\xC2\xB0");
  }

  GPoint icon_origin = GPoint(60, 12);
  GRect temperature_rect = GRect(82, 11, 36, 24);
  // Preserve the existing four-element arrangement. In all other layouts,
  // weather uses the same bottom-row slots as the other active indicators.
  if (bottom_layout->visible_count != 4) {
    int center = (bottom_layout->center[BOTTOM_WEATHER] * 13 + 4) / 9;
    int y = (bottom_layout->y[BOTTOM_WEATHER] * 13 + 4) / 9;
    if (bottom_layout->visible_count == 2) {
      // Two-indicator slots are origins for all other indicators, so do not
      // offset the weather group relative to its assigned slot.
      icon_origin = GPoint(center, y);
      temperature_rect = GRect(center + 22, y - 2, 36, 24);
    } else {
      icon_origin = GPoint(center - 26, y);
      temperature_rect = GRect(center - 4, y - 2, 36, 24);
    }
  }

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
