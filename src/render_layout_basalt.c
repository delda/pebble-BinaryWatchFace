#include "render_layout.h"
#include "shapes_maker.h"

#if defined(PBL_PLATFORM_BASALT) || defined(PBL_PLATFORM_FLINT)
void render_layout_draw_health_indicators(GContext *gContext, Color palette,
                                          bool show_heart_rate, bool show_steps,
                                          int heart_rate_bpm, int steps_today,
                                          const BottomLayout *bottom_layout) {
#if defined(PBL_HEALTH)
  if (show_heart_rate) {
    draw_heart_rate(gContext, palette, heart_rate_bpm, show_steps, bottom_layout);
  }
  if (show_steps) {
    draw_steps(gContext, palette, steps_today, show_heart_rate, bottom_layout);
  }
#else
  (void)gContext;
  (void)palette;
  (void)show_heart_rate;
  (void)show_steps;
  (void)heart_rate_bpm;
  (void)steps_today;
  (void)bottom_layout;
#endif
}
#endif
