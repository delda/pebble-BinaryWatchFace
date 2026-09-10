#include "render_layout.h"
#include "shapes_maker.h"

#if !defined(PBL_PLATFORM_EMERY) && !defined(PBL_PLATFORM_GABBRO)
void render_layout_draw_health_indicators(GContext *gContext, Color palette,
                                          bool show_heart_rate, bool show_steps,
                                          int heart_rate_bpm, int steps_today) {
  (void)show_heart_rate;
  (void)heart_rate_bpm;
#if defined(PBL_HEALTH)
  if (show_steps) {
    draw_steps(gContext, palette, steps_today);
  }
#else
  (void)gContext;
  (void)palette;
  (void)show_steps;
  (void)steps_today;
#endif
}
#endif
