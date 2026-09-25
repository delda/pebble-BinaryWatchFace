#include "render_weather_diorite.h"
#include "diorite_info_layout.h"
#include "weather_icons.h"

#if defined(PBL_PLATFORM_DIORITE)
void render_weather_diorite(GContext *gContext, Color palette, WeatherData data) {
  DioriteInfoLayout layout = diorite_info_layout_get();
  char temperature[8];
  if (data.has_data) {
    snprintf(temperature, sizeof(temperature), "%d\xC2\xB0", data.temperature);
  } else {
    snprintf(temperature, sizeof(temperature), "--\xC2\xB0");
  }

  int temperature_width = layout.compact ? 31 : 45;
  int group_width = 19 + temperature_width;
  int icon_x = layout.weather_center - group_width / 2;
  int y = layout.weather_top ? 2 : 116;
  int top_layout_offset = layout.weather_top ? 1 : 0;

  draw_weather_icon(gContext,
                    data.has_data ? data.forecast_icon : WEATHER_ICON_UNAVAILABLE,
                    GPoint(icon_x + top_layout_offset * 10,
                           y + top_layout_offset * 5), palette);
  graphics_context_set_text_color(gContext, palette.text);
  graphics_draw_text(gContext, temperature,
                     fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD),
                     GRect(icon_x + 21 + top_layout_offset * 10,
                           y - 2 + top_layout_offset * 2,
                           temperature_width, 24),
                     GTextOverflowModeTrailingEllipsis,
                     GTextAlignmentLeft, NULL);
}
#endif
