#include "render.h"
#include "render_layout.h"

void render_watchface(GContext *gContext, Color palettes[], int heart_rate_bpm,
                      int steps_today, struct Flake *flakes,
                      Layer *flake_layers[NUM_FLAKES]) {
  int easter_egg = isEasterEggDay();
  if (easter_egg != 0) {
#ifdef PBL_PLATFORM_APLITE
    color = 0;
#else
    shape = (easter_egg == 2) ? 11 : shape;
    color = (easter_egg == 1) ? 15 : 16;
#endif
  }

  Color palette = palettes[color];
  draw_background(gContext, 0, GCornerNone, palette);
  if (number > 0) {
    draw_time_background(gContext, palette);
  }
  draw_clock(gContext, palette, (bool)help_num);

  if (bluetooth > 0) {
    draw_bluetooth(gContext);
  }
  if (battery > 0) {
    draw_battery(gContext, battery, palette);
  }
  if (date > 0) {
    draw_date(gContext, palette);
  }

  render_layout_draw_health_indicators(gContext, palette, show_heart_rate,
                                        show_steps, heart_rate_bpm, steps_today);

  if (easter_egg != 0 || snow) {
    for (int i = 0; i < NUM_FLAKES; i++) {
      draw_flake(gContext, flake_layers[i], flakes[i]);
    }
  }
}
