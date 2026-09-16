#include "weather_icons.h"

static void draw_cloud(GContext *gContext, GPoint origin) {
  graphics_fill_circle(gContext, GPoint(origin.x + 6, origin.y + 9), 5);
  graphics_fill_circle(gContext, GPoint(origin.x + 11, origin.y + 7), 6);
  graphics_fill_rect(gContext, GRect(origin.x + 3, origin.y + 9, 14, 6),
                     0, GCornerNone);
}

void draw_weather_icon(GContext *gContext, WeatherIcon icon, GPoint origin,
                       Color palette) {
  graphics_context_set_fill_color(gContext, palette.text);
  graphics_context_set_stroke_color(gContext, palette.text);
  graphics_context_set_stroke_width(gContext, 2);

  if (icon == WEATHER_ICON_CLEAR) {
    graphics_fill_circle(gContext, GPoint(origin.x + 9, origin.y + 9), 4);
    graphics_draw_line(gContext, GPoint(origin.x + 9, origin.y), GPoint(origin.x + 9, origin.y + 3));
    graphics_draw_line(gContext, GPoint(origin.x + 9, origin.y + 15), GPoint(origin.x + 9, origin.y + 18));
    graphics_draw_line(gContext, GPoint(origin.x, origin.y + 9), GPoint(origin.x + 3, origin.y + 9));
    graphics_draw_line(gContext, GPoint(origin.x + 15, origin.y + 9), GPoint(origin.x + 18, origin.y + 9));
    graphics_draw_line(gContext, GPoint(origin.x + 2, origin.y + 2), GPoint(origin.x + 4, origin.y + 4));
    graphics_draw_line(gContext, GPoint(origin.x + 14, origin.y + 14), GPoint(origin.x + 16, origin.y + 16));
    graphics_draw_line(gContext, GPoint(origin.x + 14, origin.y + 4), GPoint(origin.x + 16, origin.y + 2));
    graphics_draw_line(gContext, GPoint(origin.x + 2, origin.y + 16), GPoint(origin.x + 4, origin.y + 14));
    return;
  }

  if (icon == WEATHER_ICON_UNAVAILABLE) {
    graphics_draw_circle(gContext, GPoint(origin.x + 9, origin.y + 9), 8);
    graphics_draw_text(gContext, "?", fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD),
                       GRect(origin.x + 3, origin.y - 3, 13, 18),
                       GTextOverflowModeFill, GTextAlignmentCenter, NULL);
    return;
  }

  if (icon == WEATHER_ICON_SNOW) {
    // Three equal axes meet at the centre, creating six evenly spaced arms.
    graphics_context_set_stroke_width(gContext, 1);
    graphics_draw_line(gContext, GPoint(origin.x + 9, origin.y + 5), GPoint(origin.x + 9, origin.y + 13));
    graphics_draw_line(gContext, GPoint(origin.x + 9, origin.y + 5), GPoint(origin.x + 7, origin.y));
    graphics_draw_line(gContext, GPoint(origin.x + 9, origin.y + 5), GPoint(origin.x + 11, origin.y));
    graphics_draw_line(gContext, GPoint(origin.x + 9, origin.y + 13), GPoint(origin.x + 7, origin.y + 18));
    graphics_draw_line(gContext, GPoint(origin.x + 9, origin.y + 13), GPoint(origin.x + 11, origin.y + 18));

    graphics_draw_line(gContext, GPoint(origin.x + 5, origin.y + 7), GPoint(origin.x + 13, origin.y + 11));
    graphics_draw_line(gContext, GPoint(origin.x + 5, origin.y + 7), GPoint(origin.x + 3, origin.y + 3));
    graphics_draw_line(gContext, GPoint(origin.x + 5, origin.y + 7), GPoint(origin.x + 1, origin.y + 8));
    graphics_draw_line(gContext, GPoint(origin.x + 13, origin.y + 11), GPoint(origin.x + 15, origin.y + 15));
    graphics_draw_line(gContext, GPoint(origin.x + 13, origin.y + 11), GPoint(origin.x + 17, origin.y + 10));

    graphics_draw_line(gContext, GPoint(origin.x + 13, origin.y + 7), GPoint(origin.x + 5, origin.y + 11));
    graphics_draw_line(gContext, GPoint(origin.x + 13, origin.y + 7), GPoint(origin.x + 15, origin.y + 3));
    graphics_draw_line(gContext, GPoint(origin.x + 13, origin.y + 7), GPoint(origin.x + 17, origin.y + 8));
    graphics_draw_line(gContext, GPoint(origin.x + 5, origin.y + 11), GPoint(origin.x + 3, origin.y + 15));
    graphics_draw_line(gContext, GPoint(origin.x + 5, origin.y + 11), GPoint(origin.x + 1, origin.y + 10));

    return;
  }

  if (icon == WEATHER_ICON_FOG) {
    // Fog is intentionally only three horizontal bands, without a cloud.
    graphics_draw_line(gContext, GPoint(origin.x + 2, origin.y + 5), GPoint(origin.x + 16, origin.y + 5));
    graphics_draw_line(gContext, GPoint(origin.x, origin.y + 10), GPoint(origin.x + 18, origin.y + 10));
    graphics_draw_line(gContext, GPoint(origin.x + 3, origin.y + 15), GPoint(origin.x + 17, origin.y + 15));
    return;
  }

  if (icon == WEATHER_ICON_THUNDER) {
    // A familiar, bold lightning silhouette with two sharp triangular turns.
    GPoint points[] = {{origin.x + 12, origin.y}, {origin.x + 3, origin.y + 11},
                       {origin.x + 8, origin.y + 11}, {origin.x + 5, origin.y + 21},
                       {origin.x + 17, origin.y + 9}, {origin.x + 12, origin.y + 9}};
    GPathInfo path_info = {.num_points = ARRAY_LENGTH(points), .points = points};
    GPath *path = gpath_create(&path_info);
    if (path != NULL) {
      gpath_draw_filled(gContext, path);
      gpath_destroy(path);
    }
    return;
  }

  draw_cloud(gContext, origin);
  if (icon == WEATHER_ICON_CLOUDY) {
    return;
  }
  if (icon == WEATHER_ICON_RAIN) {
    graphics_draw_line(gContext, GPoint(origin.x + 6, origin.y + 16), GPoint(origin.x + 4, origin.y + 19));
    graphics_draw_line(gContext, GPoint(origin.x + 11, origin.y + 16), GPoint(origin.x + 9, origin.y + 19));
    graphics_draw_line(gContext, GPoint(origin.x + 16, origin.y + 16), GPoint(origin.x + 14, origin.y + 19));
  }
}
