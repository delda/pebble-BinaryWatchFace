#include "render.h"
#include "render_layout.h"

void render_watchface(GContext *gContext, Color palettes[], const RenderState *state,
                      struct Flake *flakes,
                      Layer *flake_layers[NUM_FLAKES]) {
  int easter_egg = isEasterEggDay();
  RenderState render_state = *state;
  if (easter_egg != 0) {
    render_state.shape = (easter_egg == 2) ? 11 : render_state.shape;
    render_state.shape = (easter_egg == 3) ? 12 : render_state.shape;
    render_state.shape = (easter_egg == 4) ? 13 : render_state.shape;
#ifdef PBL_PLATFORM_APLITE
    render_state.color = 0;
#else
    render_state.color = (easter_egg == 1) ? 15 :
                         (easter_egg == 2) ? 16 :
                         (easter_egg == 3) ? 17 : 18;
#endif
  }

  Color palette = palettes[render_state.color];
  draw_background(gContext, 0, GCornerNone, palette);
  if (render_state.number > 0 && easter_egg != 4) {
    draw_time_background(gContext, palette, render_state.hour, render_state.minute);
  }
  draw_clock(gContext, palette, (bool)render_state.help_num, render_state.shape,
             render_state.bullets_number, render_state.buffer_time);

  if (render_state.bluetooth > 0) {
    draw_bluetooth(gContext, render_state.bluetooth, render_state.bluetooth_status,
                   render_state.battery, render_state.battery_level, render_state.color);
  }
  if (render_state.battery > 0) {
    draw_battery(gContext, render_state.battery, render_state.bluetooth,
                 render_state.bluetooth_status, render_state.battery_level,
                 render_state.battery_modality, easter_egg, palette);
  }
  // Easter keeps its greeting visible even if the regular date is disabled.
  if (render_state.date > 0 || easter_egg == 4) {
    draw_date(gContext, palette, render_state.date, easter_egg);
  }

  render_layout_draw_health_indicators(gContext, palette, render_state.show_heart_rate,
                                        render_state.show_steps, render_state.heart_rate_bpm,
                                        render_state.steps_today);

  if (easter_egg == 1 || easter_egg == 2 || render_state.snow) {
    for (int i = 0; i < NUM_FLAKES; i++) {
      draw_flake(gContext, flake_layers[i], flakes[i]);
    }
  }
}
