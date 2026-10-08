#include "render_layout.h"
#include "shapes_maker.h"

#if !defined(PBL_PLATFORM_EMERY) && !defined(PBL_PLATFORM_GABBRO) && \
    !defined(PBL_PLATFORM_DIORITE) && !defined(PBL_PLATFORM_BASALT) && \
    !defined(PBL_PLATFORM_FLINT)
void render_layout_draw_health_indicators(GContext *gContext, Color palette,
                                          bool show_heart_rate, bool show_steps,
                                          int heart_rate_bpm, int steps_today,
                                          const BottomLayout *bottom_layout) {
  (void)show_heart_rate;
  (void)heart_rate_bpm;
#if defined(PBL_HEALTH)
  if (show_steps) {
    draw_steps(gContext, palette, steps_today, show_heart_rate, bottom_layout);
  }
#else
  (void)gContext;
  (void)palette;
  (void)show_steps;
  (void)steps_today;
  (void)bottom_layout;
#endif
}
#endif
