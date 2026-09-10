#include "render_layout.h"
#include "shapes_maker.h"

#if defined(PBL_PLATFORM_EMERY)
void render_layout_draw_health_indicators(GContext *gContext, Color palette,
                                          bool show_heart_rate, bool show_steps,
                                          int heart_rate_bpm, int steps_today) {
  if (show_heart_rate) {
    draw_heart_rate(gContext, palette, heart_rate_bpm);
  }
  if (show_steps) {
    draw_steps(gContext, palette, steps_today);
  }
}
#endif
